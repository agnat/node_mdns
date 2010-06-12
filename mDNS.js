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

function resolve(service_info, handler) {
  var resolver = new Resolver();
  resolver.addListener('resolved', function(error, resolver_info) {
    for (var property in resolver_info) {
      service_info[property] = resolver_info[property];
    }
    handler(service_info);
    resolver.stop();
  });
  resolver.start(service_info)
}

var Browser = binding.Browser;
Browser.prototype.start = function() {
  return this.doStart('_' + this.type + '._' + this.protocol);
}

exports.createBrowser = function(type, protocol) {
  var browser = new Browser();
  browser.type = type;
  browser.protocol = protocol;
  browser.addListener('changed', function(err, flags, info) {
    if (flags & binding.kDNSServiceFlagsAdd) {
      resolve(info, function(resolver_info) {
        browser.emit('serviceUp', resolver_info);
      });
    } else {
      browser.emit('serviceDown', info);
    }
  });
  return browser;
};

//=== Resolver =================================================================

var Resolver = binding.Resolver;

Resolver.prototype.start = function(service_info) {
  this.doStart(service_info.name, service_info.regtype, service_info.domain,
      service_info.interface_index, 0);
}

exports.resolve = resolve;
//=== Constants ================================================================

exports.TCP = "tcp";
exports.UDP = "udp";

