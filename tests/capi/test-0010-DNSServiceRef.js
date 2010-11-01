#!/usr/bin/env node

var mdns  = require('../../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.capi.DNSServiceRef();

assert.equal(serviceRef.fd, -1);
assert.equal(serviceRef.initialized, false);

// vim: filetype=javascript:
