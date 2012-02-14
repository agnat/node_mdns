#ifndef NODE_MDNS_WINSOCK_WATCHER_INCLUDED
#define NODE_MDNS_WINSOCK_WATCHER_INCLUDED

namespace node_mdns {

class WinsockWatcher : public node::ObjectWrap {
    public:
        static void Initialize(v8::Handle<v8::Object> target);

    private:
        static v8::Handle<v8::Value> New(const v8::Arguments & args);
        static v8::Handle<v8::Value> Set(const v8::Arguments & args);

        static v8::Persistent<v8::FunctionTemplate> constructor_template;
};

} // end of namespace node_mdns

#endif // NODE_MDNS_WINSOCK_WATCHER_INCLUDED
