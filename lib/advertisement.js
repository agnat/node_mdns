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

  serviceType = makeServiceType(serviceType);

  var flags     = options.flags          || 0
    , ifaceIdx  = options.interfaceIndex || 0
    , name      = options.name           || null
    , domain    = options.domain         || null
    , host      = options.host           || null
    , txtRecord = options.txtRecord      || null
    , context   = options.context        || null
    ;

  if (txtRecord && typeof txtRecord === 'object' &&
      ! (txtRecord instanceof dns_sd.TXTRecordRef || Buffer.isBuffer(txtRecord)))
  {
    txtRecord = objectToTXTRecord(txtRecord);
  }

  function on_service_registered(serviceRef, flags, errorCode, name,
      serviceType, domain, context)
  {
    var error = dns_sd.buildException(errorCode);
    if (callback) {
      callback.call(self, error, {
          name:    name
        , type:    makeServiceType(serviceType)
        , domain:  domain
        , context: context
        , flags:   flags
      });
    } 
    if (error) {
      self.emit('error', error);
    }
  }

  dns_sd.DNSServiceRegister(self.serviceRef, flags, ifaceIdx, name,
      '' + serviceType, domain, host, port, txtRecord, on_service_registered,
      context);
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

