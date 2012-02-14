#ifndef NODE_MDNS_SETTINGS_INCLUDED
# define NODE_MDNS_SETTINGS_INCLUDED

# ifdef WIN32
#  define _WINSOCKAPI_
#  include <windows.h>

   // Microsoft namespace pollution. A macro called 'max'? Srsly?
#  ifdef max
#   undef max
#  endif

#  define NODE_MDNS_WINSOCK_WATCHER

# endif // WIN32


#endif // NODE_MDNS_SETTINGS_INCLUDED
