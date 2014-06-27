#ifndef NODE_MDNS_INCLUDED
#define NODE_MDNS_INCLUDED

#include "mdns_settings.hpp"

#ifdef WIN32
# pragma warning( push )
# pragma warning( disable: 4251 )
#endif

// HACK: fix "node.h missing ';' before identifier" build error
#include "v8.h" // NOLINT(build/include_order)
#ifdef _WIN32
// TODO(tjfontaine) consider changing the usage of ssize_t to ptrdiff_t
#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
typedef intptr_t ssize_t;
# define _SSIZE_T_
# define _SSIZE_T_DEFINED
#endif
#else // !_WIN32
# include <sys/types.h> // size_t, ssize_t
#endif // _WIN32

#include <node.h>
#include "nan.h"

#ifdef WIN32
# pragma warning( pop )
#endif

#ifndef NODE_VERSION_AT_LEAST
# include <node_version.h>
#endif

// XXX It would be better to test UV_VERSION_MAJOR and UV_VERSION_MINOR.
//     However, libuv didn't bump the version when uv_poll_t was introduced.
#if NODE_VERSION_AT_LEAST(0, 7, 9)
# define NODE_MDNS_USE_SOCKET_WATCHER
//# warning Using SocketWatcher
#else
//# warning Using IOWatcher
#endif


#include <dns_sd.h>

#endif // NODE_MDNS_INCLUDED
