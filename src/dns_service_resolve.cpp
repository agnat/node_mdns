#include "mdns.hpp"

#include <string.h>

#ifndef WIN32 // XXX
#include <arpa/inet.h>
#endif

#include <v8.h>
#include <node_buffer.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"


using namespace v8;
using namespace node;

namespace node_mdns {

void
DNSSD_API
OnResolve(DNSServiceRef sdRef, DNSServiceFlags flags,
        uint32_t interfaceIndex, DNSServiceErrorType errorCode,
        const char * fullname, const char * hosttarget, uint16_t port,
        uint16_t txtLen, const unsigned char * txtRecord, void * context)
{

    Nan::HandleScope scope;
    ServiceRef * serviceRef = static_cast<ServiceRef*>(context);
    Local<Function> callback = serviceRef->GetCallback();
    Local<Object> this_ = serviceRef->GetThis();

    const size_t argc(9);
    Local<Value> info[argc];
    info[0] = serviceRef->handle();
    info[1] = Nan::New<Integer>(flags);
    info[2] = Nan::New<Uint32>(interfaceIndex);
    info[3] = Nan::New<Integer>(errorCode);
    info[4] = stringOrUndefined(fullname);
    info[5] = stringOrUndefined(hosttarget);
    info[6] = Nan::New<Integer>( ntohs(port) );
    Local<Object> buffer = Nan::NewBuffer(txtLen).ToLocalChecked();
    memcpy(Buffer::Data(buffer), txtRecord, txtLen);
    info[7] = buffer;
    if (serviceRef->GetContext().IsEmpty()) {
        info[8] = Nan::Undefined();
    } else {
        info[8] = serviceRef->GetContext();
    }
    Nan::AsyncResource asyncResource(LOC(__FILE__, __LINE__));
    asyncResource.runInAsyncScope(this_, callback, argc, info);
}

NAN_METHOD(DNSServiceResolve) {

    if (argumentCountMismatch(info, 8)) {
        return throwArgumentCountMismatchException(info, 8);
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
        return throwTypeError("argument 4 must be a string (name)");
    }
    Nan::Utf8String name(info[3]);

    if ( ! info[4]->IsString()) {
        return throwTypeError("argument 5 must be a string (service type)");
    }
    Nan::Utf8String serviceType(info[4]);

    if ( ! info[5]->IsString()) {
        return throwTypeError("argument 6 must be a string (domain)");
    }
    Nan::Utf8String domain(info[5]);

    if ( ! info[6]->IsFunction()) {
        return throwTypeError("argument 7 must be a function (callBack)");
    }
    serviceRef->SetCallback(Local<Function>::Cast(info[6]));

    if ( ! info[7]->IsNull() && ! info[7]->IsUndefined()) {
        serviceRef->SetContext(info[7]);
    }

    DNSServiceErrorType error = DNSServiceResolve( & serviceRef->GetServiceRef(),
            flags, interfaceIndex, *name, *serviceType, *domain, OnResolve, serviceRef);

    if (error != kDNSServiceErr_NoError) {
        return throwMdnsError(error);
    }
    if ( ! serviceRef->SetSocketFlags()) {
        return throwError("Failed to set socket flags (O_NONBLOCK, FD_CLOEXEC)");
    }
}

} // end of namespace node_mdns
