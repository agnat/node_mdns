var os = require('os')
  , mdns_test = require('../utils/lib/mdns_test')
  , dns_sd = require('../lib/dns_sd.js')
  , nif = require('../lib/network_interface.js');
  ;

function loopback() {
  var interfaces = os.networkInterfaces();
  for (var name in interfaces) {
    for (var i = 0; i < interfaces[name].length; ++i) {
      if (interfaces[name][i].address === '127.0.0.1') {
        return name;
      }
    }
  }
  throw new Error('failed to find loopback interface');
}

exports['if_nametoindex'] = function(t) {
  if (typeof dns_sd.if_nametoindex !== 'undefined') {
    var interfaces = os.networkInterfaces();
    for (var name in interfaces) {
      t.ok(dns_sd.if_nametoindex(name) > 0);
    }
  } else {
    console.log('[SKIPPED] if_nametoindex() not supported on this platform');
  }
  t.done();
}

exports['interfaceIndex'] = function(t) {
  t.strictEqual(nif.interfaceIndex({interfaceIndex: 0}), 0);
  t.strictEqual(nif.interfaceIndex({interfaceIndex: 1}), 1);

  t.strictEqual(nif.interfaceIndex({networkInterface: 0}), 0);
  t.ok(nif.interfaceIndex({networkInterface: loopback()}) > 0);
  t.ok(nif.interfaceIndex({networkInterface: '127.0.0.1'}) > 0);

  t.strictEqual(nif.interfaceIndex({networkInterface: loopback()}),
      nif.interfaceIndex({networkInterface: '127.0.0.1'}));

  t.done();
}
