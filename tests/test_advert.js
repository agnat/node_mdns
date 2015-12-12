var st = require('../lib/service_type.js')
  , dns_sd = require('../lib/dns_sd')
  , proxyquire =  require('proxyquire')
  , sinon = require('sinon')
  ;

//=== Browser ===========================================================

module.exports["Advertisement"] = {
  "Should pass error to callback when error occurs during service registration": function(test) {
    var callback = null
      , invocations = 0
      , threwException = false
      , serviceType = "foo"
      , port = "bar"
      , options = "baz"
      , error = new Error("Fail!")
      , code = 10;

    var mock_dns_sd = {
      buildException: sinon.stub().withArgs(code).returns(error),
      DNSServiceRegister: sinon.stub().callsArgWithAsync(9, null, null, code, null, null, null, null)
    };

    var mock_service_type = {
      makeServiceType: sinon.stub()
    }

    // create the browser with our mock of dns_sd
    var ad = proxyquire('../lib/advertisement.js', {
      './dns_sd': mock_dns_sd,
      './service_type': mock_service_type
    }).Advertisement.create(serviceType, port, options, function(er) {
      test.equal(error, er);

      test.done();
    });
  },

  "Should emit an error when when error occurs during service registration and no callbacked is passed": function(test) {
    var callback = null
      , invocations = 0
      , threwException = false
      , serviceType = "foo"
      , port = "bar"
      , options = "baz"
      , error = new Error("Fail!")
      , code = 10;

    var mock_dns_sd = {
      buildException: sinon.stub().withArgs(code).returns(error),
      DNSServiceRegister: sinon.stub().callsArgWithAsync(9, null, null, code, null, null, null, null)
    };

    var mock_service_type = {
      makeServiceType: sinon.stub()
    }

    // create the browser with our mock of dns_sd
    var ad = proxyquire('../lib/advertisement.js', {
      './dns_sd': mock_dns_sd,
      './service_type': mock_service_type
    }).Advertisement.create(serviceType, port, options)

    ad.on('error', function(er) {
      test.equal(error, er);

      test.done();
    });
  }
}
