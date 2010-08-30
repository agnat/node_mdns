
var MDNSService = require('./mdns_service').MDNSService;

exports.capi = require('./binding');

exports.Advertisement = require('./advertisement').Advertisement;
exports.createAdvertisement = function(regtype, port, options, callback) {
  return new exports.Advertisement(regtype, port, options, callback);
}

exports.Browser = require('./browser').Browser;
exports.createBrowser = function(regtype, options) {
  return new exports.Browser(regtype, options);
}

exports.DomainEnumerator = require('./domain_enumerator').DomainEnumerator;
exports.createDomainEnumerator = function(flags, interfaceIndex) {
  return new exports.DomainEnumerator(flags, interfaceIndex);
}

exports.RegType = require('./regtype').RegType;

function protocolHelper(transportProtocol) {
  return function() {
    return new exports.RegType(Array.prototype.slice.call(arguments),
        transportProtocol);
  }
}

exports.tcp = protocolHelper('tcp');
exports.udp = protocolHelper('udp');
