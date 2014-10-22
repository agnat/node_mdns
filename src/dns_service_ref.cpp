#include "mdns.hpp"

#include "dns_service_ref.hpp"
#include "mdns_utils.hpp"

using namespace v8;

namespace node_mdns {

Persistent<FunctionTemplate> ServiceRef::constructor_template;

ServiceRef::ServiceRef() : ref_(), callback_(), context_() {}

ServiceRef::~ServiceRef() {
    // First, dispose the serice ref. This cancels all asynchronous operations.
    if (ref_) {
        DNSServiceRefDeallocate(ref_);
    }
    // Then release the js objects.
    if ( ! callback_.IsEmpty()) {
        NanDisposePersistent(callback_);
    }
    if ( ! context_.IsEmpty()) {
        NanDisposePersistent(context_);
    }
}

void
ServiceRef::Initialize(Handle<Object> target) {
    Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
    NanAssignPersistent(constructor_template, t);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    t->SetClassName(NanNew("DNSServiceRef"));
    
    t->InstanceTemplate()->SetAccessor(NanNew("fd"), fd_getter);
    t->InstanceTemplate()->SetAccessor( NanNew("initialized"), initialized_getter);
    target->Set(NanNew("DNSServiceRef"), t->GetFunction());
}

NAN_METHOD(ServiceRef::New) {
    NanScope();
    if (argumentCountMismatch(args, 0)) {
        NanReturnValue(throwArgumentCountMismatchException(args, 0));
    }
    ServiceRef * o = new ServiceRef();
    o->Wrap(args.Holder());
    NanReturnValue(args.This());
}

bool
ServiceRef::IsInitialized() const { return ref_ != NULL; }

bool
ServiceRef::HasInstance(v8::Handle<v8::Value> value) {
  if ( ! value->IsObject() ) return false;
  v8::Local<v8::Object> object = value->ToObject();
  return NanNew(constructor_template)->HasInstance( object );
}

void
ServiceRef::SetCallback(v8::Handle<v8::Function> callback) {
  if ( ! callback_.IsEmpty()) {
    NanDisposePersistent(callback_);
  }
  NanAssignPersistent(callback_, callback);
}

v8::Handle<v8::Function>
ServiceRef::GetCallback() const { return NanNew(callback_); }

DNSServiceRef &
ServiceRef::GetServiceRef() { return ref_; }

void
ServiceRef::SetServiceRef(DNSServiceRef ref) { ref_ = ref; }

v8::Handle<v8::Value>
ServiceRef::GetContext() { return NanNew(context_); }

void
ServiceRef::SetContext(v8::Handle<v8::Value> context) {
  if ( ! context_.IsEmpty()) {
    NanDisposePersistent(context_);
  }
  NanAssignPersistent(context_, context);
}

v8::Handle<v8::Object>
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
    NanScope();
    ServiceRef * service_ref = ObjectWrap::Unwrap<ServiceRef>(args.This());
    int fd = -1;
    if (service_ref->ref_) {
        fd = DNSServiceRefSockFD(service_ref->ref_);
        if (fd == -1) {
            NanThrowError("DNSServiceRefSockFD() failed");
            NanReturnUndefined();
        }
    }
    Local<Integer> v = NanNew<Integer>(fd);
    NanReturnValue(v);
}

NAN_PROPERTY_GETTER(ServiceRef::initialized_getter) {
    NanScope();
    ServiceRef * service_ref = ObjectWrap::Unwrap<ServiceRef>(args.This());
    NanReturnValue(NanNew<Boolean>(service_ref->IsInitialized()));
}

} // end of namespace node_mdns
