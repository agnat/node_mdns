#!/usr/bin/env node

var mdns  = require('../../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.capi.DNSServiceRef();

assert.doesNotThrow( function() {
  mdns.capi.dnsServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

assert.notEqual(serviceRef.fd, -1);
assert.strictEqual(serviceRef.initialized, true);

assert.throws(function() {
  mdns.capi.dnsServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  mdns.capi.dnsServiceRegister();
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister(ref, "", 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister(ref, 0, "", null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister(ref, 0, 0, 0, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister(ref, 0, 0, null, null,
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister(ref, 0, 0, null, 0,
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    0, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, 0, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, "", null, null, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, 0, null, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, "", null);
});

// vim: filetype=javascript:
