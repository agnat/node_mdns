#ifndef NODE_JS_MDNS_ADVERTISEMENT_INCLUDED
#define NODE_JS_MDNS_ADVERTISEMENT_INCLUDED

#include "mdns_base.hpp"
#include "mdns_utils.hpp"

namespace node_mdns {

class AdContext;

class Advertisement : public mDNSBase {
    public:
        static void Initialize(v8::Handle<v8::Object> target);
        static v8::Handle<v8::Value> New(const v8::Arguments & args);

        v8::Handle<v8::Value> DoStart(DNSServiceFlags flags, uint32_t interface_index,
                const char * name, const char * regtype, const char * domain,
                const char * host, uint16_t port, v8::Handle<v8::Value> const& txt_record_object,
                AdContext * context);

        ~Advertisement();

    protected:


        Advertisement();
        void on_service_registered(DNSServiceFlags flags,
                DNSServiceErrorType errorCode, const char * name,
                const char * regtype, const char * domain, AdContext * context);
        static v8::Handle<v8::Value> DoStart(const v8::Arguments & args);
    private:
        enum { TXT_RECORD_BUFFER_SIZE = 256 };
        TXTRecordRef txt_record_;
        char         txt_record_buffer_[TXT_RECORD_BUFFER_SIZE];

        static void on_service_registered(DNSServiceRef /*sdRef*/,
                DNSServiceFlags flags, DNSServiceErrorType errorCode,
                const char *name, const char *regtype, const char *domain,
                void *context );

        static v8::Persistent<v8::String> name_symbol;
        static v8::Persistent<v8::String> regtype_symbol;
        static v8::Persistent<v8::String> domain_symbol;
};

} // end of namespace node_mdns

#endif // NODE_JS_MDNS_ADVERTISEMENT_INCLUDED
