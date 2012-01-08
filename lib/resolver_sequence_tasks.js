var dns_sd = require('./dns_sd'),
    util = require('util');

var MDNSService = require('./mdns_service').MDNSService;

exports.DNSServiceResolve = function DNSServiceResolve(options) {
  options = options || {};
  return function DNSServiceResolve(info, next) {
    var resolver = new MDNSService()
      , paths = []
      ;
    dns_sd.DNSServiceResolve(resolver.serviceRef, options.flags || 0,
        info.interfaceIndex, info.serviceName,
        '' + info.regtype, info.replyDomain,
        function(sdRef, flags, iface, errorCode, fullname, hosttarget, port, txtRecord, context) {
          var error = dns_sd.buildException(errorCode);
          if (error) {
            resolver.stop()
            next(error);
            return;
          } else {
            var path = {};
            path.interfaceIndex = iface;
            if (fullname)       path.fullname = fullname;
            if (hosttarget)     path.host = hosttarget;
            if (port)           path.port = port;
            if (txtRecord)      path.txtRecord = txtRecord;
            paths.push(path);
            if ( ! (dns_sd.kDNSServiceFlagsMoreComing & flags)) {
              info.paths = paths;
              resolver.stop()
              next();
            }
          }
        }, null);
    resolver.start();
  };
}

exports.DNSServiceGetAddrInfo = function DNSServiceGetAddrInfo() {
  return function DNSServiceGetAddrInfo(info, next) {
    var done_count = 0
    info.paths.forEach(function(p) {
      var addressInfoGetter = new MDNSService()
        , addresses = []
        , last_error
        ;
      dns_sd.DNSServiceGetAddrInfo(addressInfoGetter.serviceRef, 0, 0, 0, p.host,
          function(sdRef, flags, iface, errorCode, hostname, address, context) {
            var error = dns_sd.buildException(errorCode);
            if (error) {
              console.log(error);
              last_error = error;
              done_count++;
              addressInfoGetter.stop()
            } else {
              if (iface !== p.interfaceIndex) {
                console.log('XXX huh? Kaputt?');
              }
              addresses.push(address);
              if ( ! (dns_sd.kDNSServiceFlagsMoreComing & flags)) {
                p.addresses = addresses;
                addressInfoGetter.stop()
                done_count++;
              }
            }
            if (done_count === info.paths.length) {
              next(last_error);
            }
          }, null);
      addressInfoGetter.start();
    });
  };
}

exports.lookup = function lookup() {
  return function lookup(info, next) {
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

