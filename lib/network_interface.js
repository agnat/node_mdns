var net = require('net')
  , os = require('os')
  , dns_sd = require('./dns_sd')
  , interfaceIndexDeprecatedWarningPrinted = false
  ;

var have_if_nametoindex = typeof dns_sd.if_nametoindex !== 'undefined';

exports.interfaceIndex = function interfaceIndex(options) {
  var networkInterface
    , index = 0;
  if (typeof options.interfaceIndex !== 'undefined') {
    if( ! interfaceIndexDeprecatedWarningPrinted) {
      console.warn("WARNING: 'interfaceIndex' is deprecated and will be " +
          "removed. Please use 'networkInterface' instead and see the " +
          "documentation for new features.");
      interfaceIndexDeprecatedWarningPrinted = true;
    }
    networkInterface = options.interfaceIndex;
  } else {
    networkInterface = options.networkInterface;
  }
  if (typeof networkInterface !== 'undefined') {
    if (net.isIP(networkInterface)) {
      if ( ! have_if_nametoindex) {
        throw new Error('IP address to interface index conversion is not ' +
            'supported on this platform');
      }
      index = dns_sd.if_nametoindex(addressToName(networkInterface));
      //console.log('got IP', networkInterface, '->', index);
    } else if (isString(networkInterface)) {
      if ( ! have_if_nametoindex) {
        throw new Error('interface name to index conversion is not supported ' +
            'on this platform');
      }
      index = dns_sd.if_nametoindex(networkInterface);
      //console.log('got if name', networkInterface, '->', index);
    } else {
      //console.log('got index', networkInterface);
      index = networkInterface;
    }
  }
  return index;
}

function addressToName(address) {
  if (typeof os.networkInterfaces === 'undefined') {
    throw new Error('IP address to interface index conversion is not ' +
        'supported with this version of node');
  }
  var addresses = os.networkInterfaces();
  for (var name in addresses) {
    for (var i = 0; i < addresses[name].length; ++i) {
      if (addresses[name][i].address === address) {
        return name;
      }
    }
  }
  throw new Error('interface with address ' + address + ' does not exist');
}

function isString(s) {
  return toString.call(s) == '[object String]';
}

