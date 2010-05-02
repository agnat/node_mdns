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


//=== Service ==================================================================
class Service : public EventEmitter {
    public:
        static
        void
        Initialize(Handle<Object> target) {
            HandleScope scope;
            Local<FunctionTemplate> t = FunctionTemplate::New(New);

            t->Inherit(EventEmitter::constructor_template);
            t->InstanceTemplate()->SetInternalFieldCount(1);

            ready_symbol = NODE_PSYMBOL("ready");
            discontinue_symbol = NODE_PSYMBOL("discontinue");

            NODE_SET_PROTOTYPE_METHOD(t, "doAnnounce", DoAnnounce);
            NODE_SET_PROTOTYPE_METHOD(t, "discontinue", Discontinue);

            target->Set(String::NewSymbol("Service"), t->GetFunction());
        }

        bool
        DoAnnounce(int flags, int interface_index, const char * name,
                const char * regtype, const char * domain, const char * host,
                uint16_t port, uint16_t txt_record_length,
                const void * txt_record)
        {
            if (ref_) return false;

            int status = DNSServiceRegister( &ref_, flags, interface_index, name,
                    regtype, domain, host, port, txt_record_length, txt_record,
                    & on_service_registered, this);
            if (kDNSServiceErr_NoError != status) {
                // XXX deal with error
                return false;
            }

            prepareSocket(ref_, &read_watcher_);

            Ref();

            return true;
        }

        bool
        Discontinue(Local<Value> exception = Local<Value>()) {
            HandleScope scope;
            ev_io_stop(EV_DEFAULT_ &read_watcher_);
            DNSServiceRefDeallocate(ref_);
            ref_ = NULL;
            if (exception.IsEmpty()) {
                Emit(discontinue_symbol, 0, NULL);
            } else {
                Emit(discontinue_symbol, 1, &exception);
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

            Service * service = new Service();
            service->Wrap(args.This());
            return args.This();
        }

        Service() : EventEmitter() {
            ref_ = NULL;

            ev_init(&read_watcher_, io_event<Service>);
            read_watcher_.data = this;
        }

        ~Service() {
            assert(NULL == ref_);
        }

        void
        on_service_registered(int flags, DNSServiceErrorType errorCode, 
                    const char * name, const char * regtype, const char * domain)
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
        DoAnnounce(const Arguments & args) {
            HandleScope scope;
            Service * service = ObjectWrap::Unwrap<Service>(args.This());

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

            int flags = 0;
            int interface_index = 0;
            char * name = NULL;
            char * domain = NULL;
            char * host = NULL;
            uint16_t txt_record_length = 0;
            void * txt_record = NULL;
            
            bool r = service->DoAnnounce(flags, interface_index, name, *regtype,
                    domain, host, port, txt_record_length, txt_record);

            if (!r) {
                // XXX
                /*
                return ThrowException(Exception::Error(
                    String::New(service->ErrorMessage())));
                */
            }
            return Undefined();
        }
        static
        Handle<Value>
        Discontinue(const Arguments & args) {
            HandleScope scope;
            Service * service = ObjectWrap::Unwrap<Service>(args.This());

            bool r = service->Discontinue();

            if ( ! r) {
                // XXX
                /*
                return ThrowException(Exception::Error(
                    String::New(service->ErrorMessage())));
                */
            }
            return Undefined();
        }

    private:
        static
        void
        on_service_registered(DNSServiceRef sdRef, DNSServiceFlags flags, 
                DNSServiceErrorType errorCode, const char *name,
                const char *regtype, const char *domain, void *context )
        {
            Service * service = static_cast<Service*>( context );
            service->on_service_registered(flags, errorCode, name, regtype, domain);
        }

        ev_io read_watcher_;
        DNSServiceRef ref_;

        static Persistent<String> ready_symbol;
        static Persistent<String> discontinue_symbol;
};

Persistent<String> Service::ready_symbol;
Persistent<String> Service::discontinue_symbol;

//=== Service Browser ==========================================================

class ServiceBrowser : public EventEmitter {
    public:
        static
        void
        Initialize(Handle<Object> target) {
            HandleScope scope;
            Local<FunctionTemplate> t = FunctionTemplate::New(New);

            t->Inherit(EventEmitter::constructor_template);
            t->InstanceTemplate()->SetInternalFieldCount(1);

            target->Set(String::NewSymbol("ServiceBrowser"), t->GetFunction());
        }

        void
        Event(int revents) {
            if (EV_READ & revents) {
            }
        }
    protected:

        static Handle<Value>
        New(const Arguments & args) {
            HandleScope scope;

            ServiceBrowser * browser = new ServiceBrowser();
            browser->Wrap(args.This());
            return args.This();
        }

        ServiceBrowser() : EventEmitter() {
            ref_ = NULL;

            ev_init(&read_watcher_, io_event<ServiceBrowser>);
            read_watcher_.data = this;
        }

        ~ServiceBrowser() {
            assert(NULL == ref_);
        }
    private:
        DNSServiceRef ref_;
        ev_io read_watcher_;
};
} // end of anonymous namespace

extern "C" 
void
init (Handle<Object> target) {
    HandleScope scope;
    Service::Initialize( target );
    ServiceBrowser::Initialize( target );
}
