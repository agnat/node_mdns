var capi = require('./binding')
  , util = require('util')
  ;

var MDNSService = require('./mdns_service').MDNSService
  , Resolver = require('./resolver').Resolver
  , makeRegType = require('./regtype').makeRegType
  ;

function Browser(regtype, options) {
  MDNSService.call(this);
  var self = this;

  options = options || {};
  var flags = options.flags             || 0
    , ifaceIdx = options.interfaceIndex || 0
    , domain = options.domain           || null
    , context = options.context         || null
    ;

  self.resolverStack = options.resolverStack || Browser.defaultResolverStack;

  self.regtype = makeRegType( regtype );

  capi.dnsServiceBrowse(self.serviceRef, flags, ifaceIdx,
      '' + self.regtype, domain,
      self._onServiceChanged, context);
}
util.inherits(Browser, MDNSService);
exports.Browser = Browser;

Browser.prototype._onServiceChanged = function(sdRef, flags, ifaceIdx,
    errorCode, serviceName, regtype, replyDomain, context)
{
  if (errorCode == capi.kDNSServiceErr_NoError) {
    var self = this;
    var info = {
        interfaceIndex: ifaceIdx
      , serviceName: serviceName
      , regtype: makeRegType(regtype)
      , replyDomain: replyDomain
      , flags: flags
    };
    if (context) info.context = context;
    

    if (flags & capi.kDNSServiceFlagsAdd) {
      self._resolve(info);
    } else {
      self.emit('serviceChanged', info);
      self.emit('serviceDown', info);
    }
  } else {
    self.emit('error', capi.buildException(errorCode));
  }
}

Browser.prototype._resolve = function resolve(info) {
  var self = this
    , level = 0 
    , next = function next(error) {
      if (error) {
        self._onResolverDone(error);
        return;
      }
      if (self.resolverStack.length === level) {
        self._onResolverDone(undefined, info);
        return;
      }
      var f = self.resolverStack[level++];
      f(info, next);
    }
    ;
  next();
}

Browser.prototype._onResolverDone = function _onResolverDone(error, info) {
  if (error) {
    this.emit('error', error);
  } else {
    this.emit('serviceChanged', info);
    this.emit('serviceUp', info);
  }
}

var ServiceResolve = function ServiceResolve() {
  return function doServiceResolve(info, next) {
    var resolver = new Resolver(info.serviceName, info.regtype, info.replyDomain,
        {interfaceIndex: info.interfaceIndex}, function(err, resolverInfo, resolverFlags) {
          if (err) {
            next(err);
            return;
          }
          // XXX handle kDNSServiceFlagsMoreComing
          for (p in resolverInfo) {
            info[p] = resolverInfo[p];
          }
          resolver.stop();
          next();
        });
    resolver.start();
  };
}

var GetAddrInfo = function GetAddrInfo() {
  return function doGetAddrInfo(info, next) {
    var addressInfoGetter = new MDNSService();
    var addresses = [];
    capi.dnsServiceGetAddrInfo(addressInfoGetter.serviceRef, 0, 0, 0, info.host,
        function(sdRef, adrFlags, adrIface, errorCode, hostname, address, context) {
          var error = capi.buildException(errorCode);
          if (error) {
            next(error);
            addressInfoGetter.stop()
            return;
          } else {
            if (adrIface === info.interfaceIndex) {
              addresses.push(address);
            }
            if ( ! (capi.kDNSServiceFlagsMoreComing & adrFlags)) {
              info.addresses = addresses;
              addressInfoGetter.stop()
              next();
            }
          }
        }, null);
    addressInfoGetter.start();
  };
}

var OldschoolLookup = function OldschoolLookup() {
  return function(info, next) {
    var families = [4, 6]
      , result_counter = 0
      ;
    families.forEach(function(family) {
      require('dns').lookup(info.host, family, function(error, address, family) {
        if (address) {
          info.addresses = info.addresses || [];
          info.addresses.push(address);
        }
        if (++result_counter == families.length) {
          next();
        }
      });
    });
  }
}

Browser.defaultResolverStack = [
  ServiceResolve()
, 'dnsServiceGetAddrInfo' in capi ? GetAddrInfo() : OldschoolLookup()
]
