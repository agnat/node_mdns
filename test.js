var mDNS   = require('./mDNS'),
    sys    = require('sys'),
    assert = require('assert');

var puts = sys.puts;

ad =  mDNS.createAdvertisement("node_js_mdns_test", 4321, mDNS.TCP);
ad.start();

browser = mDNS.createBrowser("node_js_mdns_test", mDNS.TCP);
browser.start();

setTimeout(function() { ad.stop(); }, 10000);
