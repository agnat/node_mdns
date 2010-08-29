#include <dns_sd.h>

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

void
OnEnumeration(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex,
        DNSServiceErrorType errorCode, const char * replyDomain, void * context)
{
}

Handle<Value>
dnsServiceEnumerateDomains(Arguments const& args) {
    HandleScope scope;
    if (argumentCountMismatch(args, 5)) {
        return throwArgumentCountMismatchException(args, 5);
    }
    
    if ( ! ServiceRef::HasInstance(args[0])) {
        return throwTypeError("argument 1 must be a DNSServiceRef (sdRef)");
    }
    ServiceRef * serviceRef = ObjectWrap::Unwrap<ServiceRef>(args[0]->ToObject());
    if (serviceRef->IsInitialized()) {
        return throwError("DNSServiceRef is already initialized");
    }

    if ( ! args[1]->IsInt32()) {
        return throwError("argument 2 must be an integer (DNSServiceFlags)");
    }
    DNSServiceFlags flags = args[1]->ToInteger()->Int32Value();

    if ( ! args[2]->IsInt32()) {
        return throwTypeError("argument 3 must be an integer (interfaceIndex)");
    }
    uint32_t interfaceIndex = args[2]->ToInteger()->Int32Value();

    if ( ! args[3]->IsFunction()) {
        return throwTypeError("argument 4 must be a function (callBack)");
    }
    serviceRef->SetCallback(Local<Function>::Cast(args[3]));

    if ( ! args[4]->IsNull() && ! args[4]->IsUndefined()) {
        serviceRef->SetContext(args[4]);
    }

    DNSServiceErrorType error = DNSServiceEnumerateDomains( & serviceRef->GetServiceRef(),
            flags, interfaceIndex, OnEnumeration, serviceRef);

    if (error != kDNSServiceErr_NoError) {
        return throwMdnsError("dnsServiceEnumerateDomains()", error);
    }
    if ( ! serviceRef->SetSocketFlags()) {
        return throwError("Failed to set socket flags (O_NONBLOCK, FD_CLOEXEC)");
    }
    return Undefined();
}

} // end of namespace node_mdns
