#include <v8.h>

#include "mdns_utils.hpp"
#include "mdns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

Handle<Value>
DNSServiceRefSockFD(Arguments const& args) {
    HandleScope scope;
    if (argumentCountMismatch(args, 1)) {
        return throwArgumentCountMismatchException(args, 1);
    }
    if ( ! args[0]->IsObject()) {
        return throwTypeError("argument 1 must be a DNSServiceRef object");
    }

    ServiceRef * ref = ObjectWrap::Unwrap<ServiceRef>(args[0]->ToObject());
    int fd = DNSServiceRefSockFD( ref->GetServiceRef());
    if (fd == -1) {
        return throwError("failed to get socket file descriptor");
    }
    return scope.Close( Integer::New( fd ));
}

} // end of namespace node_mdns
