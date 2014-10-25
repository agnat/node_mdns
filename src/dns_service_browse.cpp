#include "mdns.hpp"
#include <v8.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

static
void
DNSSD_API
OnServiceChanged(DNSServiceRef sdRef, DNSServiceFlags flags, 
        uint32_t interfaceIndex, DNSServiceErrorType errorCode, 
        const char * serviceName, const char * serviceType,
        const char * replyDomain, void * context)
{
    NanScope();
    ServiceRef * serviceRef = static_cast<ServiceRef*>(context);
    Handle<Function> callback = serviceRef->GetCallback();
    Handle<Object> this_ = serviceRef->GetThis();

    const size_t argc(8);
    Local<Value> args[argc];
    args[0] = NanObjectWrapHandle(serviceRef);
    args[1] = NanNew<Integer>(flags);
    args[2] = NanNew<Uint32>(interfaceIndex);
    args[3] = NanNew<Integer>(errorCode);
    args[4] = stringOrUndefined(serviceName);
    args[5] = stringOrUndefined(serviceType);
    args[6] = stringOrUndefined(replyDomain);
    if (serviceRef->GetContext().IsEmpty()) {
        args[7] = NanUndefined();
    } else {
        args[7] = NanNew<Value>(serviceRef->GetContext());
    }
    NanMakeCallback(this_, callback, argc, args);
}

NAN_METHOD(DNSServiceBrowse) {
    NanScope(); 
    if (argumentCountMismatch(args, 7)) {
        NanReturnValue(throwArgumentCountMismatchException(args, 7));
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

    if ( ! args[3]->IsString()) {
        NanReturnValue(throwTypeError("argument 4 must be a string (service type)"));
    }
    String::Utf8Value serviceType(args[3]->ToString());

    bool has_domain = false;
    if ( ! args[4]->IsNull() && ! args[4]->IsUndefined()) {
        if ( ! args[4]->IsString()) {
            NanReturnValue(throwTypeError("argument 5 must be a string (domain)"));
        }
        has_domain = true;
    }
    String::Utf8Value domain(args[4]);

    if ( ! args[5]->IsFunction()) {
        NanReturnValue(throwTypeError("argument 6 must be a function (callBack)"));
    }
    serviceRef->SetCallback(Local<Function>::Cast(args[5]));

    if ( ! args[6]->IsNull() && ! args[6]->IsUndefined()) {
        serviceRef->SetContext(args[6]);
    }

    DNSServiceErrorType error = DNSServiceBrowse( & serviceRef->GetServiceRef(),
            flags, interfaceIndex, *serviceType, has_domain ? *domain : NULL,
            OnServiceChanged, serviceRef);

    if (error != kDNSServiceErr_NoError) {
        NanReturnValue(throwMdnsError(error));
    }

    if ( ! serviceRef->SetSocketFlags()) {
        NanReturnValue(throwError("Failed to set socket flags (O_NONBLOCK, FD_CLOEXEC)"));
    }

    NanReturnUndefined();
}

} // end of namespace node_mdns
