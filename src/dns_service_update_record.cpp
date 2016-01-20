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

NAN_METHOD(DNSServiceUpdateRecord) {
    if (argumentCountMismatch(info, 5)) {
      return throwArgumentCountMismatchException(info, 5);
    }

    if ( ! ServiceRef::HasInstance(info[0])) {
      return throwTypeError("argument 1 must be a DNSServiceRef (sdRef)");
    }
    ServiceRef * serviceRef = Nan::ObjectWrap::Unwrap<ServiceRef>(info[0]->ToObject());

    if ( ! info[1]->IsNull()) {
      return throwError("argument 2 must be zero. Custom records are not supported yet.");
    }

    if ( ! info[2]->IsInt32()) {
      return throwError("argument 3 must be an integer (DNSServiceFlags)");
    }
    DNSServiceFlags flags = info[2]->ToInteger()->Int32Value();

    uint16_t txtLen(0);
    const void * txtRecord(NULL);
    if ( ! info[3]->IsNull() && ! info[3]->IsUndefined()) {
        if (Buffer::HasInstance(info[3])) {
            Local<Object> bufferObject = info[3]->ToObject();
            txtRecord = Buffer::Data(bufferObject);
            txtLen = Buffer::Length(bufferObject);
        } else if (TxtRecordRef::HasInstance(info[3])) {
            TxtRecordRef * ref = Nan::ObjectWrap::Unwrap<TxtRecordRef>(info[3]->ToObject());
            txtLen = TXTRecordGetLength( & ref->GetTxtRecordRef());
            txtRecord = TXTRecordGetBytesPtr( & ref->GetTxtRecordRef());
        } else {
          return throwTypeError("argument 4 must be a buffer or a dns_sd.TXTRecordRef");
        }
    }

    if ( ! info[4]->IsInt32()) {
      return throwError("argument 5 must be an integer (ttl)");
    }
    int ttl = info[4]->ToInteger()->Int32Value();

    DNSServiceErrorType error = DNSServiceUpdateRecord(
            serviceRef->GetServiceRef(),
            NULL,
            flags,
            txtLen,
            txtRecord,
            ttl
        );
    if (error != kDNSServiceErr_NoError) {
      return throwMdnsError(error);
    }
}

} // end of namespace node_mdns
