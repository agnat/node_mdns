var mDNS = require('./mDNS'),
    sys  = require('sys'),
    puts = sys.puts;

s =  mDNS.createService( function() {
  puts('=== ready ===');
});

puts(s);


s.announce();

setTimeout(function() { puts('timeout'); s.close();}, 20000);


puts('done');
