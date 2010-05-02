var mDNS   = require('./mDNS'),
    sys    = require('sys'),
    assert = require('assert');

var puts = sys.puts,
    p    = sys.p;

ad =  mDNS.createAdvertisement("node_js_mdns_test", 4321, mDNS.TCP);
ad.start();

browser = mDNS.createBrowser("node_js_mdns_test", mDNS.TCP);
browser.addListener('changed', function(err, flags, info) {
  var verb = 'gone';
  if (flags & mDNS.ServiceAdd) {
    verb = 'found';
    mDNS.resolve(info, function(resolver_info) {
      p("=== resolved", resolver_info);
    });
  }
  puts('=== service ' + verb + ': ' + info.name + ' ' + info.regtype + ' ' + info.domain);
});
browser.start();

setTimeout(function() { ad.stop(); }, 4000);
setTimeout(function() { browser.stop(); }, 6000);
