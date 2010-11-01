var capi   = require('./binding'),
    util   = require('util'),
    events = require('events');

var IOWatcher = process.binding('io_watcher').IOWatcher;

var RegType = require('./regtype').RegType;

function MDNSService() {
  events.EventEmitter.call(this);
  var self = this;

  self.serviceRef = new capi.DNSServiceRef();
  self.watcher = new IOWatcher();
  self.watcher.host = self; // TODO: Find out what this is for ...
  self.watcher.callback = function() {
    capi.dnsServiceProcessResult.call(self, self.serviceRef);  
  };
}
util.inherits(MDNSService, events.EventEmitter);
exports.MDNSService = MDNSService;

MDNSService.prototype.start = function() {
  if (this._watcherStarted) {
    throw new Error("mdns service already started");
  }
  this.watcher.set(this.serviceRef.fd, true, false);
  this.watcher.start();
  this._watcherStarted = true;
}

MDNSService.prototype.stop = function() {
  if (this._watcherStarted) {
    this.watcher.stop();
    capi.dnsServiceRefDeallocate(this.serviceRef);
    this.serviceRef = null;
    this._watcherStarted = false;
  }
}

MDNSService.prototype._buildRegType = function( r ) {
  if ( r instanceof RegType ) {
    return r;
  }
  return new RegType( r );
}
