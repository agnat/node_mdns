#include "mdns_base.hpp"
#include <node.h>

namespace node_mdns {

using namespace v8;

v8::Local<v8::FunctionTemplate>
mDNSBase::Initialize(NewFunc f) {
    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(f);

    t->Inherit(EventEmitter::constructor_template);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "stop", Stop);

    return t;
}

mDNSBase::mDNSBase() : ref_(NULL) {
    ev_init(&read_watcher_, io_event);
    read_watcher_.data = this;
}

mDNSBase::~mDNSBase() {
    assert(NULL == ref_);
}

Handle<Value>
mDNSBase::prepareSocket() {
    int fd = DNSServiceRefSockFD(ref_);
    if (-1 == fd) {
        return Exception::Error(String::New("Failed to get socket fd"));
    }

    if (-1 == fcntl(fd, F_SETFL, O_NONBLOCK) ||
        -1 == fcntl(fd, F_SETFD, FD_CLOEXEC))
    {
        return node::ErrnoException(errno, "fcntl");
    }

    ev_io_set(&read_watcher_, fd, EV_READ);
    ev_io_start(EV_DEFAULT_ &read_watcher_);
    return Undefined();
}

bool
mDNSBase::Stop() {
    HandleScope scope;
    ev_io_stop(EV_DEFAULT_ &read_watcher_);
    DNSServiceRefDeallocate(ref_);
    ref_ = NULL;
    Unref();
}


Handle<Value>
mDNSBase::Stop(const Arguments & args) {
    HandleScope scope;
    mDNSBase * o = ObjectWrap::Unwrap<mDNSBase>(args.This());

    bool r = o->Stop();

    if ( ! r) {
        // XXX
        /*
        return ThrowException(Exception::Error(
            String::New(ad->ErrorMessage())));
        */
    }
    return Undefined();
}

void
mDNSBase::Event(int revents) {
    if (EV_READ & revents) {
        if (kDNSServiceErr_NoError != DNSServiceProcessResult(ref_)) {
            // XXX handle error
        }
    }
}

void
mDNSBase::io_event(EV_P_ ev_io *w, int revents) {
    mDNSBase * sink = static_cast<mDNSBase*>(w->data);
    sink->Event(revents);
}

} // end of namespace node_mdns
