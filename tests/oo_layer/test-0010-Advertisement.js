#!/usr/bin/env node

var mdns   = require('../../lib/mdns'),
    util   = require('util'),
    assert = require('assert');

var t = 500; // ms

var ad1 = mdns.createAdvertisement(['mdns-test1', 'tcp'], 4321);
ad1.start();
setTimeout(function() { ad1.stop() }, t);

var ad2 = mdns.createAdvertisement(['mdns-test2', 'tcp'], 4322, {});
ad2.start();
setTimeout(function() { ad2.stop() }, t);

function checkAd(info, name, proto) {
  assert.ok('flags' in info);
  assert.ok(info.regtype instanceof mdns.RegType);
  assert.strictEqual(info.regtype.toString(), '_' + name + '._' + proto + '.');
}

var ad3 = mdns.createAdvertisement(['mdns-test3', 'tcp'], 4323, {name: 'foobar'}, function(error, info) {
  if (error) assert.fail(error);
  
  checkAd(info, 'mdns-test3', 'tcp');

  var ad = this;
  setTimeout(function(){ ad.stop() }, t);
  //console.log(info)
});
ad3.start();

var ad4 = mdns.createAdvertisement(['mdns-test4', 'udp'], 4324, {}, function(error, info) {
  if (error) assert.fail(error);

  checkAd(info, 'mdns-test4', 'udp');

  var ad = this;
  setTimeout(function(){ ad.stop() }, t);
});
ad4.start();

// vim: filetype=javascript
