
var capi   = require('./binding'),
    sys     = require('sys');

function Advertisement(regtype, port, options, callback) {
  var self = this;
  if ( ! callback) {
    if (typeof options === 'function') {
      callback = options;
      options = {};
    }
  }
  options = options || {};

  var flags     = options.flags          || 0,
      ifaceIdx  = options.interfaceIndex || 0,
      name      = options.name           || null,
      domain    = options.domain         || null,
      host      = options.host           || null,
      txtRecord = options.txtRecord      || null;

  self.sdRef = new capi.DNSServiceRef();

  capi.DNSServiceRegister(self.sdRef, flags, ifaceIdx, name, regtype,
      domain, host, port, txtRecord, callback);

  self.watcher = new process.IOWatcher();
  self.watcher.host = self;
  self.watcher.callback = function() {
    capi.DNSServiceProcessResult(self.sdRef);  
  };
}

Advertisement.prototype.start = function() {
  if ( ! this._watcherStarted) {
    var fd = capi.DNSServiceRefSockFD(this.sdRef);
    this.watcher.set(fd, true, false);
    this.watcher.start();
    this._watcherStarted = true;
  }
}

exports.Advertisement = Advertisement;
exports.createAdvertisement = function(regtype, port, options, callback) {
  return new Advertisement(regtype, port, options, callback);
}
