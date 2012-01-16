#!/usr/bin/env node

var mdns  = require('../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();

assert.doesNotThrow(function() {
  mdns.dns_sd.DNSServiceBrowse(serviceRef, 0, 0, "_node-mdns-test._tcp", null,
    function() {}, null);
});

// serviceRef already initialized
assert.throws(function() {
  mdns.dns_sd.DNSServiceBrowse(serviceRef, 0, 0, "_node-mdns-test._tcp", null,
    function() {}, null);
});

// not enough arguments
assert.throws(function() {
  mdns.dns_sd.DNSServiceBrowse();
});

// serviceRef must not be a string
assert.throws(function() {
  mdns.dns_sd.DNSServiceBrowse("", 0, 0, "_node-mdns-test._tcp", null,
    function() {}, null);
});

// flags must be a number, not a string
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceBrowse(ref, "", 0, "_node-mdns-test._tcp", null,
    function() {}, null);
});

// interfaceIndex must be a number, not a string
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceBrowse(ref, 0, "", "_node-mdns-test._tcp", null,
    function() {}, null);
});

// service type must be a string, not null
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceBrowse(ref, 0, 0, null, null, function() {}, null);
});

// service type must be a string, not a number
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceBrowse(ref, 0, 0, 0, null, function() {}, null);
});

// domain must be a string, not a number
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceBrowse(ref, 0, 0, "_node-mdns-test._tcp", 0,
    function() {}, null);
});

// callback must be a function, not a number
assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceBrowse(ref, 0, 0, "_node-mdns-test._tcp", null, 0, null);
});

// vim: filetype=javascript:
