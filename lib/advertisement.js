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

  self.callback = callback;
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

  dns_sd.DNSServiceRegister(self.serviceRef, flags, ifaceIdx, name,
      self.serviceType.toString(), domain, host, port, txtRecord,
      self._onServiceRegistered, context);
}
util.inherits(Advertisement, MDNSService)
exports.Advertisement = Advertisement;

Advertisement.prototype._onServiceRegistered =
  function(serviceRef, flags, errorCode, name, serviceType, domain, context)
{
  var error = dns_sd.buildException(errorCode);
  if (this.callback) {
    var service = {
        name:    name
      , type:    makeServiceType(serviceType)
      , domain:  domain
      , context: context
      , flags:   flags
    };
    this.callback.call(this, error, service);
  } 
  if (error) {
    this.emit('error', error);
  }
}

function objectToTXTRecord(o) {
  console.log('objectToTXTRecord');
  var record = new dns_sd.TXTRecordRef()
    , buffer = new Buffer(256)
    ;
  record.buffer = buffer; // Important: keep buffer alive while record is in use
  dns_sd.TXTRecordCreate(record, buffer);
  for (var p in o) {
    dns_sd.TXTRecordSetValue(record, p, (o[p] === undefined || o[p] === null) ? o[p] : '' + o[p]);
  }
  return record;
}

