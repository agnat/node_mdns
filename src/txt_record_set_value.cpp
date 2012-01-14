#include <node.h>
#include <node_buffer.h>

#include "mdns_utils.hpp"
#include "txt_record_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

Handle<Value>
TXTRecordSetValue(Arguments const& args) {
    HandleScope scope;
    if (argumentCountMismatch(args, 3)) {
        return throwArgumentCountMismatchException(args, 3);
    }
    if ( ! args[0]->IsObject() || ! TxtRecordRef::HasInstance(args[0]->ToObject())) {
        return throwTypeError("argument 1 must be a TXTRecordRef object");
    }
    TxtRecordRef * ref = ObjectWrap::Unwrap<TxtRecordRef>(args[0]->ToObject());

    if ( ! args[1]->IsString()) {
        return throwTypeError("argument 1 must be a string (key)");
    }
    String::Utf8Value key(args[1]);
    
    if ( ! args[2]->IsNull() && ! args[2]->IsUndefined() && ! args[2]->IsString()) {
        return throwTypeError("argument 1 must be null, undefined or a string (value)");
    }
    bool has_value = args[2]->IsString();
    uint8_t length = has_value ? args[2]->ToString()->Utf8Length() : 0;
    
    DNSServiceErrorType code = TXTRecordSetValue( & ref->GetTxtRecordRef(), *key,
            length, *String::Utf8Value(args[2]->ToString()));

    if (code != kDNSServiceErr_NoError) {
        return throwMdnsError("failed to set txt record value", code);
    }
    return Undefined();
}

} // end of namespace node_mdns
