#include "mdns.hpp"

#include "winsock_watcher.hpp"

using namespace v8;

namespace node_mdns {

void
WinsockWatcher::Initialize(Handle<Object> target) {
    Local<FunctionTemplate> t = FunctionTemplate::New(New);
    Local<String> symbol = String::NewSymbol("WinsockWatcher");
    t->SetClassName(symbol);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "set", Set);

    target->Set(symbol, t->GetFunction());
}

v8::Handle<v8::Value>
WinsockWatcher::New(const v8::Arguments & args) {
    HandleScope scope;
    WinsockWatcher * o = new WinsockWatcher();
    o->Wrap(args.Holder());
    return args.This();
}

v8::Handle<v8::Value>
WinsockWatcher::Set(const v8::Arguments & args) {
    HandleScope scope;
    return Undefined();
}

} // end of namespace node_mdns
