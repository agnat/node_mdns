#include "mdns.hpp"

#include <node_buffer.h>

#include "mdns_utils.hpp"
#include "txt_record_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

size_t length(Local<Value> v) {
    if (v->IsString()) {
        return Utf8Length(v);
    } else if (Buffer::HasInstance(v)) {
        return Buffer::Length(ToObject(v));
    } else {
        return 0;
    }
}

NAN_METHOD(TXTRecordSetValue) {
    if (argumentCountMismatch(info, 3)) {
        return throwArgumentCountMismatchException(info, 3);
    }
    if ( ! info[0]->IsObject() || ! TxtRecordRef::HasInstance(ToObject(info[0]))) {
        return throwTypeError("argument 1 must be a TXTRecordRef object");
    }
    TxtRecordRef * ref = Nan::ObjectWrap::Unwrap<TxtRecordRef>(ToObject(info[0]));

    if ( ! info[1]->IsString()) {
        return throwTypeError("argument 1 must be a string (key)");
    }
    Nan::Utf8String key(info[1]);
    
    if ( ! (info[2]->IsNull() || info[2]->IsUndefined() ||
        Buffer::HasInstance(info[2]) || info[2]->IsString())) {
        return throwTypeError("argument 1 must be null, undefined, a buffer or a string (value)");
    }
    DNSServiceErrorType code = TXTRecordSetValue( & ref->GetTxtRecordRef(), *key,
            length(info[2]),
            ((info[2]->IsNull()||info[2]->IsUndefined()) 
                ? NULL : info[2]->IsString() ? *Nan::Utf8String(info[2]) : Buffer::Data(ToObject(info[2]))));

    if (code != kDNSServiceErr_NoError) {
        return throwMdnsError(code);
    }
}

} // end of namespace node_mdns
