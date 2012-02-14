#ifndef NODE_MDNS_INCLUDED
#define NODE_MDNS_INCLUDED

#include "mdns_settings.hpp"

#ifdef WIN32
# pragma warning( push )
# pragma warning( disable: 4251 )
#endif
#include <node.h>
#ifdef WIN32
# pragma warning( pop )
#endif

#include <dns_sd.h>

#endif // NODE_MDNS_INCLUDED
