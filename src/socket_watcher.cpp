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
    Nan::HandleScope scope;

    // TODO Hook for long stack traces to be made here.

    NanTryCatch try_catch;
    
    Local<Value> ret = Nan::MakeCallback(object, callback, argc, info);

    if (try_catch.HasCaught()) {
        FatalException(try_catch);
        return;
    }

    info.GetReturnValue().Set(ret);
}

}  // end of namespace node
#endif

namespace node_mdns {

    SocketWatcher::SocketWatcher() : poll_(NULL), fd_(0), events_(0) {
    }

    void
    SocketWatcher::Initialize(Handle<Object> target) {
        Local<FunctionTemplate> t = Nan::New<FunctionTemplate>(New);

        Local<String> symbol = Nan::New("SocketWatcher").ToLocalChecked();
        t->SetClassName(symbol);
        t->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetPrototypeMethod(t, "set", SocketWatcher::Set);
        Nan::SetPrototypeMethod(t, "start", SocketWatcher::Start);
        Nan::SetPrototypeMethod(t, "stop", SocketWatcher::Stop);

        Nan::Set(target, symbol, Nan::GetFunction(t).ToLocalChecked());
    }

    NAN_METHOD(SocketWatcher::Start) {
        SocketWatcher *watcher = Nan::ObjectWrap::Unwrap<SocketWatcher>(info.Holder());
        watcher->Start();
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
        Nan::HandleScope scope;

        SocketWatcher *watcher = static_cast<SocketWatcher*>(w->data);
        assert(w == watcher->poll_);

        Local<Value> callback_v = Nan::Get(watcher->handle(), Nan::New("callback").ToLocalChecked()).ToLocalChecked();
        if (!callback_v->IsFunction()) {
            watcher->Stop();
            return;
        }

        Local<Function> callback = Local<Function>::Cast(callback_v);

        Local<Value> argv[2];
        argv[0] = revents & UV_READABLE ? Nan::True() : Nan::False();
        argv[1] = revents & UV_WRITABLE ? Nan::True() : Nan::False();

        Nan::MakeCallback(watcher->handle(), callback, 2, argv);
    }

    NAN_METHOD(SocketWatcher::Stop) {
        SocketWatcher *watcher = Nan::ObjectWrap::Unwrap<SocketWatcher>(info.Holder());
        watcher->Stop();
    }

    void
    SocketWatcher::Stop() {
        if (poll_ != NULL) {
            uv_poll_stop(poll_);
            Unref();
        }
    }

    NAN_METHOD(SocketWatcher::New) {
        SocketWatcher *s = new SocketWatcher();
        s->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }

    NAN_METHOD(SocketWatcher::Set) {
        SocketWatcher *watcher = Nan::ObjectWrap::Unwrap<SocketWatcher>(info.Holder());
        if (!info[0]->IsInt32()) {
            return Nan::ThrowTypeError("First arg should be a file descriptor.");
        }
        int fd = Nan::To<int32_t>(info[0]).FromJust();
        if (!info[1]->IsBoolean()) {
            return Nan::ThrowTypeError("Second arg should be a boolean (readable).");
        }
        int events = 0;

        if (info[1]->IsTrue()) events |= UV_READABLE;

        if (!info[2]->IsBoolean()) {
            return Nan::ThrowTypeError("Third arg should be a boolean (writable).");
        }

        if (info[2]->IsTrue()) events |= UV_WRITABLE;

        assert(watcher->poll_ == NULL);

        watcher->fd_ = fd;
        watcher->events_ = events;
    }

} // end of namespace node_mdns

#endif // NODE_MDNS_USE_SOCKET_WATCHER
