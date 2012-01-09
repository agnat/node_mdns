#!/usr/bin/env node

var mdns   = require('../../lib/mdns'),
    util   = require('util'),
    assert = require('assert');

assert.strictEqual( mdns.udp('osc').toString(), '_osc._udp');

assert.strictEqual( mdns.tcp('http', 'blogapi').toString(), '_http._tcp,_blogapi');
assert.strictEqual( mdns.tcp('http', 'blogapi', 'newblogapi').toString(),
    '_http._tcp,_blogapi,_newblogapi');

function compareRegType() {
  var args = Array.prototype.slice.call(arguments)
    , str = args.pop()
    , regtype = mdns.makeRegType.apply(this, args)
    ;
  assert.strictEqual(regtype.toString(), str);
}

var http_regtype = '_http._tcp,_foo,_bar';
compareRegType(http_regtype,                      http_regtype);
compareRegType('http', 'tcp', 'foo', 'bar',       http_regtype);
compareRegType('_http', '_tcp', '_foo', '_bar',   http_regtype);
compareRegType(['_http', '_tcp', '_foo', '_bar'], http_regtype);
compareRegType(mdns.tcp('http', 'foo', 'bar'),    http_regtype);
compareRegType(
  { serviceType: 'http'
  , protocol: 'tcp'
  , subtypes: ['foo', 'bar']
  }
  ,                                               http_regtype);
compareRegType( mdns.makeRegType(
      JSON.parse(JSON.stringify(mdns.makeRegType(http_regtype))))
    , http_regtype);

compareRegType(new mdns.RegType('http', 'tcp', 'foo', 'bar'),    http_regtype);
compareRegType(new mdns.RegType(['http', 'tcp', 'foo', 'bar']),    http_regtype);

assert.throws(function() { mdns.tcp('abcdefghijklmnopq') });
assert.throws(function() { mdns.tcp('abc%') });
assert.throws(function() { mdns.tcp('abc', 'abcdefghijklmnopq') });
assert.throws(function() { mdns.tcp('abc', '%$@') });
assert.throws(function() { mdns.tcp('abc', 'def', 'foo_bar') });

assert.throws(function() { mdns.tcp('abc', 'tcp', 'foo_bar') });

assert.throws(function() { mdns.makeRegType({}) });
assert.throws(function() { mdns.makeRegType({serviceType: 'foo', protocol: 'bar'}) });
assert.throws(function() { mdns.makeRegType(5) });
assert.throws(function() { mdns.tcp('') });

// vim: filetype=javascript:
