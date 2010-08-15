#include "advertisement.hpp"

#include <limits>
#include <iostream>
#include <sstream>
#include <netinet/in.h>

#include <node.h>

namespace node_mdns {

using namespace v8;

Persistent<String> Advertisement::name_symbol;
Persistent<String> Advertisement::regtype_symbol;
Persistent<String> Advertisement::domain_symbol;

struct AdContext {
    AdContext(Local<Object> const& this_, Local<Value> const& callback) :
        this_(Persistent<Object>::New(this_)),
        callback(Persistent<Function>::New(Local<Function>::Cast(callback)))
        {}
    ~AdContext() { callback.Dispose(); this_.Dispose(); }

    Persistent<Object>   this_;
    Persistent<Function> callback;
};

void
Advertisement::Initialize(Handle<Object> target) {
    HandleScope scope;
    Local<FunctionTemplate> t = mDNSBase::Initialize(New);

    name_symbol = NODE_PSYMBOL("name");
    regtype_symbol = NODE_PSYMBOL("regtype");
    domain_symbol = NODE_PSYMBOL("domain");

    NODE_SET_PROTOTYPE_METHOD(t, "doStart", DoStart);

    target->Set(String::NewSymbol("Advertisement"), t->GetFunction());
}

Handle<Value>
Advertisement::DoStart(DNSServiceFlags flags, uint32_t interface_index,
        const char * name, const char * regtype, const char * domain,
        const char * host, uint16_t port, Handle<Value> const& txt_record_object,
        AdContext * context)
{
    if (ServiceRef()) {
        return ThrowException(Exception::Error(
                   String::New("Advertisement already started."))); 
    }

    if ( ! txt_record_object->IsUndefined()) {
        Local<Array> props = txt_record_object->ToObject()->GetPropertyNames();
        for (size_t i = 0; i < props->Length(); ++i) {
            String::Utf8Value key( props->Get(i)->ToString() );
            Handle<Value> value = txt_record_object->ToObject()->Get(props->Get(i));
            if (value->IsString()) {
                String::Utf8Value string_value( value->ToString());
                TXTRecordSetValue( &txt_record_, *key, value->ToString()->Utf8Length(), *string_value);
            }/* TODO: else if (node::Buffer::HasInstance(value)) {
                std::cout << "buffer" << std::endl;
            }*/ 
        }
    }
    uint16_t txt_record_length = TXTRecordGetLength( & txt_record_);
    const void * txt_record_ptr = txt_record_length ? TXTRecordGetBytesPtr( & txt_record_) : NULL;

    int status = DNSServiceRegister( & ServiceRef(), flags, interface_index,
            name, regtype, domain, host, port, txt_record_length,
            txt_record_ptr, & on_service_registered, context);
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

Advertisement::Advertisement() : mDNSBase() {
    TXTRecordCreate(& txt_record_, TXT_RECORD_BUFFER_SIZE, txt_record_buffer_);
}

Advertisement::~Advertisement() {
    TXTRecordDeallocate( & txt_record_);
}

void
Advertisement::on_service_registered(DNSServiceFlags flags,
        DNSServiceErrorType errorCode, const char * name,
        const char * regtype, const char * domain, AdContext * context)
{
    size_t argc = 3;
    Local<Value> args[3];
    if (kDNSServiceErr_NoError == errorCode) {
        Local<Object> info = Object::New();
        info->Set(name_symbol, String::New(name));
        info->Set(regtype_symbol, String::New(regtype));
        info->Set(domain_symbol, String::New(domain));

        args[0] = Local<Value>::New(Null());
        args[1] = info;
        args[2] = Integer::New(flags);
        argc = 3;
    } else {
        args[0] = buildException(errorCode);
        argc = 1;
    }
    context->callback->Call(context->this_, argc, args);
    delete context;
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

    Handle<Value> txt_record = Undefined();
    if ( ! args[7]->IsUndefined()) {
        if ( ! args[7]->IsObject()) {
            return ThrowException(Exception::TypeError(
                        String::New("argument 7 must be an object.")));
        }
        txt_record = args[7]->ToObject();
    }

    if ( ! args[8]->IsFunction()) {
        return ThrowException(Exception::TypeError(
                String::New("argument 8 must be a function")));
    }
    AdContext * ctx = new AdContext(args.This(), args[8]);

    Handle<Value> result = ad->DoStart(flags, interface_index,
            name.empty() ? NULL : name.c_str(),
            *regtype,
            domain.empty() ? NULL : domain.c_str(),
            host.empty() ? NULL : host.c_str(),
            port, txt_record, ctx);

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
    AdContext * ctx = static_cast<AdContext*>(context);
    Advertisement * ad = ObjectWrap::Unwrap<Advertisement>(ctx->this_);
    ad->on_service_registered(flags, errorCode, name, regtype, domain, ctx);
}

} // end of namespace node_mdns
