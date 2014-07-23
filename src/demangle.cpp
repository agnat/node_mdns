#include <v8.h>

#ifdef __GNUC__
#   include <cstdlib>
#   include <cxxabi.h>
#endif
using namespace v8;

namespace {

Handle<Value>
demangle(Arguments const& args) {
    NanScope();  
    String::Utf8Value str(args[0]->ToString());
#ifdef __GNUC__
    int status;
    char * ret = abi::__cxa_demangle(*str, NULL, NULL, & status);
    Local<String> demangled = NanNew(ret);
    ::free(ret);
#endif
    NanReturnValue(demangled);
}

} // end of anaonymous namespace

extern "C"
void
init(Handle<Object> target) {
    target->Set(NanNew("demangle"),
            NanNew<FunctionTemplate>(demangle)->GetFunction());
}
