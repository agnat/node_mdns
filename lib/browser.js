var capi = require('./binding'),
    sys  = require('sys');

var MDNSService = require('./mdns_service').MDNSService;

function Browser(regtype, options) {
  MDNSService.call(this);
  var self = this;

  options = options || {};
  var flags = options.flags             || 0,
      ifaceIdx = options.interfaceIndex || 0,
      domain = options.domain           || null,
      context = options.context         || null;
  
  capi.DNSServiceBrowse(self.serviceRef, flags, ifaceIdx, regtype, domain,
      self._onServiceChanged, context);
}
sys.inherits(Browser, MDNSService);
exports.Browser = Browser;

Browser.prototype._onServiceChanged = function(sdRef, flags, interfaceIndex,
    errorCode, serviceName, regtype, replyDomain, context)
{
  if (errorCode == capi.kDNSServiceErr_NoError) {
    var info = {
      flags: flags,
      interfaceIndex: interfaceIndex,
      serviceName: serviceName,
      regtype: regtype,
      replyDomain: replyDomain,
      context: context
    };
    this.emit('serviceChanged', info);
    if (flags & capi.kDNSServiceFlagsAdd) {
      this.emit('serviceUp', info);
    } else {
      this.emit('serviceDown', info);
    }
  } else {
    this.emit('error', capi.buildException(errorCode));
  }
}


