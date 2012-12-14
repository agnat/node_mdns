var os = require('os')
  , mdns_test = require('../utils/lib/mdns_test')
  , nif = require('../lib/network_interface.js');

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

exports['getInterfaceIndex'] = function(t) {
  t.strictEqual(nif.getInterfaceIndex({interfaceIndex: 0}), 0);
  t.strictEqual(nif.getInterfaceIndex({interfaceIndex: 1}), 1);

  t.strictEqual(nif.getInterfaceIndex({networkInterface: 0}), 0);
  t.ok(nif.getInterfaceIndex({networkInterface: loopback()}) > 0);
  t.ok(nif.getInterfaceIndex({networkInterface: '127.0.0.1'}) > 0);

  t.strictEqual(nif.getInterfaceIndex({networkInterface: loopback()}),
      nif.getInterfaceIndex({networkInterface: '127.0.0.1'}));

  t.done();
}
