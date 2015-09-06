#include "mdns.hpp"

#include "mdns_utils.hpp"
#include "txt_record_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(TXTRecordGetLength) {
    Nan::HandleScope scope;
    if (argumentCountMismatch(info, 1)) {
        info.GetReturnValue().Set(throwArgumentCountMismatchException(info, 1));
    }
    if ( ! info[0]->IsObject() || ! TxtRecordRef::HasInstance(info[0]->ToObject())) {
        info.GetReturnValue().Set(throwTypeError("argument 1 must be a buffer (txtRecord)"));
    }
    TxtRecordRef * ref = Nan::ObjectWrap::Unwrap<TxtRecordRef>(info[0]->ToObject());
    uint16_t result = ::TXTRecordGetLength( & ref->GetTxtRecordRef());
    info.GetReturnValue().Set(Nan::New<Integer>(result));
}

} // end of namespace node_mdns
