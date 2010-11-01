#!/usr/bin/env node

var mdns   = require('../../lib/mdns'),
    util   = require('util'),
    assert = require('assert');

assert.strictEqual( mdns.udp('osc').toString(), '_osc._udp');

assert.strictEqual( mdns.tcp('http', 'blogapi').toString(), '_http._tcp,_blogapi');
assert.strictEqual( mdns.tcp('http', 'blogapi', 'newblogapi').toString(),
    '_http._tcp,_blogapi,_newblogapi');

assert.throws(function() { mdns.tcp('abcdefghijklmnopq') });
assert.throws(function() { mdns.tcp('abc%') });
assert.throws(function() { mdns.tcp('abc', 'abcdefghijklmnopq') });
assert.throws(function() { mdns.tcp('abc', '%$@') });
assert.throws(function() { mdns.tcp('abc', 'def', '_narf') });

// vim: filetype=javascript:
