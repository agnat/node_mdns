binding = require('./binding');

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

exports.createServiceBrowser = function() {
};

exports.TCP = "tcp";
exports.UDP = "udp";
