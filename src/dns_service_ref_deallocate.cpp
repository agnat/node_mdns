#include "mdns.hpp"

#include <v8.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(DNSServiceRefDeallocate) {
    NanScope();
    if (argumentCountMismatch(args, 1)) {
        NanReturnValue(throwArgumentCountMismatchException(args, 1));
    }
    if ( ! args[0]->IsObject() || ! ServiceRef::HasInstance(args[0]->ToObject())) {
        NanReturnValue(throwTypeError("argument 1 must be a DNSServiceRef object"));
    }

    ServiceRef * ref = ObjectWrap::Unwrap<ServiceRef>(args[0]->ToObject());
    if ( ! ref->IsInitialized()) {
        NanReturnValue(throwError("DNSServiceRef is not initialized"));
    }
    DNSServiceRefDeallocate( ref->GetServiceRef());
    ref->SetServiceRef(NULL);

    NanReturnUndefined();
}

} // end of namespace node_mdns
