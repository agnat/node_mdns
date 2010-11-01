#!/usr/bin/env node

var mdns  = require('../../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.capi.DNSServiceRef();

assert.throws(function() {
  mdns.capi.dnsServiceRefSockFD(serviceRef)
});

mdns.capi.dnsServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);

var fd;
assert.doesNotThrow(function() {
  fd = mdns.capi.dnsServiceRefSockFD(serviceRef)
});

assert.notEqual(fd, -1);
assert.strictEqual(serviceRef.fd, fd);

assert.throws(function() {
  mdns.capi.dnsServiceRefSockFD("narf");
});

// vim: filetype=javascript:
