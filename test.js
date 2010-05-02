var mDNS   = require('./mDNS'),
    sys    = require('sys'),
    assert = require('assert');

var puts = sys.puts;

s =  mDNS.createService("node_js_mdns_test", 4321, mDNS.TCP);
s.announce();
setTimeout(function() { s.discontinue(); }, 10000);
