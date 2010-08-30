function RegType(types, transportProtocol) {
  var self = this;
  self.serviceType = types.shift();
  self.subtypes = types;
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


