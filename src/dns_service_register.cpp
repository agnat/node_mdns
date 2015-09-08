#include "mdns.hpp"

#include <limits>

#ifndef WIN32 // XXX
#include <arpa/inet.h>
#endif

#include <node_buffer.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"
#include "txt_record_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

static
void
DNSSD_API
OnServiceRegistered(DNSServiceRef sdRef, DNSServiceFlags flags,
        DNSServiceErrorType errorCode, const char * name,
        const char * serviceType, const char * domain, void * context)
{
    if ( ! context) return;

    Nan::HandleScope scope;
    ServiceRef * serviceRef = static_cast<ServiceRef*>(context);
    Local<Function> callback = serviceRef->GetCallback();
    Local<Object> this_ = serviceRef->GetThis();

    if ( ! callback.IsEmpty() && ! this_.IsEmpty()) {
        const size_t argc(7);
        Local<Value> info[argc];
        info[0] = serviceRef->handle();
        info[1] = Nan::New<Integer>(flags);
        info[2] = Nan::New<Integer>(errorCode);
        info[3] = stringOrUndefined(name);
        info[4] = stringOrUndefined(serviceType);
        info[5] = stringOrUndefined(domain);
        if (serviceRef->GetContext().IsEmpty()) {
            info[6] = Nan::Undefined();
        } else {
            info[6] = serviceRef->GetContext();
        }
        Nan::MakeCallback(this_, callback, argc, info);
    }
}

NAN_METHOD(DNSServiceRegister) {
    if (argumentCountMismatch(info, 11)) {
      return throwArgumentCountMismatchException(info, 11);
    }

    if ( ! ServiceRef::HasInstance(info[0])) {
      return throwTypeError("argument 1 must be a DNSServiceRef (sdRef)");
    }
    ServiceRef * serviceRef = Nan::ObjectWrap::Unwrap<ServiceRef>(info[0]->ToObject());
    if (serviceRef->IsInitialized()) {
      return throwError("DNSServiceRef is already initialized");
    }

    if ( ! info[1]->IsInt32()) {
      return throwError("argument 2 must be an integer (DNSServiceFlags)");
    }
    DNSServiceFlags flags = info[1]->ToInteger()->Int32Value();

    if ( ! info[2]->IsUint32() && ! info[2]->IsInt32()) {
      return throwTypeError("argument 3 must be an integer (interfaceIndex)");
    }
    uint32_t interfaceIndex = info[2]->ToInteger()->Uint32Value();

    bool has_name = false;
    if ( ! info[3]->IsNull() && ! info[3]->IsUndefined()) {
        if ( ! info[3]->IsString()) {
          return throwTypeError("argument 4 must be a string (name)");
        }
        has_name = true;
    }
    String::Utf8Value name(info[3]);

    if ( ! info[4]->IsString()) {
      return throwTypeError("argument 5 must be a string (service type)");
    }
    String::Utf8Value serviceType(info[4]->ToString());

    bool has_domain = false;
    if ( ! info[5]->IsNull() && ! info[5]->IsUndefined()) {
        if ( ! info[5]->IsString()) {
          return throwTypeError("argument 6 must be a string (domain)");
        }
        has_domain = true;
    }
    String::Utf8Value domain(info[5]);

    bool has_host = false;
    if ( ! info[6]->IsNull() && ! info[6]->IsUndefined()) {
        if ( ! info[6]->IsString()) {
          return throwTypeError("argument 7 must be a string (host)");
        }
        has_host = true;
    }
    String::Utf8Value host(info[6]);

    if ( ! info[7]->IsInt32()) {
      return throwTypeError("argument 8 must be an integer (port)");
    }
    int raw_port = info[7]->ToInteger()->Int32Value();
    if (raw_port > std::numeric_limits<uint16_t>::max() || raw_port < 0) {
      return throwError("argument 8: port number is out of bounds.");
    }
    uint16_t port = static_cast<uint16_t>(raw_port);

    uint16_t txtLen(0);
    const void * txtRecord(NULL);
    if ( ! info[8]->IsNull() && ! info[8]->IsUndefined()) {
        if (Buffer::HasInstance(info[8])) {
            Local<Object> bufferObject = info[8]->ToObject();
            txtRecord = Buffer::Data(bufferObject);
            txtLen = Buffer::Length(bufferObject);
        } else if (TxtRecordRef::HasInstance(info[8])) {
            TxtRecordRef * ref = Nan::ObjectWrap::Unwrap<TxtRecordRef>(info[8]->ToObject());
            txtLen = TXTRecordGetLength( & ref->GetTxtRecordRef());
            txtRecord = TXTRecordGetBytesPtr( & ref->GetTxtRecordRef());
        } else {
          return throwTypeError("argument 9 must be a buffer or a dns_sd.TXTRecordRef");
        }
    }

    if ( ! info[9]->IsNull() && ! info[9]->IsUndefined()) {
        if ( ! info[9]->IsFunction()) {
          return throwTypeError("argument 10 must be a function (callBack)");
        }
        serviceRef->SetCallback(Local<Function>::Cast(info[9]));
    }

    if ( ! info[10]->IsNull() && ! info[10]->IsUndefined()) {
        serviceRef->SetContext(info[10]);
    }

    // eleven arguments ... srsly?
    DNSServiceErrorType error = DNSServiceRegister(
            & serviceRef->GetServiceRef(),
            flags,
            interfaceIndex,
            has_name ? * name : NULL,
            *serviceType,
            has_domain ? * domain : NULL,
            has_host ? * host : NULL,
            htons(port),
            txtLen,
            txtRecord,
            info[9]->IsFunction() ? OnServiceRegistered : NULL,
            serviceRef);
    if (error != kDNSServiceErr_NoError) {
      return throwMdnsError(error);
    }
    if ( ! serviceRef->SetSocketFlags()) {
      return throwError("Failed to set socket flags (O_NONBLOCK, FD_CLOEXEC)");
    }
}

} // end of namespace node_mdns
