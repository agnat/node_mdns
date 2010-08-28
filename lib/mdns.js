var capi   = require('./binding'),
    sys    = require('sys'),
    events = require('events');

exports.capi = capi;

//==============================================================================
function MDNSService() {
  events.EventEmitter.call(this);
  var self = this;

  self.serviceRef = new capi.DNSServiceRef();
  self.watcher = new process.IOWatcher();
//  self.watcher.host = self; // TODO: Find out what this is for ...
  self.watcher.callback = function() {
    capi.DNSServiceProcessResult.call(self, self.serviceRef);  
  };
}
sys.inherits(MDNSService, events.EventEmitter);
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
    capi.DNSServiceRefDeallocate(this.serviceRef);
    this.serviceRef = null;
    this._watcherStarted = false;
  }
}

//==============================================================================
function Advertisement(regtype, port, options, callback) {
  MDNSService.call(this);
  var self = this;

  if ( ! callback) {
    if (typeof options === 'function') {
      callback = options;
      options = {};
    }
  }
  options = options || {};

  self.callback = callback;

  var flags     = options.flags          || 0,
      ifaceIdx  = options.interfaceIndex || 0,
      name      = options.name           || null,
      domain    = options.domain         || null,
      host      = options.host           || null,
      txtRecord = options.txtRecord      || null,
      context   = options.context        || null;

  capi.DNSServiceRegister(self.serviceRef, flags, ifaceIdx, name, regtype,
      domain, host, port, txtRecord, self._onServiceRegistered, context);
}
sys.inherits(Advertisement, MDNSService)
exports.Advertisement = Advertisement;

Advertisement.prototype._onServiceRegistered = function(serviceRef, flags,
    errorCode, name, regtype, domain, context)
{
  var error = capi.buildException(errorCode);
  if (this.callback) {
    var info = {
      name: name,
      regtype: regtype,
      domain: domain,
      context: context
    };
    this.callback.call(this, error, info, flags);
  } else if (error) {
    this.emit('error', error);
  }
}

exports.createAdvertisement = function(regtype, port, options, callback) {
  return new Advertisement(regtype, port, options, callback);
}

//==============================================================================

function Browser(regtype, options) {
  MDNSService.call(this);
  var self = this;

  options = options || {};
  var flags = options.flags             || 0,
      ifaceIdx = options.interfaceIndex || 0,
      domain = options.domain           || null,
      context = options.context         || null;
  
  capi.DNSServiceBrowse(self.serviceRef, flags, ifaceIdx, regtype, domain,
      self._onServiceChanged, context);
}
sys.inherits(Browser, MDNSService);
exports.Browser = Browser;

Browser.prototype._onServiceChanged = function(sdRef, flags, interfaceIndex,
    errorCode, serviceName, regtype, replyDomain, context)
{
  if (errorCode == capi.kDNSServiceErr_NoError) {
    var info = {
      flags: flags,
      interfaceIndex: interfaceIndex,
      serviceName: serviceName,
      regtype: regtype,
      replyDomain: replyDomain,
      context: context
    };
    this.emit('serviceChanged', info);
    if (flags & capi.kDNSServiceFlagsAdd) {
      this.emit('serviceUp', info);
    } else {
      this.emit('serviceDown', info);
    }
  } else {
    this.emit('error', capi.buildException(errorCode));
  }
}

exports.createBrowser = function(regtype, options) {
  return new Browser(regtype, options);
}

