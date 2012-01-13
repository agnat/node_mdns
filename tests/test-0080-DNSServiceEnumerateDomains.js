#!/usr/bin/env node

var mdns  = require('../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();

assert.doesNotThrow( function() {
  mdns.dns_sd.DNSServiceEnumerateDomains(serviceRef, mdns.dns_sd.kDNSServiceFlagsBrowseDomains, 0, function() {}, null);
});

assert.notEqual(serviceRef.fd, -1);
assert.strictEqual(serviceRef.initialized, true);

assert.throws(function() {
  var ref = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceRegister();
});

// vim: filetype=javascript:
