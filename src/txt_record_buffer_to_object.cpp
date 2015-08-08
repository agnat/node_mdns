#include "mdns.hpp"

#include <vector>

#include <node_buffer.h>

#include "mdns_utils.hpp"
#include "txt_record_ref.hpp"

using namespace v8;
using namespace node;

namespace node_mdns {

NAN_METHOD(txtRecordBufferToObject) {
    if (argumentCountMismatch(info, 1)) {
      return throwArgumentCountMismatchException(info, 1);
    }
    if ( ! info[0]->IsObject() || ! Buffer::HasInstance(info[0]->ToObject())) {
      return throwTypeError("argument 1 must be a buffer (txtRecord)");
    }
    Local<Object> buffer = info[0]->ToObject();

    Local<Object> result = Nan::New<Object>();
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
          return throwMdnsError(error);
        }
        if (value_ptr) {
          Nan::Set(result, Nan::New(&*key.begin()).ToLocalChecked(),
                Nan::New(static_cast<const char*>(value_ptr), value_length).ToLocalChecked());
        } else {
            Nan::Set(result, Nan::New(&*key.begin()).ToLocalChecked(), Nan::Undefined());
        }
    }
    info.GetReturnValue().Set(result);
}

} // end of namespace node_mdns
