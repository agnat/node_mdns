#include <limits>
#include <arpa/inet.h>
#include <dns_sd.h>

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

static
void
OnServiceRegistered(DNSServiceRef sdRef, DNSServiceFlags flags,
        DNSServiceErrorType errorCode, const char * name, const char * regtype,
        const char * domain, void * context)
{
    if ( ! context) return;

    HandleScope scope;
    ServiceRef * serviceRef = static_cast<ServiceRef*>(context);
    Handle<Function> callback = serviceRef->GetCallback();
    Handle<Object> this_ = serviceRef->GetThis();

    if ( ! callback.IsEmpty() && ! this_.IsEmpty()) {
        const size_t argc(7);
        Local<Value> args[argc];
        args[0] = Local<Object>::New(serviceRef->handle_);
        args[1] = Integer::New(flags);
        args[2] = Integer::New(errorCode);
        args[3] = String::New(name);
        args[4] = String::New(regtype);
        args[5] = String::New(domain);
        if (serviceRef->GetContext().IsEmpty()) {
            args[6] = Local<Value>::New(Null());
        } else {
            args[6] = Local<Value>::New(serviceRef->GetContext());
        }
        callback->Call(this_, argc, args);
    }
}


class scopedTXTRecord {
    TXTRecordRef record;
    TXTRecordRef *recordPtr;

protected:
    scopedTXTRecord(const scopedTXTRecord&);
    void operator=(const scopedTXTRecord&);

public:
    scopedTXTRecord()
        : recordPtr(NULL)
    {
    }

    ~scopedTXTRecord() {
        if (recordPtr != NULL) {
            TXTRecordDeallocate(recordPtr);
        }
    }

    void initialize() {
        recordPtr = &record;
        TXTRecordCreate(recordPtr, 0, NULL);
    }

    uint8_t getSize() const {
        return recordPtr != NULL? TXTRecordGetLength(recordPtr) : 0;
    }

    const void * getTXTRecord() const {
        return recordPtr != NULL? TXTRecordGetBytesPtr(recordPtr) : NULL;
    }

    DNSServiceErrorType setValue(const char *k, uint8_t len, const void *val) {
        return TXTRecordSetValue(recordPtr, k, len, val);
    }
};

bool
createTXTRecord(scopedTXTRecord& txtRecord, Local<Object>& object) {
    HandleScope scope;
    Local<Array> names = object->GetPropertyNames();
    uint32_t length = names->Length();

    for (uint32_t index = 0; index<length; ++index) {
        Local<Value> key = names->Get(index);

        if (key->IsString()) {
            Local<Value> buffer = object->Get(key);

            // A DNS-SD key is 7-bit ascii
            String::AsciiValue keyString(key);
            std::string buf(*keyString, keyString.length());

            Local<Object> obj;
            uint8_t valLen = 0;
            const void *value = NULL;

            if (Buffer::HasInstance(buffer)) {
                obj = buffer->ToObject();
                valLen = Buffer::Length(obj);
                value = Buffer::Data(obj);
            }

            if (txtRecord.setValue(buf.c_str(), valLen, value) != kDNSServiceErr_NoError) {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

Handle<Value>
dnsServiceRegister(Arguments const& args) {
    HandleScope scope;
    if (argumentCountMismatch(args, 11)) {
        return throwArgumentCountMismatchException(args, 11);
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

    bool has_name = false;
    if ( ! args[3]->IsNull() && ! args[3]->IsUndefined()) {
        if ( ! args[3]->IsString()) {
            return throwTypeError("argument 4 must be a string (name)");
        }
        has_name = true;
    }
    String::Utf8Value name(args[3]);

    if ( ! args[4]->IsString()) {
        return throwTypeError("argument 5 must be a string (regtype)");
    }
    String::Utf8Value regtype(args[4]->ToString());

    bool has_domain = false;
    if ( ! args[5]->IsNull() && ! args[5]->IsUndefined()) {
        if ( ! args[5]->IsString()) {
            return throwTypeError("argument 6 must be a string (domain)");
        }
        has_domain = true;
    }
    String::Utf8Value domain(args[5]);

    bool has_host = false;
    if ( ! args[6]->IsNull() && ! args[6]->IsUndefined()) {
        if ( ! args[6]->IsString()) {
            return throwTypeError("argument 7 must be a string (host)");
        }
        has_host = true;
    }
    String::Utf8Value host(args[6]);

    if ( ! args[7]->IsInt32()) {
        return throwTypeError("argument 8 must be an integer (port)");
    }
    int raw_port = args[7]->ToInteger()->Int32Value();
    if (raw_port > std::numeric_limits<uint16_t>::max() || raw_port < 0) {
        return throwError("argument 8: port number is out of bounds.");
    }
    uint16_t port = static_cast<uint16_t>(raw_port);

    scopedTXTRecord record;
    if (! args[8]->IsNull() && ! args[8]->IsUndefined()) {
        if (args[8]->IsObject()) {
            record.initialize();
            Local<Object> object = args[8]->ToObject();
            if (! createTXTRecord(record, object)) {
                return throwTypeError("argument 9 is not a valid TXT Record");
            }
        } else {
            return throwTypeError("argument 9 must be an object (txtRecord)");
        }
    }

    if ( ! args[9]->IsNull() && ! args[9]->IsUndefined()) {
        if ( ! args[9]->IsFunction()) {
            return throwTypeError("argument 10 must be a function (callBack)");
        }
        serviceRef->SetCallback(Local<Function>::Cast(args[9]));
    }

    if ( ! args[10]->IsNull() && ! args[10]->IsUndefined()) {
        serviceRef->SetContext(args[10]);
    }

    DNSServiceErrorType error = DNSServiceRegister(
            & serviceRef->GetServiceRef(),
            flags,
            interfaceIndex,
            has_name ? * name : NULL,
            *regtype,
            has_domain ? * domain : NULL,
            has_host ? * host : NULL,
            htons(port),
            record.getSize(),
            record.getTXTRecord(),
            OnServiceRegistered,
            serviceRef);
    if (error != kDNSServiceErr_NoError) {
        return throwMdnsError("dnsServiceRegister()", error);
    }
    if ( ! serviceRef->SetSocketFlags()) {
        return throwError("Failed to set socket flags (O_NONBLOCK, FD_CLOEXEC)");
    }
    return Undefined();
}

} // end of namespace node_mdns
