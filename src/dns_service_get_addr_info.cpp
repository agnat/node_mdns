#include "mdns.hpp"

#ifndef WIN32 // XXX
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h> // AF_INET and AF_INET6 on freebsd
#endif

#include <v8.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

#ifdef HAVE_DNSSERVICEGETADDRINFO

void
DNSSD_API
OnAddressInfo(DNSServiceRef sdRef, DNSServiceFlags flags, 
        uint32_t interfaceIndex, DNSServiceErrorType errorCode,
        const char * hostname, const struct sockaddr * address,
        uint32_t ttl, void * context)
{
    if ( ! context) return;

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
    info[4] = stringOrUndefined(hostname);
    info[5] = Nan::New<String>().ToLocalChecked();
    char ip[INET6_ADDRSTRLEN];
    struct sockaddr_in *a4;
    struct sockaddr_in6 *a6;
    switch (address->sa_family) {
        case AF_INET6:
            a6 = (struct sockaddr_in6*)(address);
            inet_ntop(AF_INET6, &(a6->sin6_addr), ip, INET6_ADDRSTRLEN);
            info[5] = Nan::New(ip).ToLocalChecked();
            break;
        case AF_INET:
            a4 = (struct sockaddr_in*)(address);
            inet_ntop(AF_INET, &(a4->sin_addr), ip, INET6_ADDRSTRLEN);
            info[5] = Nan::New(ip).ToLocalChecked();
            break;
        default:
            break;
    }

    info[6] = Nan::New<Integer>(ttl);

    if (serviceRef->GetContext().IsEmpty()) {
        info[7] = Nan::Undefined();
    } else {
        info[7] = serviceRef->GetContext();
    }
    Nan::AsyncResource asyncResource(LOC(__FILE__, __LINE__));
    asyncResource.runInAsyncScope(this_, callback, argc, info);
}

NAN_METHOD(DNSServiceGetAddrInfo) {

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

    if ( ! info[3]->IsInt32()) {
        return throwTypeError("argument 4 must be an integer (DNSServiceProtocol)");
    }
    uint32_t protocol = ToInt32(info[3]);

    if ( ! info[4]->IsString()) {
        return throwTypeError("argument 5 must be a string (hostname)");
    }
    Nan::Utf8String hostname(ToString(info[4]));

    if ( ! info[5]->IsFunction()) {
        return throwTypeError("argument 6 must be a function (callBack)");
    }
    serviceRef->SetCallback(Local<Function>::Cast(info[5]));

    if ( ! info[6]->IsNull() && ! info[6]->IsUndefined()) {
        serviceRef->SetContext(info[6]);
    }

    DNSServiceErrorType error = DNSServiceGetAddrInfo( & serviceRef->GetServiceRef(),
            flags, interfaceIndex, protocol, *hostname, OnAddressInfo, serviceRef);

    if (error != kDNSServiceErr_NoError) {
        return throwMdnsError(error);
    }
    if ( ! serviceRef->SetSocketFlags()) {
        return throwError("Failed to set socket flags (O_NONBLOCK, FD_CLOEXEC)");
    }
}

#endif // HAVE_DNSSERVICEGETADDRINFO

} // end of namespace node_mdns
