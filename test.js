var mDNS = require('./mDNS'),
    sys  = require('sys'),
    puts = sys.puts;

s =  mDNS.createService( "node_js_mdns_test", 4321, mDNS.TCP, function() {
  puts('=== service announced');
});

puts(s);


s.announce();
s.addListener('shutdown', function() { puts('=== announcement is down') });

setTimeout(function() { puts('shutdown'); s.shutdown();}, 10000);

puts('setup done');
