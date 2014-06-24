#include "mdns.hpp"

#include <v8.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(DNSServiceProcessResult) {
    NanScope();
    if (argumentCountMismatch(args, 1)) {
        NanReturnValue(throwArgumentCountMismatchException(args, 1));
    }
    if ( ! args[0]->IsObject()) {
        NanReturnValue(throwTypeError("argument 1 must be a DNSServiceRef object"));
    }

    ServiceRef * ref = ObjectWrap::Unwrap<ServiceRef>(args[0]->ToObject());
    ref->SetThis(args.This());
    DNSServiceErrorType error = DNSServiceProcessResult(ref->GetServiceRef());
    if (error != kDNSServiceErr_NoError) {
        NanReturnValue(throwMdnsError(error));
    }
    NanReturnUndefined();
}

} // end of namespace node_mdns
