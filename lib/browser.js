var dns_sd = require('./dns_sd')
  , util   = require('util')
  , rst    = require('./resolver_sequence_tasks')

  , MDNSService = require('./mdns_service').MDNSService
  , makeServiceType = require('./service_type').makeServiceType
  ;

var Browser = exports.Browser = function Browser(serviceType, options) {
  MDNSService.call(this);
  var self = this;

  options = options || {};
  var flags = options.flags             || 0
    , ifaceIdx = options.interfaceIndex || 0
    , domain = options.domain           || null
    , context = options.context         || null
    ;

  self.resolverSequence = options.resolverSequence || Browser.defaultResolverSequence;

  self.serviceType = makeServiceType( serviceType );

  dns_sd.DNSServiceBrowse(self.serviceRef, flags, ifaceIdx,
      '' + self.serviceType, domain,
      self._onServiceChanged, context);
}
util.inherits(Browser, MDNSService);

Browser.prototype._onServiceChanged = function(sdRef, flags, ifaceIdx,
    errorCode, serviceName, serviceType, replyDomain, context)
{
  if (errorCode == dns_sd.kDNSServiceErr_NoError) {
    var self = this;
    if (self.serviceType.isWildcard()) {
      serviceType = serviceName + '.' + serviceType.split('.').shift();
      serviceName = null;
    }
    var service = {
        interfaceIndex: ifaceIdx
      , type: makeServiceType(serviceType)
      , replyDomain: replyDomain
      , flags: flags
    };
    if (serviceName) service.name = serviceName;
    if (context) service.context = context;
    

    if (flags & dns_sd.kDNSServiceFlagsAdd) {
      self.resolve(service, self.resolverSequence);
    } else {
      self.emit('serviceChanged', service);
      self.emit('serviceDown', service);
    }
  } else {
    self.emit('error', dns_sd.buildException(errorCode));
  }
}

Browser.prototype.resolve = function resolve(service, sequence) {
  var self = this
    , step = 0 
    , next = function next(error) {
        if (error) {
          self._onResolverDone(error);
          return;
        }
        if (sequence.length === step) {
          self._onResolverDone(undefined, service);
          return;
        }
        var f = sequence[step++];
        f(service, next);
      }
    ;
  next();
}

Browser.prototype._onResolverDone = function _onResolverDone(error, service) {
  if (error) {
    this.emit('error', error);
  } else {
    this.emit('serviceChanged', service);
    this.emit('serviceUp', service);
  }
}

Browser.defaultResolverSequence = [
  rst.DNSServiceResolve()
, 'DNSServiceGetAddrInfo' in dns_sd ? rst.DNSServiceGetAddrInfo() : rst.getaddrinfo()
, rst.makeAddressesUnique()
];
