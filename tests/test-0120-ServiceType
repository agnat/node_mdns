#!/usr/bin/env node

var mdns   = require('../lib/mdns'),
    util   = require('util'),
    assert = require('assert');

assert.strictEqual( mdns.udp('osc').toString(), '_osc._udp');

assert.strictEqual( mdns.tcp('http', 'blogapi').toString(), '_http._tcp,_blogapi');
assert.strictEqual( mdns.tcp('http', 'blogapi', 'newblogapi').toString(),
    '_http._tcp,_blogapi,_newblogapi');

function compareServiceType() {
  var args = Array.prototype.slice.call(arguments)
    , str = args.pop()
    , type = mdns.makeServiceType.apply(this, args)
    ;
  assert.strictEqual(type.toString(), str);
}

var http_type = '_http._tcp,_foo,_bar';
compareServiceType(http_type,                         http_type);
compareServiceType('http', 'tcp', 'foo', 'bar',       http_type);
compareServiceType('_http', '_tcp', '_foo', '_bar',   http_type);
compareServiceType(['_http', '_tcp', '_foo', '_bar'], http_type);
compareServiceType(mdns.tcp('http', 'foo', 'bar'),    http_type);
compareServiceType(
  { name: 'http'
  , protocol: 'tcp'
  , subtypes: ['foo', 'bar']
  }
  ,                                               http_type);
compareServiceType( mdns.makeServiceType(
      JSON.parse(JSON.stringify(mdns.makeServiceType(http_type))))
    , http_type);

compareServiceType(new mdns.ServiceType('http', 'tcp', 'foo', 'bar'),    http_type);
compareServiceType(new mdns.ServiceType(['http', 'tcp', 'foo', 'bar']),    http_type);
compareServiceType(new mdns.ServiceType({name: 'http', protocol: 'tcp', subtypes: ['foo', 'bar']}), http_type);

assert.throws(function() { mdns.tcp('abcdefghijklmnopq') });
assert.throws(function() { mdns.tcp('abc%') });
assert.throws(function() { mdns.tcp('abc', 'abcdefghijklmnopq') });
assert.throws(function() { mdns.tcp('abc', '%$@') });
assert.throws(function() { mdns.tcp('abc', 'def', 'foo_bar') });

assert.throws(function() { mdns.tcp('abc', 'tcp', 'foo_bar') });

assert.throws(function() { mdns.makeServiceType({}) });
assert.throws(function() { mdns.makeServiceType({name: 'foo', protocol: 'bar'}) });
assert.throws(function() { mdns.makeServiceType(5) });
assert.throws(function() { mdns.tcp('') });

// vim: filetype=javascript:
