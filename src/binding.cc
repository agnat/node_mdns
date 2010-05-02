#include <dns_sd.h>
#include <errno.h>
#include <fcntl.h>
#include <node.h>
#include <node_events.h>

#include <iostream>

using namespace v8;
using namespace node;

namespace {

template <typename T>
void
io_event(EV_P_ ev_io *w, int revents) {
    T * sink = static_cast<T*>(w->data);
    sink->Event(revents);
}

inline
const char *
errorString(DNSServiceErrorType error) {
    switch (error) {
        case kDNSServiceErr_NoError:
            return "NoError";
        case kDNSServiceErr_Unknown:
            return "Unknown";
        case kDNSServiceErr_NoSuchName:
            return "NoSuchName";
        case kDNSServiceErr_NoMemory:
            return "NoMemory";
        case kDNSServiceErr_BadParam:
            return "BadParam";
        case kDNSServiceErr_BadReference:
            return "BadReference";
        case kDNSServiceErr_BadState:
            return "BadState";
        case kDNSServiceErr_BadFlags:
            return "BadFlags";
        case kDNSServiceErr_Unsupported:
            return "Unsupported";
        case kDNSServiceErr_NotInitialized:
            return "NotInitialized";
        case kDNSServiceErr_AlreadyRegistered:
            return "AlreadyRegistered";
        case kDNSServiceErr_NameConflict:
            return "NameConflict";
        case kDNSServiceErr_Invalid:
            return "Invalid";
        case kDNSServiceErr_Firewall:
            return "Firewall";
        case kDNSServiceErr_Incompatible:
            return "Incompatible";
        case kDNSServiceErr_BadInterfaceIndex:
            return "BadInterfaceIndex";
        case kDNSServiceErr_Refused:
            return "Refused";
        case kDNSServiceErr_NoSuchRecord:
            return "NoSuchRecord";
        case kDNSServiceErr_NoAuth:
            return "NoAuth";
        case kDNSServiceErr_NoSuchKey:
            return "NoSuchKey";
        case kDNSServiceErr_NATTraversal:
            return "NATTraversal";
        case kDNSServiceErr_DoubleNAT:
            return "DoubleNAT";
        case kDNSServiceErr_BadTime:
            return "BadTime";
        case kDNSServiceErr_BadSig:
            return "BadSig";
        case kDNSServiceErr_BadKey:
            return "BadKey";
        case kDNSServiceErr_Transient:
            return "Transient";
        case kDNSServiceErr_ServiceNotRunning:
            return "ServiceNotRunning";
        case kDNSServiceErr_NATPortMappingUnsupported:
            return "NATPortMappingUnsupported";
        case kDNSServiceErr_NATPortMappingDisabled:
            return "NATPortMappingDisabled";
#ifdef kDNSServiceErr_NoRouter
        case kDNSServiceErr_NoRouter:
            return "NoRouter";
#endif
#ifdef kDNSServiceErr_PollingMode
        case kDNSServiceErr_PollingMode:
            return "PollingMode";
#endif
    };
    return "Unknown DNSServiceError";
}

inline
Local<Value>
buildException(DNSServiceErrorType error_code) {
    HandleScope scope;

    const char * error_str = errorString(error_code);
    Local<String> error_msg = String::New(error_str);
    Local<Value> error_v = Exception::Error(error_msg);
    Local<Object> error = Local<Object>::Cast(error_v);
    return scope.Close(error);
}

void
prepareSocket(DNSServiceRef ref, ev_io * read_watcher) {
    int fd = DNSServiceRefSockFD(ref);
    if (-1 == fd) {
        // XXX deal with error
        return;
    }

    if (-1 == fcntl(fd, F_SETFL, O_NONBLOCK) ||
        -1 == fcntl(fd, F_SETFD, FD_CLOEXEC))
    {
        // XXX deal with error
        return;
    }

    ev_io_set(read_watcher, fd, EV_READ);
    ev_io_start(EV_DEFAULT_ read_watcher);
}


//=== Advertisement ============================================================
class Advertisement : public EventEmitter {
    public:
        static
        void
        Initialize(Handle<Object> target) {
            HandleScope scope;
            Local<FunctionTemplate> t = FunctionTemplate::New(New);

            t->Inherit(EventEmitter::constructor_template);
            t->InstanceTemplate()->SetInternalFieldCount(1);

            ready_symbol = NODE_PSYMBOL("ready");
            stop_symbol = NODE_PSYMBOL("stop");

            NODE_SET_PROTOTYPE_METHOD(t, "doStart", DoStart);
            NODE_SET_PROTOTYPE_METHOD(t, "stop", Stop);

            target->Set(String::NewSymbol("Advertisement"), t->GetFunction());
        }

        bool
        DoStart(DNSServiceFlags flags, uint32_t interface_index,
                const char * name, const char * regtype, const char * domain,
                const char * host, uint16_t port, uint16_t txt_record_length,
                const void * txt_record)
        {
            if (ref_) return false;

            int status = DNSServiceRegister( &ref_, flags, interface_index,
                    name, regtype, domain, host, port, txt_record_length,
                    txt_record, & on_service_registered, this);
            if (kDNSServiceErr_NoError != status) {
                // XXX deal with error
                return false;
            }

            prepareSocket(ref_, &read_watcher_);

            Ref();

            return true;
        }

        bool
        Stop(Local<Value> exception = Local<Value>()) {
            HandleScope scope;
            ev_io_stop(EV_DEFAULT_ &read_watcher_);
            DNSServiceRefDeallocate(ref_);
            ref_ = NULL;
            if (exception.IsEmpty()) {
                Emit(stop_symbol, 0, NULL);
            } else {
                Emit(stop_symbol, 1, &exception);
            }

            Unref();
        }

        void
        Event(int revents) {
            if (EV_READ & revents) {
                if (kDNSServiceErr_NoError != DNSServiceProcessResult(ref_)) {
                    // XXX handle error
                }
            }
        }
    protected:

        static Handle<Value>
        New(const Arguments & args) {
            HandleScope scope;

            Advertisement * ad = new Advertisement();
            ad->Wrap(args.This());
            return args.This();
        }

        Advertisement() : EventEmitter() {
            ref_ = NULL;

            ev_init(&read_watcher_, io_event<Advertisement>);
            read_watcher_.data = this;
        }

        ~Advertisement() {
            assert(NULL == ref_);
        }

        void
        on_service_registered(DNSServiceFlags flags,
                DNSServiceErrorType errorCode, const char * name,
                const char * regtype, const char * domain)
        {
            const size_t argc = 5;
            Local<Value> args[argc];
            if (kDNSServiceErr_NoError == errorCode) {
                args[0] = Local<Value>::New(Null());
                args[1] = Integer::New(flags);
                args[2] = String::New(name);
                args[3] = String::New(regtype);
                args[4] = String::New(domain);
                Emit(ready_symbol, argc, args);
            } else {
                args[0] = buildException(errorCode);
                Emit(ready_symbol, 1, args);
            }
        }

        static
        Handle<Value>
        DoStart(const Arguments & args) {
            HandleScope scope;
            Advertisement * ad = ObjectWrap::Unwrap<Advertisement>(args.This());

            if ( 2 < args.Length() ||
                ! args[0]->IsString() || ! args[1]->IsInt32())
            {
                return ThrowException(Exception::Error(
                        String::New("argument mismatch.")));
            }

            String::Utf8Value regtype(args[0]->ToString());

            int raw_port = args[1]->ToInteger()->Int32Value();
            if (std::numeric_limits<uint16_t>::max() < raw_port) {
                return ThrowException(Exception::Error(
                        String::New("port number to large.")));
            }
            
            uint16_t port = static_cast<uint16_t>(raw_port);

            DNSServiceFlags flags = 0;
            uint32_t interface_index = 0;
            char * name = NULL;
            char * domain = NULL;
            char * host = NULL;
            uint16_t txt_record_length = 0;
            void * txt_record = NULL;
            
            bool r = ad->DoStart(flags, interface_index, name, *regtype,
                    domain, host, port, txt_record_length, txt_record);

            if (!r) {
                // XXX
                /*
                return ThrowException(Exception::Error(
                    String::New(ad->ErrorMessage())));
                */
            }
            return Undefined();
        }
        static
        Handle<Value>
        Stop(const Arguments & args) {
            HandleScope scope;
            Advertisement * ad = ObjectWrap::Unwrap<Advertisement>(args.This());

            bool r = ad->Stop();

            if ( ! r) {
                // XXX
                /*
                return ThrowException(Exception::Error(
                    String::New(ad->ErrorMessage())));
                */
            }
            return Undefined();
        }

    private:
        static
        void
        on_service_registered(DNSServiceRef /*sdRef*/, DNSServiceFlags flags, 
                DNSServiceErrorType errorCode, const char *name,
                const char *regtype, const char *domain, void *context )
        {
            Advertisement * ad = static_cast<Advertisement*>( context );
            ad->on_service_registered(flags, errorCode, name, regtype, domain);
        }

        ev_io read_watcher_;
        DNSServiceRef ref_;

        static Persistent<String> ready_symbol;
        static Persistent<String> stop_symbol;
};

Persistent<String> Advertisement::ready_symbol;
Persistent<String> Advertisement::stop_symbol;

//=== Service Browser ==========================================================

class Browser : public EventEmitter {
    public:
        static
        void
        Initialize(Handle<Object> target) {
            HandleScope scope;
            Local<FunctionTemplate> t = FunctionTemplate::New(New);

            t->Inherit(EventEmitter::constructor_template);
            t->InstanceTemplate()->SetInternalFieldCount(1);

            NODE_SET_PROTOTYPE_METHOD(t, "doStart", DoStart);

            target->Set(String::NewSymbol("Browser"), t->GetFunction());
        }

        bool
        DoStart(DNSServiceFlags flags, uint32_t interface_index,
                const char * regtype, const char * domain)
        {
            std::cout << "=== DoStart" << std::endl;
            if (ref_) return false;

            int status = DNSServiceBrowse( &ref_, flags, interface_index,
                    regtype, domain,
                    & on_service_changed, this);
            if (kDNSServiceErr_NoError != status) {
                // XXX deal with error
                return false;
            }

            prepareSocket(ref_, &read_watcher_);

            Ref();

            return true;
        }
        void
        Event(int revents) {
            if (EV_READ & revents) {
                if (kDNSServiceErr_NoError != DNSServiceProcessResult(ref_)) {
                    // XXX handle error
                }
            }
        }
    protected:

        static
        Handle<Value>
        New(const Arguments & args) {
            HandleScope scope;

            Browser * browser = new Browser();
            browser->Wrap(args.This());

            return args.This();
        }

        Browser() : EventEmitter() {
            ref_ = NULL;

            ev_init(&read_watcher_, io_event<Browser>);
            read_watcher_.data = this;
        }

        ~Browser() {
            assert(NULL == ref_);
        }

        static
        Handle<Value>
        DoStart(const Arguments & args) {
            HandleScope scope;
            Browser * browser = ObjectWrap::Unwrap<Browser>(args.This());

            if ( 1 > args.Length() || ! args[0]->IsString()) {
                return ThrowException(Exception::Error(
                        String::New("argument mismatch.")));
            }

            String::Utf8Value regtype(args[0]->ToString());

            DNSServiceFlags flags = 0;
            uint32_t interface_index = 0;
            char * domain = NULL;
            
            bool r = browser->DoStart(flags, interface_index, *regtype, domain);

            if (!r) {
                // XXX
                /*
                return ThrowException(Exception::Error(
                    String::New(ad->ErrorMessage())));
                */
            }
            return Undefined();
        }

        static
        void
        on_service_changed(DNSServiceRef /*sdRef*/, DNSServiceFlags flags,
                uint32_t interface_index, DNSServiceErrorType errorCode,
                const char * serviceName, const char * regtype,
                const char * replyDomain, void * context)
        {
            Browser * browser = static_cast<Browser*>(context);
            browser->on_service_changed(flags, interface_index, errorCode,
                    serviceName, regtype, replyDomain);
        }
        void
        on_service_changed(DNSServiceFlags flags, uint32_t interface_index,
                DNSServiceErrorType errorCode, const char * name,
                const char * regtype, const char * domain)
        {
        }
    private:
        DNSServiceRef ref_;
        ev_io read_watcher_;
};

//=== Service ==================================================================
} // end of anonymous namespace

extern "C" 
void
init (Handle<Object> target) {
    HandleScope scope;
    Advertisement::Initialize( target );
    Browser::Initialize( target );
}
