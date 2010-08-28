#include "dns_service_ref.hpp"

using namespace v8;

namespace node_mdns {

Persistent<FunctionTemplate> ServiceRef::constructor_template;

static Persistent<String> fd_symbol;
static Persistent<String> initialized_symbol;

ServiceRef::ServiceRef() : ref_(), callback_(), context_() {}
ServiceRef::~ServiceRef() {
    // First, dispose the serice ref. This cancels all asynchronous operations.
    if (ref_) {
        DNSServiceRefDeallocate(ref_);
    }
    // Then release the js objects.
    if ( ! callback_.IsEmpty()) {
        callback_.Dispose();
    }
    if ( ! context_.IsEmpty()) {
        context_.Dispose();
    }
}

void
ServiceRef::Initialize(Handle<Object> target) {
    v8::Local<v8::FunctionTemplate> t = FunctionTemplate::New(New);
    constructor_template = Persistent<FunctionTemplate>::New(t);
    constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
    constructor_template->SetClassName(String::NewSymbol("DNSServiceRef"));

    fd_symbol = NODE_PSYMBOL("fd");
    initialized_symbol = NODE_PSYMBOL("initialized");

    constructor_template->InstanceTemplate()->SetAccessor(fd_symbol, fd_getter);
    constructor_template->InstanceTemplate()->SetAccessor(initialized_symbol, initialized_getter);
    target->Set(String::NewSymbol("DNSServiceRef"), constructor_template->GetFunction());
}

v8::Handle<v8::Value>
ServiceRef::New(const v8::Arguments & args) {
    HandleScope scope;
    ServiceRef * o = new ServiceRef();
    o->Wrap(args.Holder());
    return args.This();
}


Handle<Value>
ServiceRef::fd_getter(Local<String> property, AccessorInfo const& info) {
    HandleScope scope;
    ServiceRef * service_ref = ObjectWrap::Unwrap<ServiceRef>(info.This());
    int fd = -1;
    if (service_ref->ref_) {
        fd = DNSServiceRefSockFD(service_ref->ref_);
        if (fd == -1) {
            return ThrowException(Exception::Error(
                        String::New("DNSServiceRefSockFD() failed")));
        }
    }
    Local<Integer> v = Integer::New(fd);
    return scope.Close(v);
}

Handle<Value>
ServiceRef::initialized_getter(Local<String> property, AccessorInfo const& info) {
    HandleScope scope;
    ServiceRef * service_ref = ObjectWrap::Unwrap<ServiceRef>(info.This());
    return scope.Close(Boolean::New(service_ref->IsInitialized()));
}

} // end of namespace node_mdns
