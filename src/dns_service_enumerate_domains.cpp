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
    NanScope();
    ServiceRef * serviceRef = static_cast<ServiceRef*>(context);
    Handle<Function> callback = serviceRef->GetCallback();
    Handle<Object> this_ = serviceRef->GetThis();

    const size_t argc(6);
    Local<Value> args[argc];
    args[0] = NanNewLocal<Object>(NanObjectWrapHandle(serviceRef));
    args[1] = Integer::New(flags);
    args[2] = Integer::NewFromUnsigned(interfaceIndex);
    args[3] = Integer::New(errorCode);
    args[4] = stringOrUndefined(replyDomain);
    args[5] = NanNewLocal<Value>(serviceRef->GetContext());
    callback->Call(this_, argc, args);
}

NAN_METHOD(DNSServiceEnumerateDomains) {
    NanScope();
    if (argumentCountMismatch(args, 5)) {
        NanReturnValue(throwArgumentCountMismatchException(args, 5));
    }
    
    if ( ! ServiceRef::HasInstance(args[0])) {
        NanReturnValue(throwTypeError("argument 1 must be a DNSServiceRef (sdRef)"));
    }
    ServiceRef * serviceRef = ObjectWrap::Unwrap<ServiceRef>(args[0]->ToObject());
    if (serviceRef->IsInitialized()) {
        NanReturnValue(throwError("DNSServiceRef is already initialized"));
    }

    if ( ! args[1]->IsInt32()) {
        NanReturnValue(throwError("argument 2 must be an integer (DNSServiceFlags)"));
    }
    DNSServiceFlags flags = args[1]->ToInteger()->Int32Value();

    if ( ! args[2]->IsUint32() && ! args[2]->IsInt32()) {
       NanReturnValue(throwTypeError("argument 3 must be an integer (interfaceIndex)"));
    }
    uint32_t interfaceIndex = args[2]->ToInteger()->Uint32Value();

    if ( ! args[3]->IsFunction()) {
        NanReturnValue(throwTypeError("argument 4 must be a function (callBack)"));
    }
    serviceRef->SetCallback(Local<Function>::Cast(args[3]));

    serviceRef->SetContext(args[4]);

    DNSServiceErrorType error = DNSServiceEnumerateDomains( & serviceRef->GetServiceRef(),
            flags, interfaceIndex, OnEnumeration, serviceRef);

    if (error != kDNSServiceErr_NoError) {
        NanReturnValue(throwMdnsError(error));
    }
    if ( ! serviceRef->SetSocketFlags()) {
        NanReturnValue(throwError("Failed to set socket flags (O_NONBLOCK, FD_CLOEXEC)"));
    }
    NanReturnUndefined();
}

} // end of namespace node_mdns
