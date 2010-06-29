#ifndef NODE_JS_MDNS_ADVERTISEMENT_INCLUDED
#define NODE_JS_MDNS_ADVERTISEMENT_INCLUDED

#include "mdns_base.hpp"
#include "mdns_utils.hpp"

namespace node_mdns {

class Advertisement : public mDNSBase {
    public:
        static void Initialize(v8::Handle<v8::Object> target);
        static v8::Handle<v8::Value> New(const v8::Arguments & args);

        v8::Handle<v8::Value> DoStart(DNSServiceFlags flags, uint32_t interface_index,
                const char * name, const char * regtype, const char * domain,
                const char * host, uint16_t port, uint16_t txt_record_length,
                const void * txt_record);

    protected:


        Advertisement();
        void on_service_registered(DNSServiceFlags flags,
                DNSServiceErrorType errorCode, const char * name,
                const char * regtype, const char * domain);
        static v8::Handle<v8::Value> DoStart(const v8::Arguments & args);
    private:
        static void on_service_registered(DNSServiceRef /*sdRef*/,
                DNSServiceFlags flags, DNSServiceErrorType errorCode,
                const char *name, const char *regtype, const char *domain,
                void *context );

        static v8::Persistent<v8::String> ready_symbol;
        static v8::Persistent<v8::String> name_symbol;
        static v8::Persistent<v8::String> regtype_symbol;
        static v8::Persistent<v8::String> domain_symbol;
};

} // end of namespace node_mdns

#endif // NODE_JS_MDNS_ADVERTISEMENT_INCLUDED
