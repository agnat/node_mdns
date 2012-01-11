
var MDNSService = require('./mdns_service').MDNSService
  , regtype = require('./regtype');

exports.dns_sd = require('./dns_sd');

exports.Advertisement = require('./advertisement').Advertisement;
exports.createAdvertisement = function createAdvertisement(regtype, port, options, callback) {
  return new exports.Advertisement(regtype, port, options, callback);
}

exports.Browser = require('./browser').Browser;
exports.createBrowser = function createBrowser(regtype, options) {
  return new exports.Browser(regtype, options);
}

exports.browseThemAll = function browseThemAll(options) {
  options = options || {}
  options.resolverSequence = options.resolverSequence || [];
  return exports.createBrowser(regtype.RegType.wildcard, options);
}

exports.RegType = regtype.RegType;
exports.makeRegType = regtype.makeRegType;

function protocolHelper(protocol) {
  return function() {
    var args = Array.prototype.slice.call(arguments);
    if (regtype.isProtocol(args[1])) {
      throw new Error("duplicate protocol '" + args[1] + "' in arguments");
    }
    args.splice(1,0, protocol);
    return exports.makeRegType.apply(this, args);
  }
}

exports.tcp = protocolHelper('tcp');
exports.udp = protocolHelper('udp');

exports.dns_sd.exportConstants(exports);

exports.rst = require('./resolver_sequence_tasks');
