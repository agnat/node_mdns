
var MDNSService = require('./mdns_service').MDNSService
  , st = require('./service_type');

exports.dns_sd = require('./dns_sd');

exports.Advertisement = require('./advertisement').Advertisement;
exports.createAdvertisement = function createAdvertisement(serviceType, port, options, callback) {
  return new exports.Advertisement(serviceType, port, options, callback);
}

exports.Browser = require('./browser').Browser;
exports.createBrowser = function createBrowser(serviceType, options) {
  return new exports.Browser(serviceType, options);
}

exports.browseThemAll = function browseThemAll(options) {
  options = options || {}
  options.resolverSequence = options.resolverSequence || [];
  return exports.createBrowser(st.ServiceType.wildcard, options);
}

exports.ServiceType = st.ServiceType;
exports.makeServiceType = st.makeServiceType;

function protocolHelper(protocol) {
  return function() {
    var args = Array.prototype.slice.call(arguments);
    if (st.isProtocol(args[1])) {
      throw new Error("duplicate protocol '" + args[1] + "' in arguments");
    }
    args.splice(1,0, protocol);
    return exports.makeServiceType.apply(this, args);
  }
}

exports.tcp = protocolHelper('tcp');
exports.udp = protocolHelper('udp');

exports.dns_sd.exportConstants(exports);

exports.rst = require('./resolver_sequence_tasks');
