#include <dns_sd.h>
#include <errno.h>
#include <fcntl.h>
#include <node.h>
#include <node_events.h>

#include <iostream>

using namespace v8;
using namespace node;

namespace {

static Persistent<String> ready_symbol;
static Persistent<String> close_symbol;
#define READY_STATE_SYMBOL String::NewSymbol("readyState")

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
            close_symbol = NODE_PSYMBOL("close");

            NODE_SET_PROTOTYPE_METHOD(t, "announce", Announce);
            NODE_SET_PROTOTYPE_METHOD(t, "close",    Close);

            /*
            t->PrototypeTemplate()->SetAccessor(READY_STATE_SYMBOL,
                    ReadyStateGetter);
            */

            target->Set(String::NewSymbol("Service"), t->GetFunction());
        }

        bool
        Announce() {
            if (ref_) return false;

            int interface_index = 0;
            int flags = 0;
            char * name = NULL;
            char * regtype = "_ttt._tcp";
            char * domain = NULL;
            char * host = NULL;
            uint16_t port = 4321;
            uint16_t txt_record_length = 0;
            char * txt_record = NULL;

            int status = DNSServiceRegister( &ref_, flags, interface_index, name,
                    regtype, domain, host, port, txt_record_length, txt_record,
                    & on_service_registered, this);
            if (kDNSServiceErr_NoError != status) {
                // XXX deal with error
                return false;
            }
            int fd = DNSServiceRefSockFD(ref_);
            if (-1 == fd) {
                // XXX deal with error
                return false;
            }

            if (-1 == fcntl(fd, F_SETFL, O_NONBLOCK) ||
                -1 == fcntl(fd, F_SETFD, FD_CLOEXEC))
            {
                return false;
            }

            ev_io_set(&read_watcher_, fd, EV_READ);
            ev_io_start(EV_DEFAULT_ &read_watcher_);
            Ref();

            return true;
        }

        bool
        Close(Local<Value> exception = Local<Value>()) {
            HandleScope scope;
            ev_io_stop(EV_DEFAULT_ &read_watcher_);
            DNSServiceRefDeallocate(ref_);
            ref_ = NULL;
            if (exception.IsEmpty()) {
                Emit(close_symbol, 0, NULL);
            } else {
                Emit(close_symbol, 1, &exception);
            }
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

            ev_init(&read_watcher_, io_event);
            read_watcher_.data = this;
        }

         void on_ready(int flags, DNSServiceErrorType errorCode, 
                const char * name, const char * regtype, const char * domain)
         {
             Emit(ready_symbol, 0, NULL);
         }

        static
        Handle<Value>
        Announce(const Arguments & args) {
            HandleScope scope;
            Service * service = ObjectWrap::Unwrap<Service>(args.This());

            bool r = service->Announce();

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
        Close(const Arguments & args) {
            HandleScope scope;
            Service * service = ObjectWrap::Unwrap<Service>(args.This());

            bool r = service->Close();

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
        io_event(EV_P_ ev_io *w, int revents) {
            Service * service = static_cast<Service*>(w->data);
            service->Event(revents);
        }

        static
        void
        on_service_registered(DNSServiceRef sdRef, DNSServiceFlags flags, 
                DNSServiceErrorType errorCode, const char *name,
                const char *regtype, const char *domain, void *context )
        {
            Service * service = static_cast<Service*>( context );
            service->on_ready(flags, errorCode, name, regtype, domain);
        }

        ev_io read_watcher_;
        DNSServiceRef ref_;
};

} // end of anonymous namespace

extern "C" 
void
init (Handle<Object> target) {
    HandleScope scope;
    Service::Initialize( target );
}
