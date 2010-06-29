binding = require('./binding');

function regtype(service_type, protocol /*, subtypes ... */) {
  var subtypes = Array.prototype.slice.call(arguments, 2);
  subtypes.unshift('_' + service_type + '._' + protocol);
  return subtypes.join(',');
}

//=== Advertisement ============================================================
var Advertisement = binding.Advertisement;
Advertisement.prototype.start = function() {
  return this.doStart(
    this.options.flags,
    this.options.interface_index,
    this.options.name,
    regtype(this.service_type, this.protocol),
    this.options.domain,
    this.options.host,
    this.port,
    this.options.txt_record,
    this.options.ready_callback);
};

exports.createAdvertisement = function(service_type, port, protocol,
    flags, interface_index, name, domain, host, txt_record, ready_callback)
{
  if ( undefined == service_type ) {
    throw TypeError("required argument 'service_type' is missing");
  }
  if ( undefined == port || typeof(port) == 'function') {
    throw TypeError("required argument 'port' is missing");
  }
  if (typeof(arguments[arguments.length - 1]) == 'function') {
    ready_callback = arguments[arguments.length - 1];
  }
  var ad = new Advertisement();
  ad.service_type = service_type;
  ad.port = port;
  if (protocol && typeof(protocol) != 'function') {
    ad.protocol = protocol;
  } else {
    ad.protocol = 'tcp';
  }
  ad.options = {
    flags: flags,
    interface_index: interface_index,
    name: name,
    domain: domain,
    host: host,
    txt_record: txt_record,
    ready_callback: ready_callback
  };
  return ad;
};

//=== Service Browser ==========================================================

function resolve(service_info, handler) {
  var resolver = new Resolver();
  resolver.addListener('resolved', function(error, resolver_info) {
    for (var property in resolver_info) {
      service_info[property] = resolver_info[property];
    }
    handler(service_info);
    resolver.stop();
  });
  resolver.start(service_info)
}

var Browser = binding.Browser;
Browser.prototype.start = function() {
  return this.doStart('_' + this.type + '._' + this.protocol);
}

exports.createBrowser = function(type, protocol) {
  var browser = new Browser();
  browser.type = type;
  browser.protocol = protocol;
  browser.addListener('changed', function(err, flags, info) {
    if (flags & binding.kDNSServiceFlagsAdd) {
      resolve(info, function(resolver_info) {
        browser.emit('serviceUp', resolver_info);
      });
    } else {
      browser.emit('serviceDown', info);
    }
  });
  return browser;
};

//=== Resolver =================================================================

var Resolver = binding.Resolver;

Resolver.prototype.start = function(service_info) {
  this.doStart(service_info.name, service_info.regtype, service_info.domain,
      service_info.interface_index, 0);
}

exports.resolve = resolve;
//=== Constants ================================================================

exports.TCP = "tcp";
exports.UDP = "udp";

