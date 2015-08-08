#include "mdns.hpp"

#include "dns_service_ref.hpp"
#include "mdns_utils.hpp"

using namespace v8;

namespace node_mdns {

Nan::Persistent<FunctionTemplate> ServiceRef::constructor_template;

ServiceRef::ServiceRef() : ref_(), callback_(), context_() {}

ServiceRef::~ServiceRef() {
    // First, dispose the serice ref. This cancels all asynchronous operations.
    if (ref_) {
        DNSServiceRefDeallocate(ref_);
    }
    // Then release the js objects.
    if ( ! callback_.IsEmpty()) {
        callback_.Reset();
    }
    if ( ! context_.IsEmpty()) {
        context_.Reset();
    }
}

void
ServiceRef::Initialize(Local<Object> target) {
    Local<FunctionTemplate> t = Nan::New<FunctionTemplate>(New);
    constructor_template.Reset(t);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    t->SetClassName(Nan::New("DNSServiceRef").ToLocalChecked());
    
    Nan::SetAccessor(t->InstanceTemplate(), Nan::New("fd").ToLocalChecked(), fd_getter);
    Nan::SetAccessor(t->InstanceTemplate(),  Nan::New("initialized").ToLocalChecked(), initialized_getter);
    Nan::Set(target, Nan::New("DNSServiceRef").ToLocalChecked(), Nan::GetFunction(t).ToLocalChecked());
}

NAN_METHOD(ServiceRef::New) {
    if (argumentCountMismatch(info, 0)) {
        return throwArgumentCountMismatchException(info, 0);
    }
    ServiceRef * o = new ServiceRef();
    o->Wrap(info.Holder());
    info.GetReturnValue().Set(info.This());
}

bool
ServiceRef::IsInitialized() const { return ref_ != NULL; }

bool
ServiceRef::HasInstance(v8::Local<v8::Value> value) {
  if ( ! value->IsObject() ) return false;
  v8::Local<v8::Object> object = value->ToObject();
  return Nan::New(constructor_template)->HasInstance( object );
}

void
ServiceRef::SetCallback(v8::Local<v8::Function> callback) {
  if ( ! callback_.IsEmpty()) {
    callback_.Reset();
  }
  callback_.Reset<v8::Function>(callback);
}

v8::Local<v8::Function>
ServiceRef::GetCallback() const { return Nan::New(callback_); }

DNSServiceRef &
ServiceRef::GetServiceRef() { return ref_; }

void
ServiceRef::SetServiceRef(DNSServiceRef ref) { ref_ = ref; }

v8::Local<v8::Value>
ServiceRef::GetContext() { return Nan::New(context_); }

void
ServiceRef::SetContext(v8::Local<v8::Value> context) {
  if ( ! context_.IsEmpty()) {
    context_.Reset();
  }
  context_.Reset<v8::Value>(context);
}

v8::Local<v8::Object>
ServiceRef::GetThis() { return this_; }

void
ServiceRef::SetThis(v8::Local<v8::Object> This) { this_ = This; }

bool
ServiceRef::SetSocketFlags() {
    return true;
#if 0 // XXX I think IOWatcher does the right thing. TODO: check!
  int fd = DNSServiceRefSockFD(ref_);
  if (fd == -1) return false;
  return fcntl(fd, F_SETFD, FD_CLOEXEC) != -1 &&
    fcntl(fd, F_SETFL, O_NONBLOCK) != -1;
#endif
}

NAN_PROPERTY_GETTER(ServiceRef::fd_getter) {
    ServiceRef * service_ref = Nan::ObjectWrap::Unwrap<ServiceRef>(info.This());
    int fd = -1;
    if (service_ref->ref_) {
        fd = DNSServiceRefSockFD(service_ref->ref_);
        if (fd == -1) {
            return Nan::ThrowError("DNSServiceRefSockFD() failed");
        }
    }
    Local<Integer> v = Nan::New<Integer>(fd);
    info.GetReturnValue().Set(v);
}

NAN_PROPERTY_GETTER(ServiceRef::initialized_getter) {
    ServiceRef * service_ref = Nan::ObjectWrap::Unwrap<ServiceRef>(info.This());
    info.GetReturnValue().Set(Nan::New<Boolean>(service_ref->IsInitialized()));
}

} // end of namespace node_mdns
