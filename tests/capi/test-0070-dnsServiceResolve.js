#!/usr/bin/env node

var mdns  = require('../../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.capi.DNSServiceRef();

assert.doesNotThrow(function() {
  mdns.capi.dnsServiceResolve(serviceRef, 0, 0, 'hostname', '_node-mdns-test._tcp.',
    'local.', function() {}, null);
});

assert.strictEqual(serviceRef.initialized, true);

assert.throws(function() {
  mdns.capi.dnsServiceResolve();
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceResolve(ref, "", 0, 'hostname', '_node-mdns-test._tcp.',
    'local.', function() {}, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceResolve(ref, 0, null, 'hostname', '_node-mdns-test._tcp.',
    'local.', function() {}, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceResolve(ref, 0, 0, null, '_node-mdns-test._tcp.',
    'local.', function() {}, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceResolve(ref, 0, 0, 'hostname', null,
    'local.', function() {}, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceResolve(ref, 0, 0, 'hostname', '_node-mdens-test._tcp',
    null, function() {}, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceResolve(ref, 0, 0, 'hostname', '_node-mdens-test._tcp',
    'local.', null, null);
});

// vim: filetype=javascript:
