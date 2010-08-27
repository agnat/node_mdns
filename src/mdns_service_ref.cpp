#include "mdns_service_ref.hpp"

#include <iostream>

using namespace v8;
using namespace node;

namespace node_mdns {

Persistent<FunctionTemplate> ServiceRef::constructor_template;

static Persistent<String> fd_symbol;

ServiceRef::ServiceRef() : ref_(), callback_(), context_() {}
ServiceRef::~ServiceRef() {
    std::cout << "ServiceRef::~ServiceRef()" << std::endl;
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
    if ( ! this_.IsEmpty()) {
        this_.Dispose();
    }
}

void
ServiceRef::Initialize(Handle<Object> target) {
    v8::Local<v8::FunctionTemplate> t = FunctionTemplate::New(New);
    constructor_template = Persistent<FunctionTemplate>::New(t);
    constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
    constructor_template->SetClassName(String::NewSymbol("DNSServiceRef"));

    fd_symbol = NODE_PSYMBOL("fd");

    constructor_template->InstanceTemplate()->SetAccessor(fd_symbol, fd_getter);
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
    ServiceRef * o = ObjectWrap::Unwrap<ServiceRef>(info.This());
    assert(o);
    assert(property == fd_symbol);

    int fd = -1;
    if (o->ref_) {
        fd = DNSServiceRefSockFD(o->ref_);
        if (fd == -1) {
            return ThrowException(Exception::Error(
                        String::New("DNSServiceRefSockFD() failed")));
        }
    }
    Local<Integer> v = Integer::New(fd);
    return scope.Close(v);
}

} // end of namespace node_mdns
