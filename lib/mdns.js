var dns_sd  = require('./dns_sd')
  , ad      = require('./advertisement')
  , browser = require('./browser')
  , st      = require('./service_type')
  ;

exports.dns_sd = dns_sd;

exports.Advertisement       = ad.Advertisement;
exports.createAdvertisement = ad.Advertisement.create;

exports.Browser       = browser.Browser;
exports.createBrowser = browser.Browser.create;
exports.browseThemAll = browser.browseThemAll;
exports.resolve       = browser.resolve;

exports.MDNSService = require('./mdns_service').MDNSService;

exports.ServiceType = st.ServiceType;
exports.makeServiceType = st.makeServiceType;
exports.tcp = st.protocolHelper('tcp');
exports.udp = st.protocolHelper('udp');

exports.dns_sd.exportConstants(exports);

exports.rst = require('./resolver_sequence_tasks');
