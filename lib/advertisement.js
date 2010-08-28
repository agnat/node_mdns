var capi = require('./binding'),
    sys  = require('sys');

var MDNSService = require('./mdns_service').MDNSService;

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

  var flags     = options.flags          || 0,
      ifaceIdx  = options.interfaceIndex || 0,
      name      = options.name           || null,
      domain    = options.domain         || null,
      host      = options.host           || null,
      txtRecord = options.txtRecord      || null,
      context   = options.context        || null;

  capi.DNSServiceRegister(self.serviceRef, flags, ifaceIdx, name, regtype,
      domain, host, port, txtRecord, self._onServiceRegistered, context);
}
sys.inherits(Advertisement, MDNSService)
exports.Advertisement = Advertisement;

Advertisement.prototype._onServiceRegistered = function(serviceRef, flags,
    errorCode, name, regtype, domain, context)
{
  var error = capi.buildException(errorCode);
  if (this.callback) {
    var info = {
      name: name,
      regtype: regtype,
      domain: domain,
      context: context
    };
    this.callback.call(this, error, info, flags);
  } else if (error) {
    this.emit('error', error);
  }
}
