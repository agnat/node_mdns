#include "mdns.hpp"

// poor mans conditional compilation. is there a better way to do this with gyp?
#ifdef NODE_MDNS_USE_SOCKET_WATCHER

#include "socket_watcher.hpp"

#include <string.h> // needed for memset() with node v0.7.9 on Mac OS
#include <node.h>
#include <node_version.h>

using namespace v8;

#if ! NODE_VERSION_AT_LEAST(0, 7, 8)
namespace node {

Handle<Value>
MakeCallback(const Handle<Object> object, const Handle<Function> callback,
        int argc, Handle<Value> argv[])
{
    NanScope();

    // TODO Hook for long stack traces to be made here.

    TryCatch try_catch;

    Local<Value> ret = callback->Call(object, argc, argv);

    if (try_catch.HasCaught()) {
        FatalException(try_catch);
        NanReturnUndefined();
    }

    NanReturnValue(ret);
}

}  // end of namespace node
#endif

namespace node_mdns {

    // Persistent<String> callback_symbol;

    SocketWatcher::SocketWatcher() : poll_(NULL), fd_(0), events_(0) {
    }

    void
    SocketWatcher::Initialize(Handle<Object> target) {
        Local<FunctionTemplate> t = FunctionTemplate::New(New);

        Local<String> symbol = NanSymbol("SocketWatcher");
        t->SetClassName(symbol);
        t->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(t, "set", SocketWatcher::Set);
        NODE_SET_PROTOTYPE_METHOD(t, "start", SocketWatcher::Start);
        NODE_SET_PROTOTYPE_METHOD(t, "stop", SocketWatcher::Stop);

        target->Set(symbol, t->GetFunction());

        // NanAssignPersistent(String, callback_symbol, NanSymbol("callback"));
    }

    NAN_METHOD(SocketWatcher::Start) {
        NanScope();
        SocketWatcher *watcher = ObjectWrap::Unwrap<SocketWatcher>(args.Holder());
        watcher->Start();
        NanReturnUndefined();
    }

    void
    SocketWatcher::Start() {
        if (poll_ == NULL) {
            poll_ = new uv_poll_t;
            memset(poll_,0,sizeof(uv_poll_t));
            poll_->data = this;
            uv_poll_init_socket(uv_default_loop(), poll_, fd_);

            Ref();
        }

        if (!uv_is_active((uv_handle_t*)poll_)) {
            uv_poll_start(poll_, events_, &SocketWatcher::Callback);
        }
    }

    void
    SocketWatcher::Callback(uv_poll_t *w, int status, int revents) {
        NanScope();

        SocketWatcher *watcher = static_cast<SocketWatcher*>(w->data);
        assert(w == watcher->poll_);

        Local<Value> callback_v = NanObjectWrapHandle(watcher)->Get(NanSymbol("callback"));
        if (!callback_v->IsFunction()) {
            watcher->Stop();
            return;
        }

        Local<Function> callback = Local<Function>::Cast(callback_v);

        Local<Value> argv[2];
        argv[0] = NanNewLocal<Value>(revents & UV_READABLE ? True() : False());
        argv[1] = NanNewLocal<Value>(revents & UV_WRITABLE ? True() : False());

        node::MakeCallback(NanObjectWrapHandle(watcher), callback, 2, argv);
    }

    NAN_METHOD(SocketWatcher::Stop) {
        NanScope();
        SocketWatcher *watcher = ObjectWrap::Unwrap<SocketWatcher>(args.Holder());
        watcher->Stop();
        NanReturnUndefined();
    }

    void
    SocketWatcher::Stop() {
        if (poll_ != NULL) {
            uv_poll_stop(poll_);
            Unref();
        }
    }

    NAN_METHOD(SocketWatcher::New) {
        NanScope();
        SocketWatcher *s = new SocketWatcher();
        s->Wrap(args.This());
        NanReturnValue(args.This());
    }

    NAN_METHOD(SocketWatcher::Set) {
        NanScope();
        SocketWatcher *watcher = ObjectWrap::Unwrap<SocketWatcher>(args.Holder());
        if (!args[0]->IsInt32()) {
            NanReturnValue(ThrowException(Exception::TypeError(
                String::New("First arg should be a file descriptor."))));
        }
        int fd = args[0]->Int32Value();
        if (!args[1]->IsBoolean()) {
            NanReturnValue(ThrowException(Exception::TypeError(
                String::New("Second arg should boolean (readable)."))));
        }
        int events = 0;

        if (args[1]->IsTrue()) events |= UV_READABLE;

        if (!args[2]->IsBoolean()) {
            NanReturnValue(ThrowException(Exception::TypeError(
                String::New("Third arg should boolean (writable)."))));
        }

        if (args[2]->IsTrue()) events |= UV_WRITABLE;

        assert(watcher->poll_ == NULL);

        watcher->fd_ = fd;
        watcher->events_ = events;

        NanReturnUndefined();
    }

} // end of namespace node_mdns

#endif // NODE_MDNS_USE_SOCKET_WATCHER
