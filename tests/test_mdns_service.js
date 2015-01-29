var dns_sd = require('../lib/dns_sd')
  , proxyquire =  require('proxyquire')
  ;

//=== MDNSService ===========================================================

module.exports["MDNSService"] = {
  "DNS errors should propagate": function(test) {
    var message = "Panic!"
    var mock_dns_sd = {
      DNSServiceProcessResult: function() {
        throw new Error(message);
      }
    };

    var MDNSService = proxyquire("../lib/mdns_service.js", { "./dns_sd": mock_dns_sd }).MDNSService;

    var service = new MDNSService();
    service.on("error", function(error) {
      test.equal(message, error.message);

      test.done();
    });
    service.watcher.start = function() {
      process.nextTick(this.callback.bind(null));
    }
    service.start();
  }
}
