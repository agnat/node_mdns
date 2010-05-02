var mDNS   = require('./mDNS'),
    sys    = require('sys'),
    assert = require('assert');

var puts = sys.puts,
    p    = sys.p;

ad =  mDNS.createAdvertisement("node_js_mdns_test", 4321, mDNS.TCP);
ad.start();

browser = mDNS.createBrowser("node_js_mdns_test", mDNS.TCP);
browser.addListener('changed', function(err, flags, interface_index, name, regtype, domain) {
  var verb = 'gone';
  if (flags & mDNS.ServiceAdd) {
    verb = 'found';
    var resolver = mDNS.createResolver();
    resolver.addListener('resolved', function() { puts('=== resolved'); resolver.stop(); });
    resolver.start(flags, interface_index, name, regtype, domain);
  }
  puts('=== service ' + verb + ': ' + name + ' ' + regtype + ' ' + domain);
});
browser.start();

setTimeout(function() { ad.stop(); }, 4000);
setTimeout(function() { browser.stop(); }, 6000);
