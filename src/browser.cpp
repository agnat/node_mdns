#include "browser.hpp"

#include <iostream>

namespace node_mdns {

using namespace v8;

void
Browser::Initialize(Handle<Object> target) {
    HandleScope scope;
    Local<FunctionTemplate> t = mDNSBase::Initialize(target,New);

    NODE_SET_PROTOTYPE_METHOD(t, "doStart", DoStart);
    NODE_SET_PROTOTYPE_METHOD(t, "stop", Stop);

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
    std::cout << "=== service changed" << std::endl;
}

} // end of namespace node_mdns
