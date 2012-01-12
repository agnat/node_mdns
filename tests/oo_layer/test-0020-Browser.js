#!/usr/bin/env node

var mdns   = require('../../lib/mdns')
  , assert = require('assert')
  , util   = require('util')
  ;

// XXX This test will break if two instances are run on the same network

var timeout = 5000;
var timeoutId = setTimeout(function() {
  assert.fail("test did not finish within " + (timeout / 1000) + " seconds.");
}, timeout)

var browser = mdns.createBrowser(mdns.tcp('node-mdns-test'));

function stopBrowserIfDone() {
  if (upCount > 0 &&
      downCount > 0 && 
      upCount == downCount && 
      changedCount == upCount + downCount)
  {
    browser.stop();
    clearTimeout(timeoutId);
  }
}

var changedCount = 0;
browser.on('serviceChanged', function(service, flags) {
  //console.log("changed:", service);
  assert.strictEqual(typeof service.flags, 'number');
  if (changedCount === 0) {
    assert.ok(service.flags & mdns.kDNSServiceFlagsAdd);
    assert.strictEqual(typeof service.fullname, 'string');
    assert.strictEqual(typeof service.host, 'string');
    assert.strictEqual(typeof service.port, 'number');
    assert.strictEqual(service.port, 4321);

    assert.ok('addresses' in service);
    assert.ok(Array.isArray(service.addresses));
    assert.ok(service.addresses.length > 0);
  }
  assert.strictEqual(typeof service.interfaceIndex, 'number');
  assert.strictEqual(typeof service.name, 'string');
  assert.ok(service.type instanceof mdns.ServiceType);
  assert.strictEqual('' + service.type, '_node-mdns-test._tcp.');
  assert.strictEqual(typeof service.replyDomain, 'string');
  assert.strictEqual(service.replyDomain, 'local.');

  changedCount += 1;
  stopBrowserIfDone();
});

var upCount = 0;
browser.on('serviceUp', function(service) {
  //console.log("up:", service);
  assert.strictEqual(typeof service.flags, 'number');
  assert.strictEqual(typeof service.interfaceIndex, 'number');
  assert.strictEqual(typeof service.name, 'string');
  assert.ok(service.type instanceof mdns.ServiceType);
  assert.strictEqual('' + service.type, '_node-mdns-test._tcp.');
  assert.strictEqual(typeof service.replyDomain, 'string');
  assert.strictEqual(service.replyDomain, 'local.');

  assert.strictEqual(typeof service.fullname, 'string');
  assert.strictEqual(typeof service.host, 'string');
  assert.strictEqual(typeof service.port, 'number');
  assert.strictEqual(service.port, 4321);

  assert.ok('addresses' in service);
  assert.ok(Array.isArray(service.addresses));
  assert.ok(service.addresses.length > 0);

  upCount += 1;
  stopBrowserIfDone();
});

var downCount = 0;
browser.on('serviceDown', function(service) {
  assert.strictEqual(typeof service.flags, 'number');
  assert.strictEqual(typeof service.interfaceIndex, 'number');
  assert.strictEqual(typeof service.name, 'string');
  assert.ok(service.type instanceof mdns.ServiceType);
  assert.strictEqual('' + service.type, '_node-mdns-test._tcp.');
  assert.strictEqual(typeof service.replyDomain, 'string');
  assert.strictEqual(service.replyDomain, 'local.');

  downCount += 1;
  stopBrowserIfDone();
});

browser.start();

process.on('exit', function() {
  assert.ok(changedCount >= 2);
  assert.ok(upCount >= 1);
  assert.ok(downCount >= 1);
});

var ad = mdns.createAdvertisement(mdns.tcp('node-mdns-test'), 4321, function(err, service, flags) {
    if (err) throw err;
    setTimeout(function() { ad.stop() }, 500);
});

ad.start();


//=== Regression Tests ========================================================

// autoResolve can not be set to a falsy value.
// Reported by orlandv and others (issue #9)

// FIX: replaced nested-flag-madness with something more scalable.
//var regression_browser = mdns.createBrowser(mdns.tcp('node-mdns-t2'),
//    {autoResolve: false});
//assert.ok(regression_browser.autoresolve === false);



// vim: filetype=javascript
