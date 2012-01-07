#!/usr/bin/env node
var express = require('express')
  , mdns    = require('../lib/mdns')
  , app     = express.createServer()
  ;

app.get('/', function() { return "Hello World"; });

app.on('listening', function() {
  mdns.createAdvertisement(mdns.tcp('http') , app.address().port ).start();
});

app.listen(4321);
