#include "mdns_settings.hpp"

#include "winsock_watcher.hpp"

using namespace v8;

namespace node_mdns {

Persistent<FunctionTemplate> WinsockWatcher::constructor_template;

void
WinsockWatcher::Initialize(Handle<Object> target) {
    Local<FunctionTemplate> t = FunctionTemplate::New(New);
    constructor_template = Persistent<FunctionTemplate>::New(t);
    constructor_template->InstanceTemplate()->SetInternalFieldCount(1);

    Local<String> symbol = String::NewSymbol("WinsockWatcher");
    constructor_template->SetClassName(symbol);

    target->Set(symbol, constructor_template->GetFunction());
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
