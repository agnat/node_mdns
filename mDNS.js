binding = require('./binding');

//=== Advertisement ============================================================
var Advertisement = binding.Advertisement;
Advertisement.prototype.start = function() {
  return this.doStart("_" + this.type + "._" + this.protocol, this.port);
};

exports.createAdvertisement = function(type, port, protocol, ready_callback) {
  var ad = new Advertisement();
  ad.type = type;
  ad.port = port;
  ad.protocol = protocol;
  if (ready_callback) {
    ad.addListener('ready', ready_callback);
  }
  return ad;
};

//=== Service Browser ==========================================================

var Browser = binding.Browser;
Browser.prototype.start = function() {
  return this.doStart('_' + this.type + '._' + this.protocol);
}

exports.createBrowser = function(type, protocol) {
  var browser = new Browser();
  browser.type = type;
  browser.protocol = protocol;
  return browser;
};

//=== Resolver =================================================================

var Resolver = binding.Resolver;

Resolver.prototype.start = function(flags, interface_index, name, regtype, domain) {
  this.doStart(name, regtype, domain, interface_index, flags);
}

exports.createResolver = function() {
  return new Resolver();
}

//=== Constants ================================================================

exports.TCP = "tcp";
exports.UDP = "udp";

exports.ServiceAdd = binding.kDNSServiceFlagsAdd;
