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

    NODE_DEFINE_CONSTANT(target, kDNSServiceFlagsAdd);
}
