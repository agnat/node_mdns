var mdns   = require('./lib/mdns'),
    sys    = require('sys'),
    assert = require('assert');

var puts    = sys.puts,
    inspect = sys.inspect;

var ad =  mdns.createAdvertisement("node_mdns_test", 4321, 'tcp');
var browser = mdns.createBrowser("node_mdns_test", 'tcp');

timeout_id = setTimeout(function() { assert.fail('time out'); }, 10000);

browser.addListener('serviceUp', function(info) {
  puts('Up', inspect(info));
  assert.equal('_node_mdns_test._tcp.', info['regtype']);
  assert.equal(4321, info['port']);
  ad.stop();
});

browser.addListener('serviceDown', function(info) {
  puts('Down', inspect(info));
  assert.equal('_node_mdns_test._tcp.', info['regtype']);
  browser.stop();
  clearTimeout(timeout_id);
});

browser.start();
ad.start();

//==============================================================================

assert.throws(function() { mdns.createAdvertisement()});
assert.throws(function() { mdns.createAdvertisement('narf')});
assert.throws(function() { mdns.createAdvertisement(function(){})});
assert.throws(function() { mdns.createAdvertisement('narf', function(){})});

var test_ad = mdns.createAdvertisement('node_mdns_test', 4321);
test_ad.start();
assert.throws(function() { test_ad.start()});
test_ad.stop();


