#!/usr/bin/env node

var mdns  = require('../../lib/mdns'),
    assert = require('assert');

var serviceRef = new mdns.capi.DNSServiceRef();

mdns.capi.dnsServiceRegister(serviceRef, 0, 0, null, "_node-mdns-test._tcp",
    null, null, 4321, null, null, null);

mdns.capi.dnsServiceRefDeallocate(serviceRef);

assert.strictEqual(serviceRef.initialized, false);

// vim: filetype=javascript:
