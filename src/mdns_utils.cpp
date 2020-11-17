#include "mdns.hpp"

#include "mdns_utils.hpp"

#include <string>

namespace node_mdns {

using namespace v8;

Local<Value>
buildException(DNSServiceErrorType error_code) {
    if (error_code == kDNSServiceErr_NoError) {
        return Nan::Undefined();
    }

    std::string error_str("dns service error: ");
    error_str += errorString(error_code);
    Local<String> error_msg = Nan::New(error_str.c_str()).ToLocalChecked();
    Local<Value> error_v = Exception::Error(error_msg);
    Local<Object> error = ToObject(error_v);
    Nan::Set(error, Nan::New("errorCode").ToLocalChecked(), Nan::New<Integer>(error_code));
    return error_v;
}

const char *
errorString(DNSServiceErrorType error) {
    switch (error) {
        case kDNSServiceErr_NoError:
            return "no error";
        case kDNSServiceErr_Unknown:
            return "unknown";
        case kDNSServiceErr_NoSuchName:
            return "no such name";
        case kDNSServiceErr_NoMemory:
            return "no memory";
        case kDNSServiceErr_BadParam:
            return "bad param";
        case kDNSServiceErr_BadReference:
            return "bad reference";
        case kDNSServiceErr_BadState:
            return "bad state";
        case kDNSServiceErr_BadFlags:
            return "bad flags";
        case kDNSServiceErr_Unsupported:
            return "unsupported";
        case kDNSServiceErr_NotInitialized:
            return "not initialized";
        case kDNSServiceErr_AlreadyRegistered:
            return "already registered";
        case kDNSServiceErr_NameConflict:
            return "name conflict";
        case kDNSServiceErr_Invalid:
            return "invalid";
        case kDNSServiceErr_Firewall:
            return "firewall";
        case kDNSServiceErr_Incompatible:
            return "incompatible";
        case kDNSServiceErr_BadInterfaceIndex:
            return "bad interface index";
        case kDNSServiceErr_Refused:
            return "refused";
        case kDNSServiceErr_NoSuchRecord:
            return "no such record";
        case kDNSServiceErr_NoAuth:
            return "no auth";
        case kDNSServiceErr_NoSuchKey:
            return "no such key";
        case kDNSServiceErr_NATTraversal:
            return "NAT traversal";
        case kDNSServiceErr_DoubleNAT:
            return "double NAT";
        case kDNSServiceErr_BadTime:
            return "bad time";
#if defined(kDNSServiceErr_BadSig) || _DNS_SD_H+0 >= 1610100
        case kDNSServiceErr_BadSig:
            return "bad sig";
#endif
#if defined(kDNSServiceErr_BadKey) || _DNS_SD_H+0 >= 1610100
        case kDNSServiceErr_BadKey:
            return "bad key";
#endif
#if defined(kDNSServiceErr_Transient) || _DNS_SD_H+0 >= 1610100
        case kDNSServiceErr_Transient:
            return "transient";
#endif
#if defined(kDNSServiceErr_ServiceNotRunning) || _DNS_SD_H+0 >= 1610100
        case kDNSServiceErr_ServiceNotRunning:
            return "service not running";
#endif
#if defined(kDNSServiceErr_NATPortMappingUnsupported) || _DNS_SD_H+0 >= 1610100
        case kDNSServiceErr_NATPortMappingUnsupported:
            return "NAT port mapping unsupported";
#endif
#if defined(kDNSServiceErr_NATPortMappingDisabled) || _DNS_SD_H+0 >= 1610100
        case kDNSServiceErr_NATPortMappingDisabled:
            return "NAT port mapping disabled";
#endif
#if defined(kDNSServiceErr_NoRouter) || _DNS_SD_H+0 >= 1710400
        case kDNSServiceErr_NoRouter:
            return "no router";
#endif
#if defined(kDNSServiceErr_PollingMode) || _DNS_SD_H+0 >= 1710400
        case kDNSServiceErr_PollingMode:
            return "polling mode";
#endif
#if defined(kDNSServiceErr_Timeout) || _DNS_SD_H+0 >= 3200500
        case kDNSServiceErr_Timeout:
            return "timeout";
#endif
        default:
            return "unknown error code";
    }
}

} // end of namespace node_mdns
