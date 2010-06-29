#ifndef NODE_JS_MDNS_SERVICE_REF_INCLUDED
#define NODE_JS_MDNS_SERVICE_REF_INCLUDED

#include <v8.h>
#include <node.h>
#include <dns_sd.h>

namespace node_mdns {

class ServiceRef : public node::ObjectWrap {
    public:
        static void Initialize(v8::Handle<v8::Object> target);
        static v8::Handle<v8::Value> New(const v8::Arguments & args);
    private:
        static v8::Handle<v8::Value> fd_getter(v8::Local<v8::String> property,
                v8::AccessorInfo const& info);

        DNSServiceRef ref_;
        static v8::Persistent<v8::FunctionTemplate> constructor_template;
};

} // end of namespace node_mdns
#endif // NODE_JS_MDNS_SERVICE_REF_INCLUDED
