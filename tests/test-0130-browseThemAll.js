#!/usr/bin/env node

var mdns  = require('../lib/mdns')
  , assert = require('assert')
  , t = require('./mdns_test')
  ;

t.lifeExpectancy = 15000;

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

var ad = t.runTestAd(type, 1337, 2000, 10000, function() {
  assert.strictEqual(down, up);
  assert.strictEqual(down + up, changed);
  browser.stop();
  t.done();
});

browser.start();

// vim: filetype=javascript:
