var binding = require('./binding'),
    sys = require('sys');

function regtype(service_type, protocol) {
  var subtypes = [];
  if (Array.isArray(service_type)) {
    subtypes = service_type;
    service_type = subtypes.shift();
  }
  if (service_type.length > 14) {
    throw Error("service_type is longer than 14 characters");
  }
  subtypes.unshift('_' + service_type + '._' + protocol);
  return subtypes.join(',_');
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
  } else {
    ready_callback = function() {}
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
    flags: typeof(flags) == 'function' ? 0 : flags,
    interface_index: typeof(interface_index) == 'function' ? 0 : interface_index,
    name: typeof(name) == 'function' ? null : name,
    domain: typeof(domain) == 'function' ? null : domain,
    host: typeof(host) == 'function' ? null : host,
    txt_record: typeof(txt_record) == 'function' ? null : txt_record,
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
  return this.doStart(regtype(this.service_type, this.protocol));
}

exports.createBrowser = function(service_type, protocol) {
  var browser = new Browser();
  browser.service_type = service_type;
  browser.protocol = protocol;
  browser.addListener('changed', function(err, info, flags) {
    if (flags & binding.kDNSServiceFlagsAdd) {
      resolve(info, function(resolver_info) {
        browser.emit('serviceUp', resolver_info, flags);
      });
    } else {
      browser.emit('serviceDown', info, flags);
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
exports.regtype = regtype;

