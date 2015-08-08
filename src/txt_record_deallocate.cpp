#include "mdns.hpp"

#include "mdns_utils.hpp"
#include "txt_record_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(TXTRecordDeallocate) {
    if (argumentCountMismatch(info, 1)) {
      return throwArgumentCountMismatchException(info, 1);
    }
    if ( ! info[0]->IsObject() || ! TxtRecordRef::HasInstance(info[0]->ToObject())) {
      return throwTypeError("argument 1 must be a TXTRecordRef object");
    }

    TxtRecordRef * ref = Nan::ObjectWrap::Unwrap<TxtRecordRef>(info[0]->ToObject());
    TXTRecordDeallocate( & ref->GetTxtRecordRef());
}

} // end of namespace node_mdns
