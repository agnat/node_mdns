var dns_sd = require('./dns_sd')
  , util = require('util')
  , MDNSService = require('./mdns_service').MDNSService
  ;

// Converts a versionString (v0.11.5) to an array of numbers [0,11,5]
function versionStringToArray(versionString) {
  return /^v(\d+(\.\d+)*)/.exec(versionString)[1].split('.').map(Number);
}

// Accepts versions in the form of v0.11.5.
// Returns 1 if versionStringA is higher than versionStringB, -1 if lower, 0 if equal.
function compareVersions(versionStringA, versionStringB) {
  var versionA = versionStringToArray(versionStringA);
  var versionB = versionStringToArray(versionStringB);

  // Make sure the versions have just as segments, fill them up with 0's.
  // Ex. Comparing v0.11 with v0.11.5 will become v0.11.0 and v0.11.5
  while(versionA.length < versionB.length) { versionA.push(0); }
  while(versionB.length < versionA.length) { versionB.push(0); }

  var versionLength = versionA.length;

  for(var i=0;i<versionLength;i++) {
    if (versionA[i] > versionB[i]) {
      return 1;
    } else if (versionA[i] < versionB[i]) {
      return -1;
    }
  }

  return 0;
}

// Checks whether Ipv6 is supported on the OS.
function hasIpv6Support() {
  // We presume only Linux has the capability to disable ipv6.
  if (process.platform !== 'linux') {
    return true;
  }
  // Determine ipv6 support on Linux by checking whether if_inet6 exists.
  try {
    fs.statSync('/proc/net/if_inet6')
    return true;
  } catch(e) {
    return false;
  }
}

// Checks whether we can use { families: [0] } for getaddrinfo.
// Support was added in NodeJS v0.11.5
function hasGetaddrinfoZeroFamilySupport() {
  return compareVersions(process.version, 'v0.11.5') >= 0;
}

exports.DNSServiceResolve = function DNSServiceResolve(options) {
  options = options || {};
  options.flags = options.flags || 0;
  options.unwrapTxtRecord =
    'unwrapTxtRecord' in options ? options.unwrapTxtRecord : true;
  return function DNSServiceResolve(service, next) {
    try {
      var resolver = new MDNSService();

      function on_resolver_done(sdRef, flags, iface, errorCode, fullname,
          hosttarget, port, txtRecord, context)
      {
        try {
          var error = dns_sd.buildException(errorCode);
          if ( ! error && service.interfaceIndex === iface) {
            if (fullname)       service.fullname = fullname;
            if (hosttarget)     service.host = hosttarget;
            if (port)           service.port = port;
            // all services have a TXT record. ignore the empty ones.
            if (txtRecord.length > 1) {
              service.rawTxtRecord = txtRecord;
              if (options.unwrapTxtRecord) {
                service.txtRecord = dns_sd.txtRecordBufferToObject(txtRecord)
              }
            }
          }
          resolver.stop();
          next(error);
        } catch (ex) {
          resolver.stop();
          next(ex);
        }
      }

      dns_sd.DNSServiceResolve(resolver.serviceRef, options.flags,
          service.interfaceIndex, service.name, '' + service.type,
          service.replyDomain, on_resolver_done, null);
      resolver.start();
    } catch (ex) { 
      resolver.stop();
      next(ex);
    }
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
    try {
      var adr_getter = new MDNSService()
        , addresses = []
        ;

      function on_get_addr_info_done(sdRef, flags, iface, errorCode, hostname,
          address, context)
      {
        try {
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
        } catch (ex) {
          adr_getter.stop();
          next(ex);
        }
      }

      dns_sd.DNSServiceGetAddrInfo(
          adr_getter.serviceRef, 0, service.interfaceIndex, family_flags,
          service.host, on_get_addr_info_done, null);
      adr_getter.start();
    } catch (ex) {
      adr_getter.stop();
      next(ex);
    }
  }
}

var _getaddrinfo;
try {
  var cares = process.binding('cares_wrap');
  function getaddrinfo_complete(err, addresses, cb) {
    if (addresses) {
      cb(undefined, addresses);
    } else if (err === 'ENOENT') {
      cb(undefined, []);
    } else {
      cb(errnoException(err, 'getaddrinfo'));
    }
  }
  function getaddrinfo_0_11(host, family, cb) {
    var req = new cares.GetAddrInfoReqWrap()
      , err = cares.getaddrinfo(req, host, family)
      ;
    req.oncomplete = function oncomplete(err, addresses) {
        getaddrinfo_complete(err, addresses, cb);
    }
    if (err) throw errnoException(err, 'getaddrinfo', host);
  }
  function getaddrinfo_0_10(host, family, cb) {
    var wrap = cares.getaddrinfo(host, family);
    if ( ! wrap) {
      throw errnoException(process._errno || global.errno, 'getaddrinfo');
    }
    wrap.oncomplete = function (addresses) {
      getaddrinfo_complete((process._errno || global.errno), addresses, cb);
    }
  }
  // node 0.11+ cares.getaddrinfo function uses request object.
  // use appropriate version based on node version number
  if (Number(process.version.match(/^v(\d+\.\d+)/)[1]) > 0.1) {
    _getaddrinfo = getaddrinfo_0_11;
  } else {
    _getaddrinfo = getaddrinfo_0_10;
  }
} catch (ex) {
  _getaddrinfo = process.binding('net').getaddrinfo;
}

// Depending on the support for NodeJS and the OS, we use the most supported
// address families as default for getaddrinfo.
var defaultFamilies = hasGetaddrinfoZeroFamilySupport()
  ? [0]
  : hasIpv6Support()
    ? [4, 6]
    : [4];

exports.getaddrinfo = function getaddrinfo(options) {
  options = options || {};
  var families = options.families || defaultFamilies;
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
  array.forEach(function(e) { o[e] = undefined });
  for (p in o) { r.push(p) }
  return r;
}

exports.makeAddressesUnique = function makeAddressesUnique() {
  return function makeAddressesUnique(service, next) {
    service.addresses = unique(service.addresses);
    next();
  }
}

exports.filterAddresses = function filterAddresses(filter_function) {
  return function filterAddresses(service, next) {
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

function errnoException(errorno, syscall) {
  // TODO make this more compatible with ErrnoException from src/node.cc
  // Once all of Node is using this function the ErrnoException from
  // src/node.cc should be removed.
  var e = new Error(syscall + ' ' + errorno);

  // For backwards compatibility. libuv returns ENOENT on NXDOMAIN.
  if (errorno == 'ENOENT') {
    errorno = 'ENOTFOUND'
  }

  e.errno = e.code = errorno;
  e.syscall = syscall;
  return e;
}


