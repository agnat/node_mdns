var mDNS   = require('./mDNS'),
    sys    = require('sys'),
    assert = require('assert');

var puts = sys.puts,
    inspect = sys.inspect;

ad =  mDNS.createAdvertisement("node_js_mdns_test", 4321, mDNS.TCP);
browser = mDNS.createBrowser("node_js_mdns_test", mDNS.TCP);

timeout_id = setTimeout(function() { assert.fail('time out'); }, 10000);

browser.addListener('serviceUp', function(info) {
  puts('Up', inspect(info));
  assert.equal('_node_js_mdns_test._tcp.', info['regtype']);
  assert.equal(4321, info['port']);
  ad.stop();
});

browser.addListener('serviceDown', function(info) {
  puts('Down', inspect(info));
  assert.equal('_node_js_mdns_test._tcp.', info['regtype']);
  browser.stop();
  clearTimeout(timeout_id);
});

browser.start();
ad.start();
