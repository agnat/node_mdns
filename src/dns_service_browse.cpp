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
    Nan::HandleScope scope;
    ServiceRef * serviceRef = static_cast<ServiceRef*>(context);
    Local<Function> callback = serviceRef->GetCallback();
    Local<Object> this_ = serviceRef->GetThis();

    const size_t argc(8);
    Local<Value> info[argc];
    info[0] = serviceRef->handle();
    info[1] = Nan::New<Integer>(flags);
    info[2] = Nan::New<Uint32>(interfaceIndex);
    info[3] = Nan::New<Integer>(errorCode);
    info[4] = stringOrUndefined(serviceName);
    info[5] = stringOrUndefined(serviceType);
    info[6] = stringOrUndefined(replyDomain);
    if (serviceRef->GetContext().IsEmpty()) {
        info[7] = Nan::Undefined();
    } else {
        info[7] = serviceRef->GetContext();
    }
    Nan::AsyncResource asyncResource(LOC(__FILE__, __LINE__));
    asyncResource.runInAsyncScope(this_, callback, argc, info);
}

NAN_METHOD(DNSServiceBrowse) { 
    if (argumentCountMismatch(info, 7)) {
        return throwArgumentCountMismatchException(info, 7);
    }

    if ( ! ServiceRef::HasInstance(info[0])) {
        return throwTypeError("argument 1 must be a DNSServiceRef (sdRef)");
    }
    ServiceRef * serviceRef = Nan::ObjectWrap::Unwrap<ServiceRef>(ToObject(info[0]));
    if (serviceRef->IsInitialized()) {
        return throwError("DNSServiceRef is already initialized");
    }

    if ( ! info[1]->IsInt32()) {
        return throwError("argument 2 must be an integer (DNSServiceFlags)");
    }
    DNSServiceFlags flags = ToInt32(info[1]);

    if ( ! info[2]->IsUint32() && ! info[2]->IsInt32()) {
        return throwTypeError("argument 3 must be an integer (interfaceIndex)");
    }
    uint32_t interfaceIndex = ToUint32(info[2]);

    if ( ! info[3]->IsString()) {
        return throwTypeError("argument 4 must be a string (service type)");
    }
    Nan::Utf8String serviceType(ToString(info[3]));

    bool has_domain = false;
    if ( ! info[4]->IsNull() && ! info[4]->IsUndefined()) {
        if ( ! info[4]->IsString()) {
            return throwTypeError("argument 5 must be a string (domain)");
        }
        has_domain = true;
    }
    Nan::Utf8String domain(info[4]);

    if ( ! info[5]->IsFunction()) {
        return throwTypeError("argument 6 must be a function (callBack)");
    }
    serviceRef->SetCallback(Local<Function>::Cast(info[5]));

    if ( ! info[6]->IsNull() && ! info[6]->IsUndefined()) {
        serviceRef->SetContext(info[6]);
    }

    DNSServiceErrorType error = DNSServiceBrowse( & serviceRef->GetServiceRef(),
            flags, interfaceIndex, *serviceType, has_domain ? *domain : NULL,
            OnServiceChanged, serviceRef);

    if (error != kDNSServiceErr_NoError) {
        return throwMdnsError(error);
    }

    if ( ! serviceRef->SetSocketFlags()) {
        return throwError("Failed to set socket flags (O_NONBLOCK, FD_CLOEXEC)");
    }
}

} // end of namespace node_mdns
