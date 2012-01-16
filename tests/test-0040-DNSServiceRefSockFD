#!/usr/bin/env node

var mdns  = require('../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();

// serviceRef uninitialized -> no file descriptor
assert.throws(function() {
  mdns.dns_sd.DNSServiceRefSockFD(serviceRef)
});

mdns.dns_sd.DNSServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);

var fd;
// serviceRef initialized -> now we have a descriptor
assert.doesNotThrow(function() {
  fd = mdns.dns_sd.DNSServiceRefSockFD(serviceRef)
});

assert.notEqual(fd, -1);

// compare result with result of getter
assert.strictEqual(serviceRef.fd, fd);

// argument must be a serviceRef, not a string
assert.throws(function() {
  mdns.dns_sd.DNSServiceRefSockFD("narf");
});

// vim: filetype=javascript:
