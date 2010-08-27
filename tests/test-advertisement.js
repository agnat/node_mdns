#!/usr/bin/env node

var mdns = require('../lib/new_mdns'),
    sys  = require('sys');

var ad = mdns.createAdvertisement('_node-mdns-test._tcp', 4321, function() {
  sys.puts("called. this: " + sys.inspect(this));
});

ad.start();
