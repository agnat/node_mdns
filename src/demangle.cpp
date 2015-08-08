#include <v8.h>

#ifdef __GNUC__
#   include <cstdlib>
#   include <cxxabi.h>
#endif
using namespace v8;

namespace {

Handle<Value>
demangle(Arguments const& info) {
    Nan::HandleScope scope;  
    String::Utf8Value str(info[0]->ToString());
#ifdef __GNUC__
    int status;
    char * ret = abi::__cxa_demangle(*str, NULL, NULL, & status);
    Local<String> demangled = Nan::New(ret);
    ::free(ret);
#endif
    info.GetReturnValue().Set(demangled);
}

} // end of anaonymous namespace

extern "C"
void
init(Handle<Object> target) {
    Nan::Set(target, Nan::New("demangle").ToLocalChecked(),
            Nan::GetFunction(Nan::New<FunctionTemplate>(demangle)));
}
