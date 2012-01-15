#!/usr/bin/env node

var dns_sd = require('../lib/dns_sd')
  , assert = require('assert')
  ;

var txtRecord = new dns_sd.TXTRecordRef();
dns_sd.TXTRecordCreate(txtRecord, null);

var txtRecord = new dns_sd.TXTRecordRef();
var buffer = new Buffer(256);
dns_sd.TXTRecordCreate(txtRecord, buffer);
txtRecord.buffer = buffer;

dns_sd.TXTRecordSetValue(txtRecord, 'foo', 'bar');
assert.strictEqual(dns_sd.TXTRecordGetLength(txtRecord), 8);
dns_sd.TXTRecordSetValue(txtRecord, 'foobar', 'foobar');
assert.strictEqual(dns_sd.TXTRecordGetLength(txtRecord), 22);

assert.throws(function() { dns_sd.TXTRecordCreate() });
assert.throws(function() { dns_sd.TXTRecordCreate('narf') });
assert.throws(function() { dns_sd.TXTRecordCreate(txtRecord) });

//assert.strictEqual(dns_sd.TXTRecordGetCount(record_buffer), 1);

dns_sd.TXTRecordDeallocate( txtRecord );

process.nextTick(function() { /*console.log('done')*/ });
