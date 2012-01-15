var dns_sd = require('./dns_sd')
  , util = require('util')
  ;

var MDNSService = require('./mdns_service').MDNSService
  , makeServiceType = require('./service_type').makeServiceType
  ;

function Advertisement(serviceType, port, options, callback) {
  MDNSService.call(this);
  var self = this;

  if ( ! callback) {
    if (typeof options === 'function') {
      callback = options;
      options = {};
    }
  }
  options = options || {};

  self.serviceType = makeServiceType( serviceType );

  var flags     = options.flags          || 0
    , ifaceIdx  = options.interfaceIndex || 0
    , name      = options.name           || null
    , domain    = options.domain         || null
    , host      = options.host           || null
    , txtRecord = options.txtRecord      || null
    , context   = options.context        || null
    ;

  if (txtRecord) {
    if (txtRecord instanceof dns_sd.TXTRecordRef) {
      console.log('txtRecord is TXTRecordRef')
    } else if (Buffer.isBuffer(txtRecord)) {
      console.log('txtRecord is buffer')
    } else if (typeof txtRecord === 'object') {
      txtRecord = objectToTXTRecord(txtRecord);
    } else {
      // let it fail
    }
  }

  function on_service_registered(serviceRef, flags, errorCode, name,
      serviceType, domain, context)
  {
    var error = dns_sd.buildException(errorCode);
    if (callback) {
      var service = {
          name:    name
        , type:    makeServiceType(serviceType)
        , domain:  domain
        , context: context
        , flags:   flags
      };
      callback.call(self, error, service);
    } 
    if (error) {
      self.emit('error', error);
    }
  }

  dns_sd.DNSServiceRegister(self.serviceRef, flags, ifaceIdx, name,
      '' + self.serviceType, domain, host, port, txtRecord,
      on_service_registered, context);
}
util.inherits(Advertisement, MDNSService)
exports.Advertisement = Advertisement;

function objectToTXTRecord(o) {
  var record = new dns_sd.TXTRecordRef()
    , value
    ;
  record.buffer = new Buffer(256);
  dns_sd.TXTRecordCreate(record, record.buffer);
  for (var p in o) {
    value = (o[p] === undefined || o[p] === null) ? o[p] : '' + o[p];
    dns_sd.TXTRecordSetValue(record, p, value);
  }
  return record;
}

