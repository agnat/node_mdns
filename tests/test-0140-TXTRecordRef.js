#!/usr/bin/env node

var dns_sd = require('../lib/dns_sd')
  , assert = require('assert')
  ;

var txtRecord = new dns_sd.TXTRecordRef();

dns_sd.TXTRecordCreate(txtRecord, null);


