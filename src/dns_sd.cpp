#include "mdns.hpp"

#include <v8.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"
#include "txt_record_ref.hpp"
#ifdef NODE_MDNS_USE_SOCKET_WATCHER
# include "socket_watcher.hpp"
#endif

using namespace v8;
using namespace node;

namespace node_mdns {

// === dns_sd ===========================================
NAN_METHOD(DNSServiceRegister);
NAN_METHOD(DNSServiceUpdateRecord);
NAN_METHOD(DNSServiceRefSockFD);
NAN_METHOD(DNSServiceProcessResult);
NAN_METHOD(DNSServiceBrowse);
NAN_METHOD(DNSServiceRefDeallocate);
NAN_METHOD(DNSServiceResolve);
NAN_METHOD(DNSServiceEnumerateDomains);
#ifdef HAVE_DNSSERVICEGETADDRINFO
NAN_METHOD(DNSServiceGetAddrInfo); 
#endif
NAN_METHOD(TXTRecordCreate); 
NAN_METHOD(TXTRecordDeallocate);
//NAN_METHOD(TXTRecordGetCount); 
NAN_METHOD(TXTRecordSetValue); 
NAN_METHOD(TXTRecordGetLength);

// === posix ============================================
#ifdef NODE_MDNS_HAVE_INTERFACE_NAME_CONVERSION
NAN_METHOD(if_nametoindex); 
NAN_METHOD(if_indextoname);
#endif

// === additions ========================================
NAN_METHOD(txtRecordBufferToObject);
NAN_METHOD(exportConstants);
NAN_METHOD(buildException);

// === locals ===========================================

void defineFunction(Local<Object> target, const char * name, Nan::FunctionCallback f);
void addConstants(Local<Object> target);

void
init(Local<Object> target) {
    Nan::HandleScope scope;

    ServiceRef::Initialize( target );
    TxtRecordRef::Initialize( target );
#ifdef NODE_MDNS_USE_SOCKET_WATCHER
    SocketWatcher::Initialize( target );
#endif

    defineFunction(target, "DNSServiceRegister", DNSServiceRegister);
    defineFunction(target, "DNSServiceUpdateRecord", DNSServiceUpdateRecord);
    defineFunction(target, "DNSServiceRefSockFD", DNSServiceRefSockFD);
    defineFunction(target, "DNSServiceProcessResult", DNSServiceProcessResult);
    defineFunction(target, "DNSServiceBrowse", DNSServiceBrowse);
    defineFunction(target, "DNSServiceRefDeallocate", DNSServiceRefDeallocate);
    defineFunction(target, "DNSServiceResolve", DNSServiceResolve);
    defineFunction(target, "DNSServiceEnumerateDomains",
            DNSServiceEnumerateDomains);
#ifdef HAVE_DNSSERVICEGETADDRINFO
    defineFunction(target, "DNSServiceGetAddrInfo", DNSServiceGetAddrInfo);
#endif
    defineFunction(target, "TXTRecordCreate", TXTRecordCreate);
    defineFunction(target, "TXTRecordDeallocate", TXTRecordDeallocate);
    //defineFunction(target, "TXTRecordGetCount", TXTRecordGetCount);
    defineFunction(target, "TXTRecordSetValue", TXTRecordSetValue);
    defineFunction(target, "TXTRecordGetLength", TXTRecordGetLength);

#ifdef NODE_MDNS_HAVE_INTERFACE_NAME_CONVERSION
    defineFunction(target, "if_nametoindex", if_nametoindex);
    defineFunction(target, "if_indextoname", if_indextoname);
#endif

    defineFunction(target, "txtRecordBufferToObject", txtRecordBufferToObject);
    defineFunction(target, "buildException", buildException);
    defineFunction(target, "exportConstants", exportConstants);

    addConstants(target);
}

inline
void
defineFunction(Local<Object> target, const char * name, Nan::FunctionCallback f) {
    Nan::Set(target, Nan::New(name).ToLocalChecked(),
            Nan::GetFunction(Nan::New<FunctionTemplate>(f)).ToLocalChecked());
}

NAN_METHOD(buildException) {
    if (argumentCountMismatch(info, 1)) {
        return throwArgumentCountMismatchException(info, 1);
    }
    if ( ! info[0]->IsInt32()) {
        return throwTypeError("argument 1 must be an integer " 
                "(DNSServiceErrorType)");
    }

    DNSServiceErrorType error = Nan::To<int32_t>(info[0]).FromJust();
    info.GetReturnValue().Set<v8::Value>(buildException(error));
}

void
addConstants(Local<Object> target) {
    // DNS Classes
    NODE_DEFINE_CONSTANT(target, kDNSServiceClass_IN);

    // DNS Error Codes
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NoError);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_Unknown);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NoSuchName);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NoMemory);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_BadParam);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_BadReference);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_BadState);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_BadFlags);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_Unsupported);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NotInitialized);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_AlreadyRegistered);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NameConflict);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_Invalid);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_Firewall);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_Incompatible);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_BadInterfaceIndex);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_Refused);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NoSuchRecord);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NoAuth);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NoSuchKey);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NATTraversal);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_DoubleNAT);
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_BadTime);
#ifdef kDNSServiceErr_BadSig
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_BadSig);
#endif
#ifdef kDNSServiceErr_BadKey
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_BadKey);
#endif
#ifdef kDNSServiceErr_Transient
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_Transient);
#endif
#ifdef kDNSServiceErr_ServiceNotRunning
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_ServiceNotRunning);
#endif
#ifdef kDNSServiceErr_NATPortMappingUnsupported
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NATPortMappingUnsupported);
#endif
#ifdef kDNSServiceErr_NATPortMappingDisabled
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NATPortMappingDisabled);
#endif
#ifdef kDNSServiceErr_NoRouter
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_NoRouter);
#endif
#ifdef kDNSServiceErr_PollingMode
    NODE_DEFINE_CONSTANT(target, kDNSServiceErr_PollingMode);
#endif

    // Interface Index
#ifdef kDNSServiceInterfaceIndexAny
    NODE_DEFINE_CONSTANT(target, kDNSServiceInterfaceIndexAny);
#endif
#ifdef kDNSServiceInterfaceIndexLocalOnly
    NODE_DEFINE_CONSTANT(target, kDNSServiceInterfaceIndexLocalOnly);
#endif
#ifdef kDNSServiceInterfaceIndexP2P
    NODE_DEFINE_CONSTANT(target, kDNSServiceInterfaceIndexP2P);
#endif
#ifdef kDNSServiceInterfaceIndexUnicast
    NODE_DEFINE_CONSTANT(target, kDNSServiceInterfaceIndexUnicast);
#endif

    // DNS Service Types
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_A);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_NS);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_MD);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_MF);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_CNAME);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_SOA);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_MB);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_MG);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_MR);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_NULL);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_WKS);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_PTR);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_HINFO);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_MINFO);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_MX);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_TXT);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_RP);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_AFSDB);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_X25);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_ISDN);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_RT);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_NSAP);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_NSAP_PTR);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_SIG);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_KEY);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_PX);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_GPOS);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_AAAA);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_LOC);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_NXT);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_EID);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_NIMLOC);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_SRV);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_ATMA);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_NAPTR);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_KX);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_CERT);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_A6);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_DNAME);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_SINK);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_OPT);
#ifdef kDNSServiceType_APL
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_APL);
#endif
#ifdef kDNSServiceType_DS
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_DS);
#endif
#ifdef kDNSServiceType_SSHFP
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_SSHFP);
#endif
#ifdef kDNSServiceType_IPSECKEY
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_IPSECKEY);
#endif
#ifdef kDNSServiceType_RRSIG
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_RRSIG);
#endif
#ifdef kDNSServiceType_NSEC
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_NSEC);
#endif
#ifdef kDNSServiceType_DNSKEY
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_DNSKEY);
#endif
#ifdef kDNSServiceType_DHCID
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_DHCID);
#endif
#ifdef kDNSServiceType_NSEC3
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_NSEC3);
#endif
#ifdef kDNSServiceType_NSEC3PARAM
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_NSEC3PARAM);
#endif
#ifdef kDNSServiceType_HIP
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_HIP);
#endif
#ifdef kDNSServiceType_SPF
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_SPF);
#endif
#ifdef kDNSServiceType_UINFO
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_UINFO);
#endif
#ifdef kDNSServiceType_UID
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_UID);
#endif
#ifdef kDNSServiceType_GID
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_GID);
#endif
#ifdef kDNSServiceType_UNSPEC
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_UNSPEC);
#endif
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_TKEY);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_TSIG);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_IXFR);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_AXFR);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_MAILB);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_MAILA);
    NODE_DEFINE_CONSTANT(target, kDNSServiceType_ANY);

    // General Flags
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsMoreComing);
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsAdd);
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsDefault);
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsNoAutoRename);
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsShared);
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsUnique);
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsBrowseDomains);
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsRegistrationDomains);
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsLongLivedQuery);
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsAllowRemoteQuery);
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsForceMulticast);
#ifdef kDNSServiceFlagsForce
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsForce);
#endif
#ifdef kDNSServiceFlagsReturnIntermediates
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsReturnIntermediates);
#endif
#ifdef kDNSServiceFlagsNonBrowsable
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsNonBrowsable);
#endif
#ifdef kDNSServiceFlagsShareConnection
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsShareConnection);
#endif
#ifdef kDNSServiceFlagsSuppressUnusable
    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsSuppressUnusable);
#endif
}

NAN_METHOD(exportConstants) {
    if (argumentCountMismatch(info, 1)) {
        return throwArgumentCountMismatchException(info, 1);
    }
    if ( ! info[0]->IsObject()) {
        return throwTypeError("argument 1 must be an object.");
    }

    addConstants(info[0]->ToObject());
}

} // end of namespace node_mdns

NODE_MODULE(dns_sd_bindings,node_mdns::init);
