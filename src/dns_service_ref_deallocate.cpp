#include "mdns.hpp"

#include <v8.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(DNSServiceRefDeallocate) {
    Nan::HandleScope scope;
    if (argumentCountMismatch(info, 1)) {
        info.GetReturnValue().Set(throwArgumentCountMismatchException(info, 1));
    }
    if ( ! info[0]->IsObject() || ! ServiceRef::HasInstance(info[0]->ToObject())) {
        info.GetReturnValue().Set(throwTypeError("argument 1 must be a DNSServiceRef object"));
    }

    ServiceRef * ref = Nan::ObjectWrap::Unwrap<ServiceRef>(info[0]->ToObject());
    if ( ! ref->IsInitialized()) {
        info.GetReturnValue().Set(throwError("DNSServiceRef is not initialized"));
    }
    DNSServiceRefDeallocate( ref->GetServiceRef());
    ref->SetServiceRef(NULL);

    return;
}

} // end of namespace node_mdns
