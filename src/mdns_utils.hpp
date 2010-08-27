#ifndef NODE_JS_MDNS_UTILS_INCLUDED
#define NODE_JS_MDNS_UTILS_INCLUDED

#include <dns_sd.h>
#include <errno.h>
#include <fcntl.h>
#include <node.h>
#include <node_events.h>

namespace node_mdns {

const char * errorString(DNSServiceErrorType error);
v8::Local<v8::Value> buildException(DNSServiceErrorType error_code);

inline
v8::Handle<v8::Value>
throwError(const char * message) {
    return ThrowException( v8::Exception::Error( v8::String::New( message )));
}

inline
v8::Handle<v8::Value>
throwTypeError(const char * message) {
    return ThrowException( v8::Exception::TypeError( v8::String::New( message )));
}

inline
v8::Local<v8::Value>
mdnsError(const char * message, DNSServiceErrorType error_code) {
    v8::Local<v8::String> str1 = v8::String::Concat( v8::String::New(message), v8::String::NewSymbol(": "));
    v8::Local<v8::String> str2 = v8::String::Concat(str1, v8::String::New(errorString(error_code)));
    return v8::Local<v8::Object>::Cast( v8::Exception::Error( str2 ));
}

inline
v8::Handle<v8::Value>
throwMdnsError(const char * message, DNSServiceErrorType error_code) {
    return ThrowException(mdnsError(message, error_code));
}

} // end of namespace node_mdns

#endif // NODE_JS_MDNS_UTILS_INCLUDED
