#!/usr/bin/env node

var mdns  = require('../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();

assert.doesNotThrow( function() {
  mdns.dns_sd.DNSServiceEnumerateDomains(serviceRef,
    mdns.dns_sd.kDNSServiceFlagsBrowseDomains, 0, function() {}, null);
});

assert.notEqual(serviceRef.fd, -1);
assert.strictEqual(serviceRef.initialized, true);

// serviceRef already initialized
assert.throws(function() {
  mdns.dns_sd.DNSServiceEnumerateDomains(serviceRef,
    mdns.dns_sd.kDNSServiceFlagsBrowseDomains, 0, function() {}, null);
});

// flags must be a number, not a string
assert.throws(function() {
  var serviceRef = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceEnumerateDomains(serviceRef,
    'flags', 0, function() {}, null);
});

// flags must be kDNSServiceFlagsBrowseDomains or
// kDNSServiceFlagsRegistrationDomains
assert.throws(function() {
  var serviceRef = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceEnumerateDomains(serviceRef,
    0, 0, function() {}, null);
});

// interfaceIndex must be number, not a string
assert.throws(function() {
  var serviceRef = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceEnumerateDomains(serviceRef,
    mdns.dns_sd.kDNSServiceFlagsBrowseDomains, 'interfaceIndex', function() {}, null);
});

// callback must be function, not a string
assert.throws(function() {
  var serviceRef = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceEnumerateDomains(serviceRef,
    mdns.dns_sd.kDNSServiceFlagsBrowseDomains, 0, 'function', null);
});

// callback must be function, not null
assert.throws(function() {
  var serviceRef = new mdns.dns_sd.DNSServiceRef();
  mdns.dns_sd.DNSServiceEnumerateDomains(serviceRef,
    mdns.dns_sd.kDNSServiceFlagsBrowseDomains, 0, null, null);
});

// vim: filetype=javascript:
