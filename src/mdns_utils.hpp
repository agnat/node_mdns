#ifndef NODE_MDNS_UTILS_INCLUDED
#define NODE_MDNS_UTILS_INCLUDED

#include <sstream>
#include <errno.h>
#include <fcntl.h>

namespace node_mdns {

const char * errorString(DNSServiceErrorType error);
v8::Local<v8::Value> buildException(DNSServiceErrorType error_code);

inline
v8::Local<v8::Value>
throwError(const char * message) {
    Nan::ThrowError( Nan::Error(message) );
    return Nan::Undefined();
}

inline
v8::Local<v8::Value>
throwTypeError(const char * message) {
    Nan::ThrowTypeError( message );
    return Nan::Undefined();
}

inline
v8::Local<v8::Value>
throwMdnsError(DNSServiceErrorType error_code) {
    Nan::ThrowError( buildException(error_code) );
    return Nan::Undefined();
}

inline
bool
argumentCountMismatch(Nan::NAN_METHOD_ARGS_TYPE info, int expectedCount) {
    return info.Length() != expectedCount;
}

inline
v8::Local<v8::Value>
throwArgumentCountMismatchException(Nan::NAN_METHOD_ARGS_TYPE info, size_t expectedCount) {
    std::ostringstream msg;
    msg << "argument count mismatch: expected " << expectedCount
        << ", but got " <<  info.Length() << " arguments.";
    return throwError(msg.str().c_str());
}

inline
v8::Local<v8::Value>
stringOrUndefined(const char * str) {
    if (str) {
        return Nan::New(str).ToLocalChecked();
    } else {
        return Nan::Undefined();
    }
}

} // end of namespace node_mdns

#endif // NODE_MDNS_UTILS_INCLUDED
