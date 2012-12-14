var os = require('os')
  , mdns_test = require('../utils/lib/mdns_test')
  , nif = require('../lib/network_interface.js');

function loopback() {
  switch (os.platform()) {
    case 'linux':  return 'lo';
    case 'darwin': return 'lo0';
    default: throw new Error('unhandled platform' + os.platform());
  }
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
