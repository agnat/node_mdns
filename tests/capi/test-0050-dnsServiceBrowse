#!/usr/bin/env node

var mdns  = require('../../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.capi.DNSServiceRef();

assert.doesNotThrow(function() {
  mdns.capi.dnsServiceBrowse(serviceRef, 0, 0, "_node-mdns-test._tcp", null, function() {
    }, null);
});

assert.throws(function() {
  mdns.capi.dnsServiceBrowse(serviceRef, 0, 0, "_node-mdns-test._tcp", null, function() {
    }, null);
});

assert.throws(function() {
  mdns.capi.dnsServiceBrowse();
});

assert.throws(function() {
  mdns.capi.dnsServiceBrowse("", 0, 0, "_node-mdns-test._tcp", null, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceBrowse(ref, "", 0, "_node-mdns-test._tcp", null, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceBrowse(ref, 0, "", "_node-mdns-test._tcp", null, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceBrowse(ref, 0, 0, null, null, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceBrowse(ref, 0, 0, 0, null, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceBrowse(ref, 0, 0, "_node-mdns-test._tcp", 0, function() {
    }, null);
});

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceBrowse(ref, 0, 0, "_node-mdns-test._tcp", null, 0, null);
});

// vim: filetype=javascript:
