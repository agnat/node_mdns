#include "mdns.hpp"

#include <vector>

#include <node_buffer.h>

#include "mdns_utils.hpp"
#include "txt_record_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(txtRecordBufferToObject) {
    NanScope();
    if (argumentCountMismatch(args, 1)) {
        NanReturnValue(throwArgumentCountMismatchException(args, 1));
    }
    if ( ! args[0]->IsObject() || ! Buffer::HasInstance(args[0]->ToObject())) {
        NanReturnValue(throwTypeError("argument 1 must be a buffer (txtRecord)"));
    }
    Local<Object> buffer = args[0]->ToObject();

    Local<Object> result = NanNew<Object>();
    std::vector<char> key(16);
    size_t buffer_length = Buffer::Length(buffer);
    void * data = Buffer::Data(buffer);
    uint16_t item_count = TXTRecordGetCount(buffer_length, data);
    DNSServiceErrorType error;
    const void * value_ptr;
    uint8_t value_length;
    for (uint16_t i = 0; i < item_count; ++i) {
        while (kDNSServiceErr_NoMemory == (error =
                    TXTRecordGetItemAtIndex(buffer_length, data, i, key.size(),
                        &*key.begin(), & value_length, & value_ptr)))
        {
            key.resize(key.size() * 2);
        }
        if (error != kDNSServiceErr_NoError) {
            NanReturnValue(throwMdnsError(error));
        }
        if (value_ptr) {
            result->Set(NanNew(&*key.begin()),
                NanNew(static_cast<const char*>(value_ptr), value_length));
        } else {
            result->Set(NanNew(&*key.begin()), NanUndefined());
        }
    }
    NanReturnValue(result);
}

} // end of namespace node_mdns
