#include "mdns.hpp"

#include "mdns_utils.hpp"
#include "txt_record_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(TXTRecordGetLength) {
    NanScope();
    if (argumentCountMismatch(args, 1)) {
        NanReturnValue(throwArgumentCountMismatchException(args, 1));
    }
    if ( ! args[0]->IsObject() || ! TxtRecordRef::HasInstance(args[0]->ToObject())) {
        NanReturnValue(throwTypeError("argument 1 must be a buffer (txtRecord)"));
    }
    TxtRecordRef * ref = ObjectWrap::Unwrap<TxtRecordRef>(args[0]->ToObject());
    uint16_t result = ::TXTRecordGetLength( & ref->GetTxtRecordRef());
    NanReturnValue(NanNew<Integer>(result));
}

} // end of namespace node_mdns
