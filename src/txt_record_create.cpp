#include "mdns.hpp"

#include <node_buffer.h>

#include "mdns_utils.hpp"
#include "txt_record_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(TXTRecordCreate) {
    if (argumentCountMismatch(info, 2)) {
        return throwArgumentCountMismatchException(info, 2);
    }
    if ( ! info[0]->IsObject() || ! TxtRecordRef::HasInstance(info[0]->ToObject())) {
        return throwTypeError("argument 1 must be a TXTRecordRef object");
    }

    void * buffer = NULL;
    uint16_t buffer_length = 0;
    if ( ! info[1]->IsUndefined() && ! info[1]->IsNull()) {
        if ( ! info[1]->IsObject() || ! Buffer::HasInstance(info[1]->ToObject())) {
            return throwTypeError("argument 1 must be a buffer");
        }
        Local<Object> buffer_object = info[1]->ToObject();
        buffer = Buffer::Data(buffer_object);
        buffer_length = Buffer::Length(buffer_object);
    }

    TxtRecordRef * ref = Nan::ObjectWrap::Unwrap<TxtRecordRef>(info[0]->ToObject());
    TXTRecordCreate( & ref->GetTxtRecordRef(), buffer_length, buffer);
}

} // end of namespace node_mdns
