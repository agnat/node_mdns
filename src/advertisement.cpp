#include "advertisement.hpp"

#include <limits>
#include <iostream>
#include <sstream>
#include <netinet/in.h>

namespace node_mdns {

using namespace v8;

Persistent<String> Advertisement::ready_symbol;

void
Advertisement::Initialize(Handle<Object> target) {
    HandleScope scope;
    Local<FunctionTemplate> t = mDNSBase::Initialize(target, New);

    ready_symbol = NODE_PSYMBOL("ready");

    NODE_SET_PROTOTYPE_METHOD(t, "doStart", DoStart);

    target->Set(String::NewSymbol("Advertisement"), t->GetFunction());
}

Handle<Value>
Advertisement::DoStart(DNSServiceFlags flags, uint32_t interface_index,
        const char * name, const char * regtype, const char * domain,
        const char * host, uint16_t port, uint16_t txt_record_length,
        const void * txt_record)
{
    if (ServiceRef()) {
        return ThrowException(Exception::Error(
                   String::New("Advertisement already started."))); 
    }

    int status = DNSServiceRegister( & ServiceRef(), flags, interface_index,
            name, regtype, domain, host, port, txt_record_length,
            txt_record, & on_service_registered, this);
    if (kDNSServiceErr_NoError != status) {
        return ThrowException(buildException(status));
    }

    Handle<Value> result = prepareSocket();
    if ( ! result->IsUndefined()) {
        return ThrowException(result);
    }

    Ref();

    return Undefined();
}

Handle<Value>
Advertisement::New(const Arguments & args) {
    HandleScope scope;

    Advertisement * ad = new Advertisement();
    ad->Wrap(args.This());
    return args.This();
}

Advertisement::Advertisement() : mDNSBase() {}


void
Advertisement::on_service_registered(DNSServiceFlags flags,
        DNSServiceErrorType errorCode, const char * name,
        const char * regtype, const char * domain)
{
    const size_t argc = 5;
    Local<Value> args[argc];
    if (kDNSServiceErr_NoError == errorCode) {
        args[0] = Local<Value>::New(Null());
        args[1] = Integer::New(flags);
        args[2] = String::New(name);
        args[3] = String::New(regtype);
        args[4] = String::New(domain);
        Emit(ready_symbol, argc, args);
    } else {
        args[0] = buildException(errorCode);
        Emit(ready_symbol, 1, args);
    }
}

Handle<Value>
Advertisement::DoStart(const Arguments & args) {
    HandleScope scope;
    Advertisement * ad = ObjectWrap::Unwrap<Advertisement>(args.This());

    std::ostringstream msg;
    if ( 9 != args.Length()) {
        msg << "argument mismatch. expected 9 but got " << args.Length();
        return ThrowException(Exception::Error(
                String::New(msg.str().c_str())));
    }

    DNSServiceFlags flags;
    if ( args[0]->IsUndefined()) {
        flags = 0;
    } else if (args[0]->IsInt32()) {
        flags = args[0]->ToInteger()->Int32Value();
    } else {
        return ThrowException(Exception::TypeError(
                    String::New("argument 0 must be an integer")));
    }
     
    uint32_t interface_index;
    if (args[1]->IsUndefined()) {
        interface_index = 0;
    } else if (args[1]->IsInt32()) {
        interface_index = args[1]->ToInteger()->Int32Value();
    } else {
        return ThrowException(Exception::TypeError(
                    String::New("argument 1 must be an integer")));
    }

    std::string name;
    if (args[2]->IsUndefined()) {
        // nothing to do
    } else if (args[2]->IsString()) {
        name = *String::Utf8Value(args[2]->ToString());
    } else {
        return ThrowException(Exception::TypeError(
                    String::New("argument 2 must be a string")));
    }

    if ( ! args[3]->IsString()) {
        return ThrowException(Exception::TypeError(
                    String::New("argument 3 must be a string")));
    }
    String::Utf8Value regtype(args[3]->ToString());

    std::string domain;
    if (args[4]->IsUndefined()) {
        // nothing to do
    } else if (args[4]->IsString()) {
        domain = *String::Utf8Value(args[4]->ToString());
    } else {
        return ThrowException(Exception::TypeError(
                    String::New("argument 4 must be a string.")));
    }

    std::string host;
    if (args[5]->IsUndefined()) {
        // nothing to do
    } else if (args[5]->IsString()) {
        host = *String::Utf8Value(args[5]->ToString());
    } else {
        return ThrowException(Exception::TypeError(
                    String::New("argument 5 must be a string")));
    }

    if ( ! args[6]->IsInt32()) {
        return ThrowException(Exception::TypeError(
                    String::New("argument 6 must be an integer")));
    }
    int raw_port = args[6]->ToInteger()->Int32Value();
    if (std::numeric_limits<uint16_t>::max() < raw_port ||
        0 > raw_port)
    {
        return ThrowException(Exception::TypeError(
                String::New("argument 6: illegal port number.")));
    }
    uint16_t port = static_cast<uint16_t>(htons(raw_port));

    // TODO: handle more arguments
    uint16_t txt_record_length = 0;
    void * txt_record = NULL;
    
    Handle<Value> result = ad->DoStart(flags, interface_index,
            name.empty() ? NULL : name.c_str(),
            *regtype,
            domain.empty() ? NULL : domain.c_str(),
            host.empty() ? NULL : host.c_str(),
            port, txt_record_length, txt_record);

    if (! result->IsUndefined()) {
        return ThrowException(result);
    }
    return Undefined();
}

void
Advertisement::on_service_registered(DNSServiceRef /*sdRef*/, DNSServiceFlags flags, 
        DNSServiceErrorType errorCode, const char *name,
        const char *regtype, const char *domain, void *context )
{
    Advertisement * ad = static_cast<Advertisement*>( context );
    ad->on_service_registered(flags, errorCode, name, regtype, domain);
}

} // end of namespace node_mdns
