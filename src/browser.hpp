#ifndef NODE_JS_MDNS_BROWSER_INCLUDED
#define NODE_JS_MDNS_BROWSER_INCLUDED

#include "mdns_base.hpp"
#include "mdns_utils.hpp"

namespace node_mdns {

class Browser : public mDNSBase {
    public:
        static void Initialize(v8::Handle<v8::Object> target);
        bool DoStart(DNSServiceFlags flags, uint32_t interface_index,
                const char * regtype, const char * domain);

        static v8::Handle<v8::Value> New(const v8::Arguments & args);

    protected:
        Browser();

        static v8::Handle<v8::Value> DoStart(const v8::Arguments & args);
        static void on_service_changed(DNSServiceRef /*sdRef*/, DNSServiceFlags flags,
                uint32_t interface_index, DNSServiceErrorType errorCode,
                const char * serviceName, const char * regtype,
                const char * replyDomain, void * context);
        void on_service_changed(DNSServiceFlags flags, uint32_t interface_index,
                DNSServiceErrorType errorCode, const char * name,
                const char * regtype, const char * domain);

        static v8::Persistent<v8::String> changed_symbol;
        static v8::Persistent<v8::String> name_symbol;
        static v8::Persistent<v8::String> interface_symbol;
        static v8::Persistent<v8::String> regtype_symbol;
        static v8::Persistent<v8::String> domain_symbol;
};

} // end of namespace node_mdns

#endif // NODE_JS_MDNS_BROWSER_INCLUDED
