binding = require('./binding');

//=== Service ==================================================================
var Service = binding.Service;
Service.prototype.announce = function() {
  return this.doAnnounce("_" + this.name + "._" + this.protocol, this.port);
};

exports.createService = function(name, port, protocol, ready_callback) {
  var service = new Service();
  service.name = name;
  service.port = port;
  service.protocol = protocol;
  if (ready_callback) {
    service.addListener('ready', ready_callback);
  }
  return service;
};

//=== Service Browser ==========================================================

exports.createServiceBrowser = function() {
  var browser = new ServiceBrowser();
  return browser;
};

//=== Constants ================================================================

exports.TCP = "tcp";
exports.UDP = "udp";
