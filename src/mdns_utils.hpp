#ifndef NODE_MDNS_UTILS_INCLUDED
#define NODE_MDNS_UTILS_INCLUDED

#include <sstream>
#include <errno.h>
#include <fcntl.h>

namespace node_mdns {

const char * errorString(DNSServiceErrorType error);
v8::Local<v8::Value> buildException(DNSServiceErrorType error_code);

inline
v8::Handle<v8::Value>
throwError(const char * message) {
    NanThrowError( NanError(message) );
    return NanUndefined();
}

inline
v8::Handle<v8::Value>
throwTypeError(const char * message) {
    NanThrowTypeError( message );
    return NanUndefined();
}

inline
v8::Handle<v8::Value>
throwMdnsError(DNSServiceErrorType error_code) {
    NanThrowError( buildException(error_code) );
    return NanUndefined();
}

inline
bool
argumentCountMismatch(_NAN_METHOD_ARGS, int expectedCount) {
    return args.Length() != expectedCount;
}

inline
v8::Handle<v8::Value>
throwArgumentCountMismatchException(_NAN_METHOD_ARGS, size_t expectedCount) {
    std::ostringstream msg;
    msg << "argument count mismatch: expected " << expectedCount 
        << ", but got " <<  args.Length() << " arguments.";
    return throwError(msg.str().c_str());
}

inline
v8::Local<v8::Value>
stringOrUndefined(const char * str) {
    if (str) {
        return NanNew(str);
    } else {
        return NanUndefined();
    }
}

} // end of namespace node_mdns

#endif // NODE_MDNS_UTILS_INCLUDED
