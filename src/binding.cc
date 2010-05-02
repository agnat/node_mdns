#include <iostream>

#include "mdns_utils.hpp"
#include "advertisement.hpp"
#include "browser.hpp"

extern "C" 
void
init (Handle<Object> target) {
    using namespace node_mdns;
    HandleScope scope;
    Advertisement::Initialize( target );
    Browser::Initialize( target );
}
