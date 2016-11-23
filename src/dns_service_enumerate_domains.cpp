#include "mdns.hpp"

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

void
DNSSD_API
OnEnumeration(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex,
        DNSServiceErrorType errorCode, const char * replyDomain, void * context)
{
    Nan::HandleScope scope;
    ServiceRef * serviceRef = static_cast<ServiceRef*>(context);
    Local<Function> callback = serviceRef->GetCallback();
    Handle<Object> this_ = serviceRef->GetThis();

    const size_t argc(6);
    Local<Value> info[argc];
    info[0] = serviceRef->handle();
    info[1] = Nan::New<Integer>(flags);
    info[2] = Nan::New<Uint32>(interfaceIndex);
    info[3] = Nan::New<Integer>(errorCode);
    info[4] = stringOrUndefined(replyDomain);
    info[5] = serviceRef->GetContext();
    Nan::MakeCallback(this_, callback, argc, info);
}

NAN_METHOD(DNSServiceEnumerateDomains) {
    Nan::HandleScope scope;
    if (argumentCountMismatch(info, 5)) {
        info.GetReturnValue().Set(throwArgumentCountMismatchException(info, 5));
    }

    if ( ! ServiceRef::HasInstance(info[0])) {
        info.GetReturnValue().Set(throwTypeError("argument 1 must be a DNSServiceRef (sdRef)"));
    }
    ServiceRef * serviceRef = Nan::ObjectWrap::Unwrap<ServiceRef>(info[0]->ToObject());
    if (serviceRef->IsInitialized()) {
        info.GetReturnValue().Set(throwError("DNSServiceRef is already initialized"));
    }

    if ( ! info[1]->IsInt32()) {
        info.GetReturnValue().Set(throwError("argument 2 must be an integer (DNSServiceFlags)"));
    }
    DNSServiceFlags flags = info[1]->ToInteger()->Int32Value();

    if ( ! info[2]->IsUint32() && ! info[2]->IsInt32()) {
       info.GetReturnValue().Set(throwTypeError("argument 3 must be an integer (interfaceIndex)"));
    }
    uint32_t interfaceIndex = info[2]->ToInteger()->Uint32Value();

    if ( ! info[3]->IsFunction()) {
        info.GetReturnValue().Set(throwTypeError("argument 4 must be a function (callBack)"));
    }
    serviceRef->SetCallback(Local<Function>::Cast(info[3]));

    serviceRef->SetContext(info[4]);

    DNSServiceErrorType error = DNSServiceEnumerateDomains( & serviceRef->GetServiceRef(),
            flags, interfaceIndex, OnEnumeration, serviceRef);

    if (error != kDNSServiceErr_NoError) {
        info.GetReturnValue().Set(throwMdnsError(error));
    }
    if ( ! serviceRef->SetSocketFlags()) {
        info.GetReturnValue().Set(throwError("Failed to set socket flags (O_NONBLOCK, FD_CLOEXEC)"));
    }
    return;
}

} // end of namespace node_mdns
