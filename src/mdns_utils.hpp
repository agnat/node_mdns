#ifndef NODE_JS_MDNS_UTILS_INCLUDED
#define NODE_JS_MDNS_UTILS_INCLUDED

#include <dns_sd.h>
#include <errno.h>
#include <fcntl.h>
#include <node.h>
#include <node_events.h>

namespace node_mdns {

const char * errorString(DNSServiceErrorType error);
v8::Local<v8::Value> buildException(DNSServiceErrorType error_code);

} // end of namespace node_mdns

#endif // NODE_JS_MDNS_UTILS_INCLUDED
