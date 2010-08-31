function checkLengthAndCharset(str) {
  if (str.length > 14) {
    throw new Error('type ' + str + ' has more than 14 characters');
  }
  var re = new RegExp('[^-a-zA-Z0-9]');
  if (str.match(re)) {
    throw new Error('type ' + str + ' may only consist of alphanumeric characters and hyphens');
  }
}

function RegType(types, transportProtocol) {
  var self = this;
  if (! (types instanceof Array)) {
    types = [types];
  }
  types = types.slice();
  if (types.length < 1) {
    throw new Error('no service name given');
  }
  self.serviceType = types.shift();
  
  checkLengthAndCharset(self.serviceType);

  self.subtypes = types;

  self.subtypes.forEach(function(t) { checkLengthAndCharset(t); });

  self.transportProtocol = transportProtocol || 'tcp';
}

exports.RegType = RegType;

function _u(str) { return str[0] == '_' ? str : "_" + str; }

RegType.prototype.toString = function() {
  if (this.subtypes.length > 0) {
    var primaryType = _u(this.serviceType) + "." + _u(this.transportProtocol);
    var subtypes = this.subtypes.map(function(t) { return _u(t) });
    subtypes.unshift(primaryType);
    return subtypes.join(',');
  } else {
    return _u(this.serviceType) + "." + _u(this.transportProtocol);
  }
}


