var capi = require('./binding'),
    util = require('util');

var MDNSService = require('./mdns_service').MDNSService;

function DomainEnumerator(flags, interfaceIndex) {
  MDNSService.call(this);
  var self = this;

  capi.dnsServiceEnumerateDomains(self.serviceRef, flags, interfaceIndex, 
      self._onEnumerate, null);
}
util.inherits(DomainEnumerator, MDNSService);
exports.DomainEnumerator = DomainEnumerator;

DomainEnumerator.prototype._onEnumerate = function(sdRef, flags, interfaceIndex, 
    errorCode, domain, context)
{
  error = capi.buildException(errorCode);
  if (error) {
    this.emit('error', error);
  } else {
    this.emit('domainChanged', domain, flags, interfaceIndex, context);
  }
}
