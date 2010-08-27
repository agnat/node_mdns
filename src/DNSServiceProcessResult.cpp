#include <sstream>

#include <v8.h>

#include "mdns_utils.hpp"
#include "mdns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

Handle<Value>
DNSServiceProcessResult(Arguments const& args) {
    HandleScope scope;
    if (args.Length() != 1) {
        std::ostringstream msg;
        msg << "argument mismatch. expected 1 but got " << args.Length();
        return throwError(msg.str().c_str());
    }
    if ( ! args[0]->IsObject()) {
        return throwTypeError("argument 1 must be a DNSServiceRef object");
    }

    ServiceRef * ref = ObjectWrap::Unwrap<ServiceRef>(args[0]->ToObject());
    DNSServiceErrorType error = DNSServiceProcessResult(ref->GetServiceRef());
    if (error != kDNSServiceErr_NoError) {
        return throwMdnsError("DNSServiceProcessResult()", error);
    }
}

} // end of namespace node_mdns
