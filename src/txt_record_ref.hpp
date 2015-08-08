#ifndef NODE_MDNS_TXT_RECORD_REF_INCLUDED
#define NODE_MDNS_TXT_RECORD_REF_INCLUDED

namespace node_mdns {

class TxtRecordRef : public Nan::ObjectWrap {
    public:
        TxtRecordRef();
        ~TxtRecordRef();

        static void Initialize(v8::Handle<v8::Object> target);
        static NAN_METHOD(New);

        //inline bool IsInitialized() const { return ref_ != NULL; }

        static inline bool HasInstance(v8::Handle<v8::Value> value) {
            if ( ! value->IsObject() ) return false;
            v8::Local<v8::Object> object = value->ToObject();
            return Nan::New(constructor_template)->HasInstance( object );
        }

        TXTRecordRef & GetTxtRecordRef() { return ref_; }
        void SetTxtRecordRef(TXTRecordRef ref) { ref_ = ref; }

    private:
        TXTRecordRef ref_;

        static Nan::Persistent<v8::FunctionTemplate> constructor_template;
};

} // end of namespace node_mdns
#endif // NODE_MDNS_TXT_RECORD_REF_INCLUDED
