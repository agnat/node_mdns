#ifndef NODE_MDNS_UTILS_INCLUDED
#define NODE_MDNS_UTILS_INCLUDED

#include <sstream>
#include <errno.h>
#include <fcntl.h>

namespace node_mdns {

const char * errorString(DNSServiceErrorType error);
v8::Local<v8::Value> buildException(DNSServiceErrorType error_code);

inline
void
throwError(const char * message) {
    Nan::ThrowError( Nan::Error(message) );
}

inline
void
throwTypeError(const char * message) {
    Nan::ThrowTypeError( message );
}

inline
void
throwMdnsError(DNSServiceErrorType error_code) {
    Nan::ThrowError( buildException(error_code) );
}

inline
bool
argumentCountMismatch(Nan::NAN_METHOD_ARGS_TYPE info, int expectedCount) {
    return info.Length() != expectedCount;
}

inline
void
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
        return Nan::New<v8::String>(str).ToLocalChecked();
    } else {
        return Nan::Undefined();
    }
}

} // end of namespace node_mdns

#endif // NODE_MDNS_UTILS_INCLUDED
