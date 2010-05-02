#ifndef NODE_JS_MDNS_BASE_INCLUDED
#define NODE_JS_MDNS_BASE_INCLUDED

#include <node_events.h>
#include "mdns_utils.hpp"

namespace node_mdns {

class mDNSBase : public node::EventEmitter {
    protected:
        typedef v8::Handle<v8::Value> (NewFunc)(const v8::Arguments &);

        mDNSBase();
        virtual ~mDNSBase();

        static
        v8::Local<v8::FunctionTemplate>
        Initialize(v8::Handle<v8::Object> target, NewFunc f);

    protected:
        DNSServiceRef & ServiceRef() { return ref_; }
        void prepareSocket();
        void Event(int revents);

        bool Stop(v8::Local<v8::Value> exception = v8::Local<v8::Value>());
        static v8::Handle<v8::Value> Stop(const v8::Arguments & args);
    private:
        static void io_event(EV_P_ ev_io *w, int revents);

        DNSServiceRef ref_;
        ev_io         read_watcher_;

        static v8::Persistent<v8::String> stop_symbol;
};

}
#endif // NODE_JS_MDNS_BASE_INCLUDED
