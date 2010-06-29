var express = require('express'),
    mdns    = require('mdns'),
    sys     = require('sys');

get('/', function() {
  return "Hello World";
});

var server = run();
server.addListener('listening', function() {
  mdns.createAdvertisement('http', Express.server.port, mdns.TCP).start();
});
