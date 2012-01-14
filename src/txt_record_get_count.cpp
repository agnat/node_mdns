#include <iostream>

#include <node.h>
#include <node_buffer.h>

#include "mdns_utils.hpp"
#include "txt_record_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

Handle<Value>
TXTRecordGetCount(Arguments const& args) {
    HandleScope scope;
    if (argumentCountMismatch(args, 1)) {
        return throwArgumentCountMismatchException(args, 1);
    }
    if ( ! args[0]->IsObject() || ! Buffer::HasInstance(args[0]->ToObject())) {
        return throwTypeError("argument 1 must be a buffer (txtRecord)");
    }

    Local<Object> buffer = args[0]->ToObject();
    uint16_t result = ::TXTRecordGetCount(Buffer::Length(buffer), Buffer::Data(buffer));
    return scope.Close(Integer::New(result));
}

} // end of namespace node_mdns
