#!/usr/bin/env node

var mdns  = require('../../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();

assert.throws(function() {
  mdns.dns_sd.dnsServiceRefSockFD(serviceRef)
});

mdns.dns_sd.dnsServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);

var fd;
assert.doesNotThrow(function() {
  fd = mdns.dns_sd.dnsServiceRefSockFD(serviceRef)
});

assert.notEqual(fd, -1);
assert.strictEqual(serviceRef.fd, fd);

assert.throws(function() {
  mdns.dns_sd.dnsServiceRefSockFD("narf");
});

// vim: filetype=javascript:
