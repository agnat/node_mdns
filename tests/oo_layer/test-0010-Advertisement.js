#!/usr/bin/env node

var mdns   = require('../../lib/mdns'),
    util   = require('util'),
    assert = require('assert');

var t = 500; // ms

var ad1 = mdns.createAdvertisement('mdns-test1', 4321);
ad1.start();
setTimeout(function() { ad1.stop() }, t);

var ad2 = mdns.createAdvertisement('mdns-test2', 4322, {});
ad2.start();
setTimeout(function() { ad2.stop() }, t);

var ad3 = mdns.createAdvertisement('mdns-test3', 4323, function(error, info, flags) {
  if (error) assert.fail(error);
  var ad = this;
  setTimeout(function(){ ad.stop() }, t);
});
ad3.start();

var ad4 = mdns.createAdvertisement('mdns-test4', 4324, {}, function(error, info, flags) {
  if (error) assert.fail(error);
  var ad = this;
  setTimeout(function(){ ad.stop() }, t);
});
ad4.start();

// vim: filetype=javascript
