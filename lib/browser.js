var dns_sd = require('./dns_sd')
  , util   = require('util')
  , rsi    = require('./resolver_sequence_items')
  ;

var MDNSService = require('./mdns_service').MDNSService
  , makeRegType = require('./regtype').makeRegType
  ;

function Browser(regtype, options) {
  MDNSService.call(this);
  var self = this;

  options = options || {};
  var flags = options.flags             || 0
    , ifaceIdx = options.interfaceIndex || 0
    , domain = options.domain           || null
    , context = options.context         || null
    ;

  self.resolverSequence = options.resolverSequence || Browser.defaultResolverSequence;

  self.regtype = makeRegType( regtype );

  dns_sd.DNSServiceBrowse(self.serviceRef, flags, ifaceIdx,
      '' + self.regtype, domain,
      self._onServiceChanged, context);
}
util.inherits(Browser, MDNSService);
exports.Browser = Browser;

Browser.prototype._onServiceChanged = function(sdRef, flags, ifaceIdx,
    errorCode, serviceName, regtype, replyDomain, context)
{
  if (errorCode == dns_sd.kDNSServiceErr_NoError) {
    var self = this;
    var info = {
        interfaceIndex: ifaceIdx
      , serviceName: serviceName
      , regtype: makeRegType(regtype)
      , replyDomain: replyDomain
      , flags: flags
    };
    if (context) info.context = context;
    

    if (flags & dns_sd.kDNSServiceFlagsAdd) {
      self._resolve(info);
    } else {
      self.emit('serviceChanged', info);
      self.emit('serviceDown', info);
    }
  } else {
    self.emit('error', dns_sd.buildException(errorCode));
  }
}

Browser.prototype._resolve = function resolve(info) {
  var self = this
    , step = 0 
    , next = function next(error) {
      if (error) {
        self._onResolverDone(error);
        return;
      }
      if (self.resolverSequence.length === step) {
        self._onResolverDone(undefined, info);
        return;
      }
      var f = self.resolverSequence[step++];
      f(info, next);
    }
    ;
  next();
}

Browser.prototype._onResolverDone = function _onResolverDone(error, info) {
  if (error) {
    this.emit('error', error);
  } else {
    this.emit('serviceChanged', info);
    this.emit('serviceUp', info);
  }
}

Browser.defaultResolverSequence = [
  rsi.DNSServiceResolve()
, 'DNSServiceGetAddrInfo' in dns_sd ? rsi.DNSServiceGetAddrInfo() : rsi.lookup()
];
