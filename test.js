var mDNS = require('./mDNS'),
    sys  = require('sys'),
    puts = sys.puts;

s =  mDNS.createService( function() {
  puts('=== service announced');
});

puts(s);


s.announce();
s.addListener('shutdown', function() { puts('=== announcement is down') });

setTimeout(function() { puts('shutdown'); s.shutdown();}, 5000);

puts('setup done');
