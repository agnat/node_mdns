#include "mdns_utils.hpp"

#include <string>

namespace node_mdns {

using namespace v8;

inline
const char *
errorString(DNSServiceErrorType error) {
    switch (error) {
        case kDNSServiceErr_NoError:
            return "NoError";
        case kDNSServiceErr_Unknown:
            return "Unknown";
        case kDNSServiceErr_NoSuchName:
            return "NoSuchName";
        case kDNSServiceErr_NoMemory:
            return "NoMemory";
        case kDNSServiceErr_BadParam:
            return "BadParam";
        case kDNSServiceErr_BadReference:
            return "BadReference";
        case kDNSServiceErr_BadState:
            return "BadState";
        case kDNSServiceErr_BadFlags:
            return "BadFlags";
        case kDNSServiceErr_Unsupported:
            return "Unsupported";
        case kDNSServiceErr_NotInitialized:
            return "NotInitialized";
        case kDNSServiceErr_AlreadyRegistered:
            return "AlreadyRegistered";
        case kDNSServiceErr_NameConflict:
            return "NameConflict";
        case kDNSServiceErr_Invalid:
            return "Invalid";
        case kDNSServiceErr_Firewall:
            return "Firewall";
        case kDNSServiceErr_Incompatible:
            return "Incompatible";
        case kDNSServiceErr_BadInterfaceIndex:
            return "BadInterfaceIndex";
        case kDNSServiceErr_Refused:
            return "Refused";
        case kDNSServiceErr_NoSuchRecord:
            return "NoSuchRecord";
        case kDNSServiceErr_NoAuth:
            return "NoAuth";
        case kDNSServiceErr_NoSuchKey:
            return "NoSuchKey";
        case kDNSServiceErr_NATTraversal:
            return "NATTraversal";
        case kDNSServiceErr_DoubleNAT:
            return "DoubleNAT";
        case kDNSServiceErr_BadTime:
            return "BadTime";
#if 0 // missing on linux ...
        case kDNSServiceErr_BadSig:
            return "BadSig";
        case kDNSServiceErr_BadKey:
            return "BadKey";
        case kDNSServiceErr_Transient:
            return "Transient";
        case kDNSServiceErr_ServiceNotRunning:
            return "ServiceNotRunning";
        case kDNSServiceErr_NATPortMappingUnsupported:
            return "NATPortMappingUnsupported";
        case kDNSServiceErr_NATPortMappingDisabled:
            return "NATPortMappingDisabled";
#if 0 // missing on leopard
        case kDNSServiceErr_NoRouter:
            return "NoRouter";
        case kDNSServiceErr_PollingMode:
            return "PollingMode";
#endif
#endif
    };
    return "Unknown DNSServiceError";
}

Local<Value>
buildException(DNSServiceErrorType error_code) {
    HandleScope scope;

    std::string error_str("DNSServiceError: ");
    error_str += errorString(error_code);
    Local<String> error_msg = String::New(error_str.c_str());
    Local<Value> error_v = Exception::Error(error_msg);
    Local<Object> error = Local<Object>::Cast(error_v);
    return scope.Close(error);
}

}
