#!/usr/bin/env node

var mdns   = require('../lib/mdns')
  , util   = require('util')
  ;

// XXX This test will break if two instances run on the same network

exports['simple browsing'] = function(t) {
  var timeout = 5000;
  var timeoutId = setTimeout(function() {
    t.fail("test did not finish within " + (timeout / 1000) + " seconds.");
    t.done();
  }, timeout)

  var browser = mdns.createBrowser(mdns.tcp('node-mdns-test'));

  function stopBrowserIfDone() {
    if (upCount > 0 &&
        downCount > 0 && 
        upCount == downCount && 
        changedCount == upCount + downCount)
    {
      browser.stop();
      clearTimeout(timeoutId);
      t.done();
    }
  }

  var changedCount = 0;
  browser.on('serviceChanged', function(service, flags) {
    //console.log("changed:", service);
    t.strictEqual(typeof service.flags, 'number');
    if (changedCount === 0) {
      t.ok(service.flags & mdns.kDNSServiceFlagsAdd);
      t.strictEqual(typeof service.fullname, 'string');
      t.strictEqual(typeof service.host, 'string');
      t.strictEqual(typeof service.port, 'number');
      t.strictEqual(service.port, 4321);

      t.ok('addresses' in service);
      t.ok(Array.isArray(service.addresses));
      t.ok(service.addresses.length > 0);
    }
    t.strictEqual(typeof service.interfaceIndex, 'number');
    t.strictEqual(typeof service.name, 'string');
    t.ok(service.type instanceof mdns.ServiceType);
    t.strictEqual('' + service.type, '_node-mdns-test._tcp.');
    t.strictEqual(typeof service.replyDomain, 'string');
    t.strictEqual(service.replyDomain, 'local.');

    changedCount += 1;
    stopBrowserIfDone();
  });

  var upCount = 0;
  browser.on('serviceUp', function(service) {
    //console.log("up:", service);
    t.strictEqual(typeof service.flags, 'number');
    t.strictEqual(typeof service.interfaceIndex, 'number');
    t.strictEqual(typeof service.name, 'string');
    t.ok(service.type instanceof mdns.ServiceType);
    t.strictEqual('' + service.type, '_node-mdns-test._tcp.');
    t.strictEqual(typeof service.replyDomain, 'string');
    t.strictEqual(service.replyDomain, 'local.');

    t.strictEqual(typeof service.fullname, 'string');
    t.strictEqual(typeof service.host, 'string');
    t.strictEqual(typeof service.port, 'number');
    t.strictEqual(service.port, 4321);

    t.ok('addresses' in service);
    t.ok(Array.isArray(service.addresses));
    t.ok(service.addresses.length > 0);

    t.ok('rawTxtRecord' in service);
    t.ok(service.rawTxtRecord);
    t.ok(service.txtRecord);
    var p;
    for (p in txt_record) {
      t.strictEqual('' + txt_record[p], service.txtRecord[p]);
    }
    
    upCount += 1;
    stopBrowserIfDone();
  });

  var downCount = 0;
  browser.on('serviceDown', function(service) {
    t.strictEqual(typeof service.flags, 'number');
    t.strictEqual(typeof service.interfaceIndex, 'number');
    t.strictEqual(typeof service.name, 'string');
    t.ok(service.type instanceof mdns.ServiceType);
    t.strictEqual('' + service.type, '_node-mdns-test._tcp.');
    t.strictEqual(typeof service.replyDomain, 'string');
    t.strictEqual(service.replyDomain, 'local.');

    downCount += 1;
    stopBrowserIfDone();
  });

  browser.start();

  var txt_record = {type: 'bacon', chunky: true, strips: 5, buffer: new Buffer('raw')}
    , ad = mdns.createAdvertisement(mdns.tcp('node-mdns-test'), 4321,
        {txtRecord: txt_record}, function(err, service, flags) {
          if (err) throw err;
          setTimeout(function() { ad.stop() }, 500);
        });

  ad.start();
}


/*
exports['another browser test'] = function(t) {
  mdns_browser_test(
      []
    , []
    , { up: function() {}
      , down: function() {}
      , changed: function() {}
      }
  );
}
*/
