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

exports['testing DNSServiceRegister(...)'] = function(t) {

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

  t.throws(function() { dns_sd.DNSServiceRegister(); },
      'Call with zero arguments must throw');

  t.throws(function() {
    var a1, a2, a3, a4, a5, a6, a7, a8;
    dns_sd.DNSServiceRegister(a1, a2, a3, a4, a5, a6, a7, a7);
  },  'Call with eight arguments must throw');

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
  },  '"flags" must be a number, not a string');

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
  },  '"interfaceIndex" must be a number, not a string');

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
  },  '"name" must be a string, not a number');

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
  },  '"regtype" must be a string, not null');

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
  },  '"regtype" has to be a string, not a number');

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
  },  '"domain" must not be a string, not a number');

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
  },  '"host" must be a string, not a number');

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
  },  '"port" must be a number, not a string');

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
  },  '"txtRecord" must be a TXTRecordRef or buffer, not a number');

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
  },  '"callback" must be a function, not a string');

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
  },  'port number must be <= ' + ((1 << 16) - 1));

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
  },  'port number must >= 0');
  t.done();
}

















// vim: filetype=javascript:

