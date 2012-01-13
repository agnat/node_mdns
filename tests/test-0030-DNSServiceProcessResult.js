#!/usr/bin/env node

var mdns   = require('../lib/mdns')
  , assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();
var IOWatcher = process.binding('io_watcher').IOWatcher;
var watcher = new IOWatcher();

timeout = 3000;
var timeoutId = setTimeout(function() {
  assert.fail("callbacks not called within " + (timeout/1000) + " seconds");
}, timeout);

watcher.callback = function() {
  assert.doesNotThrow( function() {
    mdns.dns_sd.DNSServiceProcessResult(serviceRef);
  });
}

var result_callback = function(sdRef, flags, errorCode, name, serviceType, domain, context) {
  assert.strictEqual(sdRef, serviceRef);
  assert.strictEqual(typeof flags, "number");
  assert.strictEqual(typeof errorCode, "number");
  assert.strictEqual(errorCode, mdns.dns_sd.kDNSServiceErr_NoError);
  assert.strictEqual(typeof name, "string");
  assert.strictEqual(typeof serviceType, "string");
  assert.strictEqual(typeof domain, "string");
  assert.strictEqual(typeof context, "string");
  assert.strictEqual(context, "foobar");
  clearTimeout(timeoutId);
  watcher.stop();
}

mdns.dns_sd.DNSServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, result_callback, "foobar");

watcher.set(serviceRef.fd, true, false);
watcher.start();

// vim: filetype=javascript:
