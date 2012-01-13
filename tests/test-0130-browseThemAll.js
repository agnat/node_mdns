#!/usr/bin/env node

var mdns  = require('../lib/mdns')
  , assert = require('assert')
  , t = require('./mdns_test')
  ;


var browser = new mdns.browseThemAll()
  , up = 0
  , down = 0
  , changed = 0
  , type = t.suffixedServiceType('node-mdns', 'udp')
  ;

browser.on('serviceUp', function(service) {
  if (type.matches(service.type)) ++up;
});
browser.on('serviceDown', function(service) {
  if (type.matches(service.type)) ++down;
});
browser.on('serviceChanged', function(service) {
  if (type.matches(service.type)) ++changed;
});

// it takes forever until the service type disappears
var cooltime = 10000;
t.lifeExpectancy = cooltime + 5000;
var ad = t.runTestAd(type, 1337, 1000, cooltime, function() {
  assert.strictEqual(down, up);
  assert.strictEqual(down + up, changed);
  browser.stop();
  t.done();
});

browser.start();

// vim: filetype=javascript:
