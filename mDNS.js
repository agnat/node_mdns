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

//=== Constants ================================================================

exports.TCP = "tcp";
exports.UDP = "udp";
