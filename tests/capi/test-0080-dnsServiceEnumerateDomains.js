#!/usr/bin/env node

var mdns  = require('../../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.capi.DNSServiceRef();

assert.doesNotThrow( function() {
  mdns.capi.dnsServiceEnumerateDomains(serviceRef, mdns.capi.kDNSServiceFlagsBrowseDomains, 0, function() {}, null);
});

assert.notEqual(serviceRef.fd, -1);
assert.strictEqual(serviceRef.initialized, true);

assert.throws(function() {
  var ref = new mdns.capi.DNSServiceRef();
  mdns.capi.dnsServiceRegister();
});

// vim: filetype=javascript:
