#!/usr/bin/env node

var mdns  = require('../../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.dns_sd.DNSServiceRef();

mdns.dns_sd.DNSServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);

mdns.dns_sd.DNSServiceRefDeallocate(serviceRef);

assert.strictEqual(serviceRef.initialized, false);

// vim: filetype=javascript:
