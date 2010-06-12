#include "advertisement.hpp"

#include <limits>
#include <iostream>
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

bool
Advertisement::DoStart(DNSServiceFlags flags, uint32_t interface_index,
        const char * name, const char * regtype, const char * domain,
        const char * host, uint16_t port, uint16_t txt_record_length,
        const void * txt_record)
{
    if (ServiceRef()) return false;

    int status = DNSServiceRegister( & ServiceRef(), flags, interface_index,
            name, regtype, domain, host, port, txt_record_length,
            txt_record, & on_service_registered, this);
    if (kDNSServiceErr_NoError != status) {
        // XXX deal with error
        return false;
    }

    prepareSocket();

    Ref();

    return true;
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

    if ( 2 < args.Length() ||
        ! args[0]->IsString() || ! args[1]->IsInt32())
    {
        return ThrowException(Exception::Error(
                String::New("argument mismatch.")));
    }

    String::Utf8Value regtype(args[0]->ToString());

    int raw_port = args[1]->ToInteger()->Int32Value();
    if (std::numeric_limits<uint16_t>::max() < raw_port) {
        return ThrowException(Exception::Error(
                String::New("port number to large.")));
    }
    uint16_t port = static_cast<uint16_t>(htons(raw_port));

    DNSServiceFlags flags = 0;
    if ( 3 < args.Length()) {
        if ( ! args[2]->IsInt32()) {
            return ThrowException(Exception::Error(
                        String::New("argument mismatch.")));
        }
        flags = args[2]->ToInteger()->Int32Value();
    }

    uint32_t interface_index = 0;
    if ( 4 < args.Length()) {
        if ( ! args[3]->IsInt32()) {
            return ThrowException(Exception::Error(
                        String::New("argument mismatch.")));
        }
        interface_index = args[3]->ToInteger()->Int32Value();
    }

    char * name = NULL;
    if ( 5 < args.Length()) {
        if ( ! args[4]->IsString()) {
            return ThrowException(Exception::Error(
                        String::New("argument mismatch.")));
        }
        // XXX is there a better way to do this?
        name = strdup(*String::Utf8Value(args[4]->ToString()));
    }

    char * domain = NULL;
    if ( 6 < args.Length()) {
        if ( ! args[5]->IsString()) {
            return ThrowException(Exception::Error(
                        String::New("argument mismatch.")));
        }
        // XXX is there a better way to do this?
        domain = strdup(*String::Utf8Value(args[5]->ToString()));
    }

    char * host = NULL;
    if ( 7 < args.Length()) {
        if ( ! args[6]->IsString()) {
            return ThrowException(Exception::Error(
                        String::New("argument mismatch.")));
        }
        // XXX is there a better way to do this?
        host = strdup(*String::Utf8Value(args[6]->ToString()));
    }

    uint16_t txt_record_length = 0;
    void * txt_record = NULL;
    
    bool r = ad->DoStart(flags, interface_index, name, *regtype,
            domain, host, port, txt_record_length, txt_record);
    // XXX is there a better way to do this?
    free(name);
    free(domain);
    free(host);

    if (!r) {
        std::cout << "kaputt" << std::endl;
        // XXX
        /*
        return ThrowException(Exception::Error(
            String::New(ad->ErrorMessage())));
        */
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
