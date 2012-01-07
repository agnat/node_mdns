var capi = require('./binding'),
    util = require('util');

var MDNSService = require('./mdns_service').MDNSService;

function Resolver(name, regtype, domain, options, callback) {
  MDNSService.call(this);
  var self = this;

  if ( ! callback) {
    if (typeof options === 'function') {
      callback = options;
      options = {};
    }
  }
  options = options || {};

  var flags    = options.flags          || 0
    , ifaceIdx = options.interfaceIndex || 0
    , context  = options.context        || null
    ;

  self.callback = callback

  capi.dnsServiceResolve(self.serviceRef, flags, ifaceIdx, name, '' + regtype,
      domain, self._onResolve, context);
}

util.inherits(Resolver, MDNSService);
exports.Resolver = Resolver;

Resolver.prototype._onResolve = function(sdRef, flags, interfaceIndex,
    errorCode, fullname, hosttarget, port, txtRecord, context)
{
  var self = this
    , error = capi.buildException(errorCode)
    , info = {}
    ;
  // XXX handle flags
  if (interfaceIndex) info.interfaceIndex = interfaceIndex;
  if (fullname)       info.fullname = fullname;
  if (hosttarget)     info.host = hosttarget;
  if (port)           info.port = port;
  if (txtRecord)      info.txtRecord = txtRecord;
  if (context)        info.context   = context;

  self.callback(error, info, flags); // XXX flags
}
