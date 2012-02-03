var mdns = require('../lib/mdns')
  ;

//=== DNSServiceRef ===========================================================

exports['DNSServiceEnumerateDomains'] = function(t) {
  var enumerator = new mdns.MDNSService();
  function on_enum(sref, flags, iface, error, domain, context) {
    t.strictEqual(error, mdns.kDNSServiceErr_NoError, "error must be NoError");
    t.strictEqual(typeof flags, 'number', "'flags' must be of type number");
    t.strictEqual(typeof iface, 'number', "'iface' must be of type number");

    enumerator.stop();

    t.done();
  }
  mdns.dns_sd.DNSServiceEnumerateDomains(enumerator.serviceRef,
      mdns.kDNSServiceFlagsBrowseDomains, 0, on_enum, {some: 'context'});
  enumerator.start();
}

