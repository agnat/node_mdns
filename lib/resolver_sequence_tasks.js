var dns_sd = require('./dns_sd'),
    util = require('util');

var MDNSService = require('./mdns_service').MDNSService;

exports.DNSServiceResolve = function DNSServiceResolve(options) {
  options = options || {};
  options.flags = options.flags || 0;
  options.unwrapTxtRecord = 'unwrapTxtRecord' in options ? options.unwrapTxtRecord : true;
  return function DNSServiceResolve(service, next) {
    var resolver = new MDNSService();
    dns_sd.DNSServiceResolve(resolver.serviceRef, options.flags,
        service.interfaceIndex, service.name,
        '' + service.type, service.replyDomain,
        function(sdRef, flags, iface, errorCode, fullname, hosttarget, port, txtRecord, context) {
          var error = dns_sd.buildException(errorCode);
          if (error) {
            resolver.stop()
            next(error);
            return;
          } else {
            if (service.interfaceIndex === iface) {
              if (fullname)       service.fullname = fullname;
              if (hosttarget)     service.host = hosttarget;
              if (port)           service.port = port;
              if (txtRecord) {
                service.rawTxtRecord = txtRecord;
                if (options.unwrapTxtRecord) {
                  service.txtRecord = dns_sd.txtRecordBufferToObject(txtRecord)
                }
              }
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
  return function DNSServiceGetAddrInfo(service, next) {
    var adr_getter = new MDNSService()
      , addresses = []
      ;
    dns_sd.DNSServiceGetAddrInfo(
        adr_getter.serviceRef, 0, service.interfaceIndex, family_flags, service.host,
        function(sdRef, flags, iface, errorCode, hostname, address, context) {
          var error = dns_sd.buildException(errorCode);
          if (error) {
            adr_getter.stop()
            next(error);
          } else {
            if (iface === service.interfaceIndex) {
              addresses.push(address);
            }
            if ( ! (dns_sd.kDNSServiceFlagsMoreComing & flags)) {
              service.addresses = addresses;
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
  return function getaddrinfo(service, next) {
    var last_error
      , counter = 0
      ;
    // XXX in older versions of node a zero family value is not supported
    families.forEach(function(family) {
      _getaddrinfo(service.host, family, function(error, addresses) {
        if (error) {
          last_error = error
        } else {
          service.addresses = (service.addresses || []).concat(addresses);
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
  return function makeAddressesUnique(service, next) {
    service.addresses = unique(service.addresses);
    next();
  }
}

exports.filterAdresses = function filterAdresses(filter_function) {
  return function filterAdresses(service, next) {
    service.addresses = (service.addresses || []).filter(filter_function);
    next();
  }
}

exports.logService = function logService() {
  return function logService(service, next) {
    console.log(service);
    next();
  }
}

