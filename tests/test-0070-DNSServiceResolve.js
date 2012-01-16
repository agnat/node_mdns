#!/usr/bin/env node

var mdns  = require('../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();

assert.doesNotThrow(function() {
  mdns.dns_sd.DNSServiceResolve(serviceRef, 0, 0, 'hostname',
    '_node-mdns-test._tcp.', 'local.', function() {}, null);
});

assert.strictEqual(serviceRef.initialized, true);

// not enough arguments
assert.throws(function() {
  mdns.dns_sd.DNSServiceResolve();
});

// flags must be a number, not a string
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceResolve(ref, "", 0, 'hostname',
    '_node-mdns-test._tcp.', 'local.', function() {}, null);
});

// interfaceIndex must be a number, not null
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceResolve(ref, 0, null, 'hostname',
    '_node-mdns-test._tcp.', 'local.', function() {}, null);
});

// name must be a string, not null
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceResolve(ref, 0, 0, null,
    '_node-mdns-test._tcp.', 'local.', function() {}, null);
});

// regtype must be a string, not null
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceResolve(ref, 0, 0, 'hostname', null,
    'local.', function() {}, null);
});

// domain must be a string, not null
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceResolve(ref, 0, 0, 'hostname',
    '_node-mdens-test._tcp', null, function() {}, null);
});

// callback must be a function, not null
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceResolve(ref, 0, 0, 'hostname',
    '_node-mdens-test._tcp', 'local.', null, null);
});

// vim: filetype=javascript:
