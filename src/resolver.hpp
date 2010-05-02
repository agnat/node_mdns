#ifndef NODE_JS_MDNS_RESOLVER_INCLUDED
#define NODE_JS_MDNS_RESOLVER_INCLUDED

#include "mdns_base.hpp"

namespace node_mdns {

class Resolver : public mDNSBase {
    public:
        static void Initialize(v8::Handle<v8::Object> target);
        bool DoStart(DNSServiceFlags flags, uint32_t interface_index,
                const char * name, const char * regtype, const char * domain);

        static v8::Handle<v8::Value> New(v8::Arguments const& args);

    protected:
        Resolver();

        static v8::Handle<v8::Value> DoStart(v8::Arguments const& args);

        static void on_resolve(DNSServiceRef /*sdRef*/, DNSServiceFlags,
                uint32_t interface_index, DNSServiceErrorType error_code,
                const char * fullname, const char * hosttarget, uint16_t port,
                uint16_t txt_record_length, const unsigned char * txt_record,
                void * context);
        void on_resolve(DNSServiceFlags flags, uint32_t interface_index,
                DNSServiceErrorType error_code, const char * fullname,
                const char * hosttarget, uint16_t port,
                uint16_t txt_record_length, const unsigned char * txt_record);

        static v8::Persistent<v8::String> resolved_symbol;
        static v8::Persistent<v8::String> interface_symbol;
        static v8::Persistent<v8::String> fullname_symbol;
        static v8::Persistent<v8::String> hosttarget_symbol;
        static v8::Persistent<v8::String> port_symbol;
};

} // end of namespace node_mdns

#endif // NODE_JS_MDNS_RESOLVER_INCLUDED
