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

inline
v8::Local<v8::Object>
ToObject(v8::Local<v8::Value> v) {
#if (NODE_MODULE_VERSION >= NODE_11_0_MODULE_VERSION)
  return v->ToObject(v8::Isolate::GetCurrent()->GetCurrentContext()).ToLocalChecked();
#else
  return v->ToObject();
#endif
}

inline
int32_t
ToInt32(v8::Local<v8::Value> v) {
#if (NODE_MODULE_VERSION >= NODE_11_0_MODULE_VERSION)
  v8::Local<v8::Context> ctx = v8::Isolate::GetCurrent()->GetCurrentContext();
  return v->ToInt32(ctx).ToLocalChecked()->Value();
#else
  return v->ToInteger()->Int32Value();
#endif
}

inline
uint32_t
ToUint32(v8::Local<v8::Value> v) {
#if (NODE_MODULE_VERSION >= NODE_11_0_MODULE_VERSION)
  v8::Local<v8::Context> ctx = v8::Isolate::GetCurrent()->GetCurrentContext();
  return v->ToUint32(ctx).ToLocalChecked()->Value();
#else
  return v->ToInteger()->Uint32Value();
#endif
}

inline
v8::Local<v8::String>
ToString(v8::Local<v8::Value> v) {
#if (NODE_MODULE_VERSION >= NODE_11_0_MODULE_VERSION)
  v8::Local<v8::Context> ctx = v8::Isolate::GetCurrent()->GetCurrentContext();
  return v->ToString(ctx).ToLocalChecked();
#else
  return v->ToString();
#endif
}

inline
int
Utf8Length(v8::Local<v8::Value> v) {
#if (NODE_MODULE_VERSION >= NODE_11_0_MODULE_VERSION)
  return ToString(v)->Utf8Length(v8::Isolate::GetCurrent());
#else
  return ToString(v)->Utf8Length();
#endif
}

} // end of namespace node_mdns

#define LOC(file, line) \
  (file ":" NAN_STRINGIZE(line))
#endif // NODE_MDNS_UTILS_INCLUDED
