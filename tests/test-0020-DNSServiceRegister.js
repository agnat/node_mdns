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

// duplicate initialization of serviceRef
assert.throws(function() {
  mdns.dns_sd.DNSServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

// not enough arguments
assert.throws(function() {
  mdns.dns_sd.DNSServiceRegister();
});

// flags must be a number, not a string
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, "", 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

// interfaceIndex must be a number, not a string
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, "", null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

// name must be a string, not a number
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, 0, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);
});

// service type must be a string, not null
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, null,
    null, null, 4321, null, null, null);
});

// service type has to be a string, not a number
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, 0,
    null, null, 4321, null, null, null);
});

// domain must not be a string, not a number
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    0, null, 4321, null, null, null);
});

// host must be a string, not a number
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, 0, 4321, null, null, null);
});

// port must be a number, not a string
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, "", null, null, null);
});

// txtRecord must be a TXTRecordRef or buffer, not a number
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, 0, null, null);
});

// callback must be a function, not a string
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, "", null);
});

// invalid port number
assert.throws( function() {
  var ref = new mdns.dns_sd.DNSServiceRef()
    , not_a_valid_port = 100000000;
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, not_a_valid_port, null, null, null);
});

// invalid port number
assert.throws( function() {
  var ref = new mdns.dns_sd.DNSServiceRef()
    , not_a_valid_port = -1;
  mdns.dns_sd.DNSServiceRegister(ref, 0, 0, null, "_node-mdns-test._tcp",
    null, null, not_a_valid_port, null, null, null);
});

// vim: filetype=javascript:
