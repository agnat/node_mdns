#ifndef NODE_MDNS_SERVICE_REF_INCLUDED
#define NODE_MDNS_SERVICE_REF_INCLUDED

#include <fcntl.h>

namespace node_mdns {

class ServiceRef : public Nan::ObjectWrap {
    public:
        ServiceRef();
        ~ServiceRef();

        static void Initialize(v8::Local<v8::Object> target);
        static NAN_METHOD(New);

        bool IsInitialized() const;

        static bool HasInstance(v8::Local<v8::Value> value);

        void SetCallback(v8::Local<v8::Function> callback);
        v8::Local<v8::Function> GetCallback() const;


        DNSServiceRef & GetServiceRef();
        void SetServiceRef(DNSServiceRef ref);

        v8::Local<v8::Value> GetContext();
        void SetContext(v8::Local<v8::Value> context);

        v8::Local<v8::Object> GetThis();
        void SetThis(v8::Local<v8::Object> This);

        bool SetSocketFlags();

    private:
        static NAN_PROPERTY_GETTER(fd_getter);
        static NAN_PROPERTY_GETTER(initialized_getter);

        DNSServiceRef ref_;
        Nan::Persistent<v8::Function> callback_;
        v8::Local<v8::Object>        this_;
        Nan::Persistent<v8::Value>    context_;

        static Nan::Persistent<v8::FunctionTemplate> constructor_template;
};

} // end of namespace node_mdns
#endif // NODE_MDNS_SERVICE_REF_INCLUDED
