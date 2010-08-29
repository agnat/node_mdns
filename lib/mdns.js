
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

function RegType(types, transportProtocol) {
  var self = this;
  self.serviceType = types.shift();
  self.subtypes = types;
  self.transportProtocol = transportProtocol;
}

function _u(str) { return str[0] == '_' ? str : "_" + str; }

RegType.prototype.regtypeString = function() {
  if (this.subtypes.length > 0) {
    var primaryType = _u(this.serviceType) + "." + _u(this.transportProtocol);
    var subtypes = this.subtypes.map(function(t) { return _u(t) });
    subtypes.unshift(primaryType);
    return subtypes.join(',');
  } else {
    return _u(this.serviceType) + "." + _u(this.transportProtocol);
  }
}

RegType.prototype.toString = RegType.prototype.regtypeString;

function protocolHelper(transportProtocol) {
  return function() {
    return new RegType(Array.prototype.slice.call(arguments), transportProtocol);
  }
}

exports.tcp = protocolHelper('tcp');
exports.udp = protocolHelper('udp');
