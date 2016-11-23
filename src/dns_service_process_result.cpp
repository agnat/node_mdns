#include "mdns.hpp"

#include <v8.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(DNSServiceProcessResult) {
    Nan::HandleScope scope;
    if (argumentCountMismatch(info, 1)) {
        info.GetReturnValue().Set(throwArgumentCountMismatchException(info, 1));
    }
    if ( ! info[0]->IsObject()) {
        info.GetReturnValue().Set(throwTypeError("argument 1 must be a DNSServiceRef object"));
    }

    ServiceRef * ref = Nan::ObjectWrap::Unwrap<ServiceRef>(info[0]->ToObject());
    ref->SetThis(info.This());
    DNSServiceErrorType error = DNSServiceProcessResult(ref->GetServiceRef());
    if (error != kDNSServiceErr_NoError) {
        info.GetReturnValue().Set(throwMdnsError(error));
    }
    return;
}

} // end of namespace node_mdns
