#!/usr/bin/env node

var mdns  = require('../../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();

assert.doesNotThrow(function() {
  mdns.dns_sd.dnsServiceBrowse(serviceRef, 0, 0, "_node-mdns-test._tcp", null, function() {
    }, null);
});

assert.throws(function() {
  mdns.dns_sd.dnsServiceBrowse(serviceRef, 0, 0, "_node-mdns-test._tcp", null, function() {
    }, null);
});

assert.throws(function() {
  mdns.dns_sd.dnsServiceBrowse();
});

assert.throws(function() {
  mdns.dns_sd.dnsServiceBrowse("", 0, 0, "_node-mdns-test._tcp", null, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.dnsServiceBrowse(ref, "", 0, "_node-mdns-test._tcp", null, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.dnsServiceBrowse(ref, 0, "", "_node-mdns-test._tcp", null, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.dnsServiceBrowse(ref, 0, 0, null, null, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.dnsServiceBrowse(ref, 0, 0, 0, null, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.dnsServiceBrowse(ref, 0, 0, "_node-mdns-test._tcp", 0, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.dnsServiceBrowse(ref, 0, 0, "_node-mdns-test._tcp", null, 0, null);
});

// vim: filetype=javascript:
