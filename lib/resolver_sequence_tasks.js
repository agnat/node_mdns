var dns_sd = require('./dns_sd'),
    util = require('util');

var MDNSService = require('./mdns_service').MDNSService;

exports.DNSServiceResolve = function DNSServiceResolve(options) {
  options = options || {};
  options.flags = options.flags || 0;
  return function DNSServiceResolve(info, next) {
    var resolver = new MDNSService();
    dns_sd.DNSServiceResolve(resolver.serviceRef, options.flags,
        info.interfaceIndex, info.serviceName,
        '' + info.regtype, info.replyDomain,
        function(sdRef, flags, iface, errorCode, fullname, hosttarget, port, txtRecord, context) {
          var error = dns_sd.buildException(errorCode);
          if (error) {
            resolver.stop()
            next(error);
            return;
          } else {
            if (info.interfaceIndex === iface) {
              if (fullname)       info.fullname = fullname;
              if (hosttarget)     info.host = hosttarget;
              if (port)           info.port = port;
              if (txtRecord)      info.txtRecord = txtRecord;
              resolver.stop()
              next();
            }
          }
        }, null);
    resolver.start();
  };
}

exports.DNSServiceGetAddrInfo = function DNSServiceGetAddrInfo(options) {
  options = options || {};
  var family_flags = 0;
  if ('families' in options) {
    if (options.families.indexOf(4) !== -1) {
      family_flags |= dns_sd.kDNSServiceProtocol_IPv4;
    }
    if (options.families.indexOf(6) !== -1) {
      family_flags |= dns_sd.kDNSServiceProtocol_IPv6;
    }
  }
  return function DNSServiceGetAddrInfo(info, next) {
    var adr_getter = new MDNSService()
      , addresses = []
      ;
    dns_sd.DNSServiceGetAddrInfo(
        adr_getter.serviceRef, 0, info.interfaceIndex, family_flags, info.host,
        function(sdRef, flags, iface, errorCode, hostname, address, context) {
          var error = dns_sd.buildException(errorCode);
          if (error) {
            adr_getter.stop()
            next(error);
          } else {
            if (iface === info.interfaceIndex) {
              addresses.push(address);
            }
            if ( ! (dns_sd.kDNSServiceFlagsMoreComing & flags)) {
              info.addresses = addresses;
              adr_getter.stop()
              next();
            }
          }
        }, null);
    adr_getter.start();
  };
}

var _getaddrinfo;
try {
  var cares = process.binding('cares_wrap');
  _getaddrinfo = function getaddrinfo_v06x(host, family, cb) {
    var wrap = cares.getaddrinfo(host, family);
    if ( ! wrap) {
      throw errnoException(errno, 'getaddrinfo');
    }
    wrap.oncomplete = function(addresses) {
      if (addresses) {
        cb(undefined, addresses);
      } else {
        cb(errnoException(errno, 'getaddrinfo'));
      }
    }
  };
} catch (ex) {
  _getaddrinfo = process.binding('net').getaddrinfo;
}

exports.getaddrinfo = function getaddrinfo(options) {
  options = options || {};
  var families = options.families || [4, 6];
  return function getaddrinfo(info, next) {
    var last_error
      , counter = 0
      ;
    // XXX in older versions of node a zero family value is not supported
    families.forEach(function(family) {
      _getaddrinfo(info.host, family, function(error, addresses) {
        if (error) {
          last_error = error
        } else {
          info.addresses = (info.addresses || []).concat(addresses);
        }
        if (++counter === families.length) {
          next(last_error);
        }
      });
    });
  }
}

function unique(array) {
  var o = {} , p , r = [] ;
  array.forEach(function(e) { o[e] = e });
  for (p in o) { r.push(o[p]) }
  return r;
}

exports.makeAddressesUnique = function makeAddressesUnique() {
  return function makeAddressesUnique(info, next) {
    info.addresses = unique(info.addresses);
    next();
  }
}

exports.filterAdresses = function filterAdresses(filter_function) {
  return function filterAdresses(info, next) {
    info.addresses = (info.addresses || []).filter(filter_function);
    next();
  }
}

exports.logInfo = function logInfo() {
  return function logInfo(info, next) {
    console.log(info);
    next();
  }
}

