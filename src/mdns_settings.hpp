#ifndef NODE_MDNS_SETTINGS_INCLUDED
# define NODE_MDNS_SETTINGS_INCLUDED

# ifdef WIN32
#  define _WINSOCKAPI_
#  include <windows.h>
#  include <winsock2.h>
# endif

// Undo Microsoft namespace pollution. A define called 'max'? Srsly?
#ifdef max
# undef max
#endif

#endif // NODE_MDNS_SETTINGS_INCLUDED
