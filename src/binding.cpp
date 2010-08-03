#include <v8.h>
#include <iostream>

#include "advertisement.hpp"
#include "browser.hpp"
#include "resolver.hpp"

extern "C" 
void
init (v8::Handle<v8::Object> target) {
    using namespace node_mdns;
    v8::HandleScope scope;

    Advertisement::Initialize( target );
    Browser::Initialize( target );
    Resolver::Initialize( target );

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
