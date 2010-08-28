var capi   = require('./binding'),
    sys     = require('sys');

//==============================================================================
function MDNSService() {
  var self = this;
  self.serviceRef = new capi.DNSServiceRef();
  self.watcher = new process.IOWatcher();
//  self.watcher.host = self; // TODO: Find out what this is for ...
  self.watcher.callback = function() {
    capi.DNSServiceProcessResult.call(self, self.serviceRef);  
  };
}
exports.MDNSService = MDNSService;

MDNSService.prototype.start = function() {
  if ( ! this._watcherStarted) {
    this.watcher.set(this.serviceRef.fd, true, false);
    this.watcher.start();
    this._watcherStarted = true;
  }
}

MDNSService.prototype.stop = function() {
  if (this._watcherStarted) {
    this.watcher.stop();
  }
}

//==============================================================================
function Advertisement(regtype, port, options, callback) {
  var self = this;
  MDNSService.call(self);

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

  capi.DNSServiceRegister(self.serviceRef, flags, ifaceIdx, name, regtype,
      domain, host, port, txtRecord, callback);
}
sys.inherits(Advertisement, MDNSService)
exports.Advertisement = Advertisement;

exports.createAdvertisement = function(regtype, port, options, callback) {
  return new Advertisement(regtype, port, options, callback);
}

//==============================================================================
function Browser() {
  var self = this;
  MDNSService.call(self);

}


