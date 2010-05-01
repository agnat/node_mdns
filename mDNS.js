binding = require('./binding');

var Service = binding.Service;

exports.createService = function(ready_callback) {
  var service = new Service();
  if (ready_callback) {
    service.addListener('ready', ready_callback);
  }
  return service;
}
