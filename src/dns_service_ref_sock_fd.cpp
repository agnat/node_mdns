#include "mdns.hpp"

#include <v8.h>

#include "mdns_utils.hpp"
#include "dns_service_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(DNSServiceRefSockFD) {
    if (argumentCountMismatch(info, 1)) {
        return throwArgumentCountMismatchException(info, 1);
    }
    if ( ! info[0]->IsObject() || ! ServiceRef::HasInstance(info[0]->ToObject())) {
      return throwTypeError("argument 1 must be a DNSServiceRef object");
    }

    ServiceRef * ref = Nan::ObjectWrap::Unwrap<ServiceRef>(info[0]->ToObject());
    if ( ! ref->IsInitialized()) {
      return throwError("DNSServiceRef is not initialized");
    }
    int fd = DNSServiceRefSockFD( ref->GetServiceRef());
    if (fd == -1) {
      return throwError("failed to get socket file descriptor");
    }
    info.GetReturnValue().Set( Nan::New<Integer>( fd ));
}

} // end of namespace node_mdns
