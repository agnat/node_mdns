#!/usr/bin/env node

var mdns  = require('../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();

assert.strictEqual(serviceRef.fd, -1);
assert.strictEqual(serviceRef.initialized, false);

// vim: filetype=javascript:
