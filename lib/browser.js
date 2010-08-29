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
      interfaceIndex: ifaceIdx,
      serviceName: serviceName,
      regtype: regtype,
      replyDomain: replyDomain
    };
    if (context) info.context = context;
    
    self.emit('serviceChanged', info, flags);

    if (flags & capi.kDNSServiceFlagsAdd) {
      if (self.autoresolve) {
        var resolver = new Resolver(serviceName, regtype, replyDomain,
            {interfaceIndex: ifaceIdx}, function(err, resolverInfo, resolverFlags) {
              // XXX handle kDNSServiceFlagsMoreComing
              for (p in resolverInfo) {
                info[p] = resolverInfo[p];
              }
              if (err) {
                self.emit('error', err);
              } else {
                self.emit('serviceUp', resolverInfo, flags, resolverFlags);
              }
              resolver.stop();
            });
        resolver.start();
      } else {
        self.emit('serviceUp', info, flags);
      }
    } else {
      self.emit('serviceDown', info, flags);
    }
  } else {
    self.emit('error', capi.buildException(errorCode));
  }
}


