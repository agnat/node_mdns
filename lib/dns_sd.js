var path = require('path')
  , buildtype = process.env.BUILDTYPE || 'Release'
  ;


function product(type) { 
  if (type === 'Coverage') {
    return path.join('..', 'dns_sd_bindings') 
  }
  return path.join('..', 'out', type, 'dns_sd_bindings') 
}

try {
  module.exports = require(product(buildtype));
} catch (ex) {
  if (! ex.code) {
    if (/not find/.test(ex)) {
      ex.code = 'MODULE_NOT_FOUND';
    }
  }
  if (ex.code === 'MODULE_NOT_FOUND') {
    module.exports = require(product('Release'));
    console.warn('dns_sd: failed to load requested ', buildtype, " build. using Release instead.");
  } else {
    throw ex;
  }
}
