#!/usr/bin/env node

var mdns   = require('../lib/mdns'),
    util   = require('util'),
    assert = require('assert');

var t = 500; // ms

var ad1 = mdns.createAdvertisement(['mdns-test1', 'tcp'], 4321);
ad1.start();
setTimeout(function() { ad1.stop() }, t);

var ad2 = mdns.createAdvertisement(['mdns-test2', 'tcp'], 4322, {});
ad2.start();
setTimeout(function() { ad2.stop() }, t);

function checkAd(service, name, proto) {
  assert.ok('flags' in service);
  assert.ok(service.type instanceof mdns.ServiceType);
  assert.strictEqual(service.type.toString(), '_' + name + '._' + proto + '.');
}

var ad3 = mdns.createAdvertisement(['mdns-test3', 'tcp'], 4323, {name: 'foobar'}, function(error, service) {
  if (error) assert.fail(error);
  
  checkAd(service, 'mdns-test3', 'tcp');

  var ad = this;
  setTimeout(function(){ ad.stop() }, t);
  //console.log(service)
});
ad3.start();

var ad4 = mdns.createAdvertisement(['mdns-test4', 'udp'], 4324, {}, function(error, service) {
  if (error) assert.fail(error);

  checkAd(service, 'mdns-test4', 'udp');

  var ad = this;
  setTimeout(function(){ ad.stop() }, t);
});
ad4.start();

// vim: filetype=javascript
