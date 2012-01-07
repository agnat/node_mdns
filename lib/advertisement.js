var dns_sd = require('./dns_sd')
  , util = require('util')
  ;

var MDNSService = require('./mdns_service').MDNSService
  , makeRegType = require('./regtype').makeRegType
  ;

function Advertisement(regtype, port, options, callback) {
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
  self.regtype = makeRegType( regtype );

  var flags     = options.flags          || 0
    , ifaceIdx  = options.interfaceIndex || 0
    , name      = options.name           || null
    , domain    = options.domain         || null
    , host      = options.host           || null
    , txtRecord = options.txtRecord      || null
    , context   = options.context        || null
    ;

  dns_sd.DNSServiceRegister(self.serviceRef, flags, ifaceIdx, name,
      self.regtype.toString(), domain, host, port, txtRecord,
      self._onServiceRegistered, context);
}
util.inherits(Advertisement, MDNSService)
exports.Advertisement = Advertisement;

Advertisement.prototype._onServiceRegistered = function(serviceRef, flags,
    errorCode, name, regtype, domain, context)
{
  var error = dns_sd.buildException(errorCode);
  if (this.callback) {
    var info = {
        name:    name
      , regtype: makeRegType(regtype)
      , domain:  domain
      , context: context
      , flags:   flags
    };
    this.callback.call(this, error, info);
  } 
  if (error) {
    this.emit('error', error);
  }
}

