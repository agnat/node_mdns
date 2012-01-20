var dns_sd       = require('../lib/mdns').dns_sd
  , service_type = "_mdns_test._tcp"
  , port         = 4321
  ;

//=== DNSServiceRef ===========================================================

exports['testing DNSServiceRef constructor'] = function(t) {
  
  var sr = new dns_sd.DNSServiceRef();

  t.ok(sr,
      'DNSServiceRef must be truthy');
  t.strictEqual(sr.fd, -1,
      'File descriptor must be -1');
  t.strictEqual(sr.initialized, false,
      'DNSServiceRef must not be initialized');
  t.done();
}

//=== DNSServiceRegister ======================================================

exports['DNSServiceRegister(...)'] = function(t) {
  var serviceRef = new dns_sd.DNSServiceRef();

  t.doesNotThrow(function() {
    var // sr = new dns_sd.DNSServiceRef()
        flags  = 0
      , iface  = 0
      , name   = null
      , type   = service_type
      , domain = null
      , host   = null
      , port   = 4321
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister( serviceRef, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  'Call with minimal argumentss must succeed.');

  t.notStrictEqual(serviceRef.fd, -1,
      'File descriptor must not be -1 after initialization');

  t.strictEqual(serviceRef.initialized, true,
      'DNSServiceRef must be initialized');

  t.throws(function() {
    var /* uses parent scope service ref */
        flags  = 0
      , iface  = 0
      , name   = null
      , type   = service_type
      , domain = null
      , host   = null
      , port   = 4321
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister( serviceRef, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  'Duplicate initialization of DNSServiceRef must throw');

  t.doesNotThrow(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = 'My sh4red stuff'
      , type   = service_type
      , domain = 'somedomain'
      , host   = 'somehost'
      , port   = 4321
      , txtRec = new Buffer('\0')
      , cb     = function() {}
      , ctx    = {anything: true}
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  'Call with all arguments on must succed.');

  t.throws(function() { dns_sd.DNSServiceRegister(); },
      'Call with zero arguments must throw.');

  t.throws(function() {
    var a1, a2, a3, a4, a5, a6, a7, a8;
    dns_sd.DNSServiceRegister(a1, a2, a3, a4, a5, a6, a7, a7);
  },  'Call with eight arguments must throw.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = '=== KAPUTT ===' /* broken */
      , iface  = 0
      , name   = null
      , type   = service_type
      , domain = null
      , host   = null
      , port   = 4321
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  '"flags" must be a number, not a string.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = '=== KAPUTT ===' /* broken */
      , name   = null
      , type   = service_type
      , domain = null
      , host   = null
      , port   = 4321
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  '"interfaceIndex" must be a number, not a string.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = 1111111111 /* broken */
      , type   = service_type
      , domain = null
      , host   = null
      , port   = 4321
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  '"name" must be a string, not a number.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = null
      , type   = null /* broken */
      , domain = null
      , host   = null
      , port   = 4321
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  '"regtype" must be a string, not null.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = null
      , type   = 1111111111 /* broken */
      , domain = null
      , host   = null
      , port   = 4321
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  '"regtype" has to be a string, not a number.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = null
      , type   = service_type
      , domain = 1111111111 /* broken */
      , host   = null
      , port   = 4321
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  '"domain" must not be a string, not a number.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = null
      , type   = service_type
      , domain = null
      , host   = 1111111111 /* broken */
      , port   = 4321
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  '"host" must be a string, not a number.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = null
      , type   = service_type
      , domain = null
      , host   = null
      , port   = '=== KAPUTT ===' /* broken */
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  '"port" must be a number, not a string.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = null
      , type   = service_type
      , domain = null
      , host   = null
      , port   = 4321
      , txtRec = 1111111111 /* broken */
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  '"txtRecord" must be a TXTRecordRef or buffer, not a number.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = null
      , type   = service_type
      , domain = null
      , host   = null
      , port   = 4321
      , txtRec = null
      , cb     = '=== KAPUTT ===' /* broken */
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  '"callback" must be a function, not a string.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = null
      , type   = service_type
      , domain = null
      , host   = null
      , port   = 1 << 16 /* broken */
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  'port number must be <= ' + ((1 << 16) - 1) + '.');

  t.throws(function() {
    var ref    = new dns_sd.DNSServiceRef()
      , flags  = 0
      , iface  = 0
      , name   = null
      , type   = service_type
      , domain = null
      , host   = null
      , port   = -1    /* broken */
      , txtRec = null
      , cb     = null
      , ctx    = null
      ;
    dns_sd.DNSServiceRegister(ref, flags, iface, name, type, domain,
      host, port, txtRec, cb, ctx);
  },  'port number must >= 0.');

  t.done();
};

//=== DNSServiceProcessResult =================================================

exports['DNSServiceProcessResult(...)'] = function(t) {
  var serviceRef = new dns_sd.DNSServiceRef()
    , IOWatcher  = process.binding('io_watcher').IOWatcher
    , watcher    = new IOWatcher()
    , timeout    = 3000
    , timeoutId  = setTimeout(function() {
        t.fail("Test did not finish within " + (timeout/1000) + "s");
        watcher.stop();
        t.done();
  }, timeout);

  watcher.callback = function() {
    t.doesNotThrow(function() {
        dns_sd.DNSServiceProcessResult(serviceRef);
    },  "DNSServiceProcessResult() must not throw");
  }

  function result_callback(sdRef, flags, errorCode, name, serviceType, domain,
      context)
  {
    t.strictEqual(sdRef, serviceRef,
        'serviceRef must be identical');
    t.strictEqual(typeof flags, "number",
        '"flags" must be of type number');
    t.strictEqual(typeof errorCode, "number",
        '"errorCode" must be of type number');
    t.strictEqual(errorCode, dns_sd.kDNSServiceErr_NoError,
        '"errorCode" must be kDNSServiceErr_NoError');
    t.strictEqual(typeof name, "string",
        '"name" must be of type string');
    t.strictEqual(typeof serviceType, "string",
        '"serviceType" must be of type string');
    t.strictEqual(typeof domain, "string",
        '"domain" must be of type string');
    t.strictEqual(typeof context, "string",
        '"context" must be of type string (in this test)');
    t.strictEqual(context, "foobar",
      'expected "foobar" but got "' + context + '"');

    clearTimeout(timeoutId);
    watcher.stop();
    t.done();
  }

  dns_sd.DNSServiceRegister(serviceRef, 0, 0, null, service_type,
      null, null, 4321, null, result_callback, "foobar");

  watcher.set(serviceRef.fd, true, false);
  watcher.start();
}

//=== DNSServiceRefSockFD =====================================================

exports['DNSServiceRefSockFD(...)'] = function(t) {
  var serviceRef = new dns_sd.DNSServiceRef();

  t.throws(function() {dns_sd.DNSServiceRefSockFD(serviceRef)},
      'call with uninitialized serviceRef must throw');

  dns_sd.DNSServiceRegister(serviceRef, 0, 0, null, service_type,
      null, null, port, null, null, null);

  var fd;
  t.doesNotThrow(function() {fd = dns_sd.DNSServiceRefSockFD(serviceRef)});

  t.notEqual(fd, -1, 'file descriptor must not be -1');

  t.strictEqual(serviceRef.fd, fd,
      'result of DNSServiceRefSockFD() and fd getter must be the same');

  t.throws(function() { dns_sd.DNSServiceRefSockFD("narf"); },
      'argument must be a DNSServiceRef');

  t.done();
}

//=== DNSServiceBrowse ========================================================

exports['DNSServiceBrowse(...)'] = function(t) {
  var serviceRef = new dns_sd.DNSServiceRef();

  t.doesNotThrow(function() {
    dns_sd.DNSServiceBrowse(serviceRef, 0, 0, service_type, null,
      function() {}, null);
  });

  t.throws(function() {
    dns_sd.DNSServiceBrowse(serviceRef, 0, 0, service_type, null,
      function() {}, null);
  },  'serviceRef already initialized');

  t.throws(function() {
    dns_sd.DNSServiceBrowse();
  }, 'not enough arguments');

  t.throws(function() {
    dns_sd.DNSServiceBrowse("", 0, 0, service_type, null,
      function() {}, null);
  }, '"serviceRef" must not be a string');

  t.throws(function() {
    var ref = new dns_sd.DNSServiceRef();
    dns_sd.DNSServiceBrowse(ref, "", 0, service_type, null,
      function() {}, null);
  }, '"flags" must be a number, not a string');

  t.throws(function() {
    var ref = new dns_sd.DNSServiceRef();
    dns_sd.DNSServiceBrowse(ref, 0, "", service_type, null,
      function() {}, null);
  }, '"interfaceIndex" must be a number, not a string'
  );

  t.throws(function() {
    var ref = new dns_sd.DNSServiceRef();
    dns_sd.DNSServiceBrowse(ref, 0, 0, null, null, function() {}, null);
  }, '"regtype" must be a string, not null');

  t.throws(function() {
    var ref = new dns_sd.DNSServiceRef();
    dns_sd.DNSServiceBrowse(ref, 0, 0, 0, null, function() {}, null);
  }, '"regtype" must be a string, not a number');

  t.throws(function() {
    var ref = new dns_sd.DNSServiceRef();
    dns_sd.DNSServiceBrowse(ref, 0, 0, service_type, 0,
      function() {}, null);
  }, '"domain" must be a string, not a number');

  t.throws(function() {
    var ref = new dns_sd.DNSServiceRef();
    dns_sd.DNSServiceBrowse(ref, 0, 0, service_type, null, 0, null);
  }, '"callback" must be a function, not a number');

  t.done();
}

// vim: filetype=javascript:
