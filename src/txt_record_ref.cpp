#include "mdns.hpp"

#include "txt_record_ref.hpp"

using namespace v8;

namespace node_mdns {

Persistent<FunctionTemplate> TxtRecordRef::constructor_template;

TxtRecordRef::TxtRecordRef() :
    ref_()
{
}

TxtRecordRef::~TxtRecordRef() {
    TXTRecordDeallocate( & ref_);
}

void
TxtRecordRef::Initialize(Handle<Object> target) {
    Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
    NanAssignPersistent(constructor_template, t);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    t->SetClassName(NanNew("TXTRecordRef"));

    target->Set(NanNew("TXTRecordRef"),
            t->GetFunction());
}

NAN_METHOD(TxtRecordRef::New) {
    NanScope();
    TxtRecordRef * o = new TxtRecordRef();
    o->Wrap(args.Holder());
    NanReturnValue(args.This());
}

} // end of namespace node_mdns
