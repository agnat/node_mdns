#!/usr/bin/env node
var express = require('express'),
    mdns    = require('../lib/mdns'),
    util    = require('util');

get('/', function() {
  return "Hello World";
});

var server = run();
server.addListener('listening', function() {
  mdns.createAdvertisement('http', Express.server.port, 'tcp').start();
});
