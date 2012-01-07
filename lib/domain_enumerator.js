var dns_sd = require('./dns_sd'),
    util = require('util');

var MDNSService = require('./mdns_service').MDNSService;

function DomainEnumerator(flags, interfaceIndex) {
  MDNSService.call(this);
  var self = this;

  dns_sd.dnsServiceEnumerateDomains(self.serviceRef, flags, interfaceIndex, 
      self._onEnumerate, null);
}
util.inherits(DomainEnumerator, MDNSService);
exports.DomainEnumerator = DomainEnumerator;

DomainEnumerator.prototype._onEnumerate = function(sdRef, flags, interfaceIndex, 
    errorCode, domain, context)
{
  error = dns_sd.buildException(errorCode);
  if (error) {
    this.emit('error', error);
  } else {
    this.emit('domainChanged', domain, flags, interfaceIndex, context);
  }
}
