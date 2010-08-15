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
        Initialize(NewFunc f);

    protected:
        DNSServiceRef & ServiceRef() { return ref_; }
        v8::Handle<v8::Value> prepareSocket();
        void Event(int revents);

        bool Stop();
        static v8::Handle<v8::Value> Stop(const v8::Arguments & args);
    private:
        static void io_event(EV_P_ ev_io *w, int revents);

        DNSServiceRef ref_;
        ev_io         read_watcher_;

};

}
#endif // NODE_JS_MDNS_BASE_INCLUDED
