#!/usr/bin/env node

var mdns   = require('../../lib/mdns'),
    util   = require('util'),
    assert = require('assert');

var timeout = 5000;
var timeoutId = setTimeout(function() {
  assert.fail('test did not finish within ' + (timeout / 1000) + ' seconds');
}, timeout);

var enumerator = mdns.createDomainEnumerator(mdns.capi.kDNSServiceFlagsBrowseDomains, 0);
enumerator.on('domainChanged', function(domain, flags, interfaceIndex, context) {
  assert.strictEqual(typeof domain, 'string');
  assert.strictEqual(typeof flags, 'number');
  assert.strictEqual(typeof interfaceIndex, 'number');

  enumerator.stop();
  clearTimeout(timeoutId);
});

enumerator.start();

// vim: filetype=javascript
