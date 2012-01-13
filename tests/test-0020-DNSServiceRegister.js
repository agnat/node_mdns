#!/usr/bin/env node

var mdns  = require('../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();

assert.doesNotThrow( function() {
  mdns.dns_sd.DNSServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

assert.notEqual(serviceRef.fd, -1);
assert.strictEqual(serviceRef.initialized, true);

assert.throws(function() {
  mdns.dns_sd.DNSServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  mdns.dns_sd.DNSServiceRegister();
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, "", 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, "", null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, 0, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, null,
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, 0,
    null, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    0, null, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, 0, 4321, null, null, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, "", null, null, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, 0, null, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, "", null);
});

assert.throws( function() {
  var ref = new mdns.dns_sd.DNSServiceRef()
    , not_a_valid_port = 100000000;
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, not_a_valid_port, null, null, null);
});

assert.throws( function() {
  var ref = new mdns.dns_sd.DNSServiceRef()
    , not_a_valid_port = -1;
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, not_a_valid_port, null, null, null);
});

// vim: filetype=javascript:
