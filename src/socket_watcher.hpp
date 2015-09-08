#ifndef NODE_MDNS_SOCKET_WATCHER_INCLUDED
#define NODE_MDNS_SOCKET_WATCHER_INCLUDED

namespace node_mdns {

class SocketWatcher : public Nan::ObjectWrap {
    public:
        SocketWatcher();

        static void Initialize(v8::Handle<v8::Object> target);

    private:
        uv_poll_t* poll_;
        int fd_;
        int events_;

        static NAN_METHOD(New);
        static NAN_METHOD(Set);
        static NAN_METHOD(Start);
        static NAN_METHOD(Stop);
        
        void Start();
        void Stop();
        static void Callback(uv_poll_t *w, int status, int events);
};

} // end of namespace node_mdns

#endif // NODE_MDNS_SOCKET_WATCHER_INCLUDED
