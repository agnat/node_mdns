#include "mdns.hpp"

#include "txt_record_ref.hpp"

using namespace v8;

namespace node_mdns {

Nan::Persistent<FunctionTemplate> TxtRecordRef::constructor_template;

TxtRecordRef::TxtRecordRef() :
    ref_()
{
}

TxtRecordRef::~TxtRecordRef() {
    TXTRecordDeallocate( & ref_);
}

void
TxtRecordRef::Initialize(Handle<Object> target) {
    Local<FunctionTemplate> t = Nan::New<FunctionTemplate>(New);
    constructor_template.Reset(t);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    t->SetClassName(Nan::New("TXTRecordRef").ToLocalChecked());

    Nan::Set(target, Nan::New("TXTRecordRef").ToLocalChecked(),
            Nan::GetFunction(t).ToLocalChecked());
}

NAN_METHOD(TxtRecordRef::New) {
    TxtRecordRef * o = new TxtRecordRef();
    o->Wrap(info.Holder());
    info.GetReturnValue().Set(info.This());
}

} // end of namespace node_mdns
