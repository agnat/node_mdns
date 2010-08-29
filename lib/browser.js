var capi = require('./binding'),
    sys  = require('sys');

var MDNSService = require('./mdns_service').MDNSService;
var Resolver = require('./resolver').Resolver;

function Browser(regtype, options) {
  MDNSService.call(this);
  var self = this;

  options = options || {};
  var flags = options.flags             || 0,
      ifaceIdx = options.interfaceIndex || 0,
      domain = options.domain           || null,
      context = options.context         || null;

  self.autoresolve = options.autoResolve || true;
  
  capi.DNSServiceBrowse(self.serviceRef, flags, ifaceIdx, regtype, domain,
      self._onServiceChanged, context);
}
sys.inherits(Browser, MDNSService);
exports.Browser = Browser;

Browser.prototype._onServiceChanged = function(sdRef, flags, ifaceIdx,
    errorCode, serviceName, regtype, replyDomain, context)
{
  if (errorCode == capi.kDNSServiceErr_NoError) {
    var self = this;
    var info = {
      flags: flags,
      interfaceIndex: ifaceIdx,
      serviceName: serviceName,
      regtype: regtype,
      replyDomain: replyDomain,
      context: context
    };
    self.emit('serviceChanged', info);
    if (flags & capi.kDNSServiceFlagsAdd) {
      if (self.autoresolve) {
        var resolver = new Resolver(serviceName, regtype, replyDomain,
            {interfaceIndex: ifaceIdx, 'flags': 0}, function(err, resolvedInfo) {
              self.emit('serviceUp', info); // XXX merge resolver info
              resolver.stop();
            });
        resolver.start();
      } else {
        self.emit('serviceUp', info);
      }
    } else {
      self.emit('serviceDown', info);
    }
  } else {
    self.emit('error', capi.buildException(errorCode));
  }
}


