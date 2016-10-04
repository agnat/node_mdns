#!/usr/bin/env node
var mdns    = require('../lib/mdns')
    ,express = require('express');

var app = express();
app.get('/', function(req, res){
  res.send('Hello World');
});

var listener = app.listen(4321, function() {
    var port = listener.address().port;
    mdns.createAdvertisement(mdns.tcp('http') , port).start();
    console.log('Listening on port', port);
});
