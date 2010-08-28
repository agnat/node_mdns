#ifndef NODE_JS_MDNS_SERVICE_REF_INCLUDED
#define NODE_JS_MDNS_SERVICE_REF_INCLUDED

#include <fcntl.h>
#include <dns_sd.h>

#include <v8.h>
#include <node.h>

#include <iostream>

namespace node_mdns {

class ServiceRef : public node::ObjectWrap {
    public:
        ServiceRef();
        ~ServiceRef();

        static void Initialize(v8::Handle<v8::Object> target);
        static v8::Handle<v8::Value> New(const v8::Arguments & args);

        inline bool IsInitialized() const { return ref_ != NULL; }

        static inline bool HasInstance(v8::Handle<v8::Value> value) {
            if ( ! value->IsObject() ) return false;
            v8::Local<v8::Object> object = value->ToObject();
            return constructor_template->HasInstance( object );
        }

        inline void SetCallback(v8::Handle<v8::Function> callback) {
            if ( ! callback_.IsEmpty()) {
                callback_.Dispose();
            }
            callback_ = v8::Persistent<v8::Function>::New(callback);
        }
        inline v8::Handle<v8::Function> GetCallback() const { return callback_; }


        DNSServiceRef & GetServiceRef() { return ref_; }
        void SetServiceRef(DNSServiceRef ref) { ref_ = ref; }

        inline v8::Handle<v8::Value> GetContext() { return context_; }
        inline void SetContext(v8::Handle<v8::Value> context) {
            if ( ! context_.IsEmpty()) {
                context_.Dispose();
            }
            context_ = v8::Persistent<v8::Value>::New(context);
        }

        inline v8::Handle<v8::Object> GetThis() { return this_; }
        inline void SetThis(v8::Local<v8::Object> This) { this_ = This; }

        inline
        bool
        SetSocketFlags() {
            int fd = DNSServiceRefSockFD(ref_);
            if (fd == -1) return false;
            return fcntl(fd, F_SETFD, FD_CLOEXEC) != -1 &&
                fcntl(fd, F_SETFL, O_NONBLOCK) != -1;
        }


    private:
        static v8::Handle<v8::Value> fd_getter(v8::Local<v8::String> property,
                v8::AccessorInfo const& info);
        static v8::Handle<v8::Value> initialized_getter(v8::Local<v8::String> property,
                v8::AccessorInfo const& info);

        DNSServiceRef ref_;
        v8::Persistent<v8::Function> callback_;
        v8::Local<v8::Object>        this_;
        v8::Persistent<v8::Value>    context_;

        static v8::Persistent<v8::FunctionTemplate> constructor_template;
};

} // end of namespace node_mdns
#endif // NODE_JS_MDNS_SERVICE_REF_INCLUDED
