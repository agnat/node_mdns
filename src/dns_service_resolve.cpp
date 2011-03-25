#include <arpa/inet.h>
#include <v8.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

void
ParseTxtRecord(uint16_t txtLen, const unsigned char * txtRecord)
{
    char keyBuf[256];
    uint16_t keyBufLen = sizeof(keyBuf);
    uint8_t valueLen;
    const void *value;
    uint16_t keyCount = TXTRecordGetCount(txtLen, txtRecord);

    for (uint16_t index = 0; index<keyCount; ++index) {
        DNSServiceErrorType error = TXTRecordGetItemAtIndex(txtLen, txtRecord,
            index, keyBufLen, keyBuf, &valueLen, &value);

        switch(error) {
        case kDNSServiceErr_NoError:
            fprintf(stderr, "Found key: %s", keyBuf);
            break;
        case kDNSServiceErr_NoMemory:
            break;
        case kDNSServiceErr_Invalid:
            break;
        default:
            break;
        }
    }
}

void
OnResolve(DNSServiceRef sdRef, DNSServiceFlags flags,
        uint32_t interfaceIndex, DNSServiceErrorType errorCode,
        const char * fullname, const char * hosttarget, uint16_t port,
        uint16_t txtLen, const unsigned char * txtRecord, void * context)
{
    if ( ! context) return;

    HandleScope scope;
    ServiceRef * serviceRef = static_cast<ServiceRef*>(context);
    Handle<Function> callback = serviceRef->GetCallback();
    Handle<Object> this_ = serviceRef->GetThis();

    const size_t argc(9);
    Local<Value> args[argc];
    args[0] = Local<Object>::New(serviceRef->handle_);
    args[1] = Integer::New(flags);
    args[2] = Integer::New(interfaceIndex);
    args[3] = Integer::New(errorCode);
    args[4] = String::New(fullname);
    args[5] = String::New(hosttarget);
    args[6] = Integer::New( ntohs(port) );
    args[7] = Local<Value>::New(Null()); // TODO: create txtRecord buffer

    ParseTxtRecord(txtLen, txtRecord);

    if (serviceRef->GetContext().IsEmpty()) {
        args[8] = Local<Value>::New(Null());
    } else {
        args[8] = Local<Value>::New(serviceRef->GetContext());
    }
    callback->Call(this_, argc, args);
}

Handle<Value>
dnsServiceResolve(Arguments const& args) {
    HandleScope scope;

    if (argumentCountMismatch(args, 8)) {
        return throwArgumentCountMismatchException(args, 8);
    }

    if ( ! ServiceRef::HasInstance(args[0])) {
        return throwTypeError("argument 1 must be a DNSServiceRef (sdRef)");
    }
    ServiceRef * serviceRef = ObjectWrap::Unwrap<ServiceRef>(args[0]->ToObject());
    if (serviceRef->IsInitialized()) {
        return throwError("DNSServiceRef is already initialized");
    }

    if ( ! args[1]->IsInt32()) {
        return throwError("argument 2 must be an integer (DNSServiceFlags)");
    }
    DNSServiceFlags flags = args[1]->ToInteger()->Int32Value();

    if ( ! args[2]->IsInt32()) {
        return throwTypeError("argument 3 must be an integer (interfaceIndex)");
    }
    uint32_t interfaceIndex = args[2]->ToInteger()->Int32Value();

    if ( ! args[3]->IsString()) {
        return throwTypeError("argument 4 must be a string (name)");
    }
    String::Utf8Value name(args[3]->ToString());

    if ( ! args[4]->IsString()) {
        return throwTypeError("argument 5 must be a string (regtype)");
    }
    String::Utf8Value regtype(args[4]->ToString());

    if ( ! args[5]->IsString()) {
        return throwTypeError("argument 6 must be a string (domain)");
    }
    String::Utf8Value domain(args[5]->ToString());

    if ( ! args[6]->IsFunction()) {
        return throwTypeError("argument 7 must be a function (callBack)");
    }
    serviceRef->SetCallback(Local<Function>::Cast(args[6]));

    if ( ! args[7]->IsNull() && ! args[7]->IsUndefined()) {
        serviceRef->SetContext(args[7]);
    }

    DNSServiceErrorType error = DNSServiceResolve( & serviceRef->GetServiceRef(),
            flags, interfaceIndex, *name, *regtype, *domain, OnResolve, serviceRef);

    if (error != kDNSServiceErr_NoError) {
        return throwMdnsError("dnsServiceResolve()", error);
    }
    if ( ! serviceRef->SetSocketFlags()) {
        return throwError("Failed to set socket flags (O_NONBLOCK, FD_CLOEXEC)");
    }

    return Undefined();
}

} // end of namespace node_mdns
