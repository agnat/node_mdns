#include "browser.hpp"

#include <iostream>

namespace node_mdns {

using namespace v8;

Persistent<String> Browser::changed_symbol;
Persistent<String> Browser::name_symbol;
Persistent<String> Browser::interface_symbol;
Persistent<String> Browser::regtype_symbol;
Persistent<String> Browser::domain_symbol;


void
Browser::Initialize(Handle<Object> target) {
    HandleScope scope;
    Local<FunctionTemplate> t = mDNSBase::Initialize(New);

    changed_symbol   = NODE_PSYMBOL("changed");
    name_symbol      = NODE_PSYMBOL("name");
    interface_symbol = NODE_PSYMBOL("interface_index");
    regtype_symbol   = NODE_PSYMBOL("regtype");
    domain_symbol    = NODE_PSYMBOL("domain");

    NODE_SET_PROTOTYPE_METHOD(t, "doStart", DoStart);

    target->Set(String::NewSymbol("Browser"), t->GetFunction());
}

bool
Browser::DoStart(DNSServiceFlags flags, uint32_t interface_index,
        const char * regtype, const char * domain)
{
    if (ServiceRef()) return false;

    int status = DNSServiceBrowse( & ServiceRef(), flags, interface_index,
            regtype, domain, & on_service_changed, this);
    if (kDNSServiceErr_NoError != status) {
        // XXX deal with error
        return false;
    }

    prepareSocket();

    Ref();

    return true;
}

Handle<Value>
Browser::New(const Arguments & args) {
    HandleScope scope;

    Browser * browser = new Browser();
    browser->Wrap(args.This());

    return args.This();
}

Browser::Browser() : mDNSBase() {}

Handle<Value>
Browser::DoStart(const Arguments & args) {
    HandleScope scope;
    Browser * browser = ObjectWrap::Unwrap<Browser>(args.This());

    if ( 1 > args.Length() || ! args[0]->IsString()) {
        return ThrowException(Exception::Error(
                String::New("argument mismatch.")));
    }

    String::Utf8Value regtype(args[0]->ToString());

    DNSServiceFlags flags = 0;
    uint32_t interface_index = 0;
    char * domain = NULL;
    
    bool r = browser->DoStart(flags, interface_index, *regtype, domain);

    if (!r) {
        // XXX
        /*
        return ThrowException(Exception::Error(
            String::New(ad->ErrorMessage())));
        */
    }
    return Undefined();
}

void
Browser::on_service_changed(DNSServiceRef /*sdRef*/, DNSServiceFlags flags,
        uint32_t interface_index, DNSServiceErrorType errorCode,
        const char * serviceName, const char * regtype,
        const char * replyDomain, void * context)
{
    Browser * browser = static_cast<Browser*>(context);
    browser->on_service_changed(flags, interface_index, errorCode,
            serviceName, regtype, replyDomain);
}

void
Browser::on_service_changed(DNSServiceFlags flags, uint32_t interface_index,
        DNSServiceErrorType errorCode, const char * name,
        const char * regtype, const char * domain)
{
    //std::cout << "=== service changed" << std::endl;
    const int argc = 3;
    Local<Value> args[argc];
    if (kDNSServiceErr_NoError == errorCode) {
        Local<Object> info = Object::New();
        info->Set(name_symbol, String::New(name));
        info->Set(interface_symbol, Integer::NewFromUnsigned(interface_index));
        info->Set(regtype_symbol, String::New(regtype));
        info->Set(domain_symbol, String::New(domain));

        args[0] = Local<Value>::New(Null());
        args[1] = info;
        args[2] = Integer::New(flags);
        Emit(changed_symbol, argc, args);
    } else {
        args[0] = buildException(errorCode);
        Emit(changed_symbol, 1, args);
    }
}

} // end of namespace node_mdns
