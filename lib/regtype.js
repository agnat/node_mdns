function RegType(/* ... */) {
  this.serviceType = '';
  this.protocol = '';
  this.subtypes = [];
  // XXX this.fullyQualified = false; 

  var args;
  if (arguments.length === 1) {
    args = arguments[0];
  } else if (arguments.length > 1) {
    args = Array.prototype.slice.call(arguments);
  }
  //console.log(args);
  if (args) {
    if (args.length === 1) {
      if (typeof args[0] === 'string') {
        this.fromString(args[0]);
      } else if (Array.isArray(args[0])) {
        this.fromArray(args[0]);
      } else if (typeof args[0] === 'object') {
        this.fromJSON(args[0]);
      } else {
        throw new Error('illegal argument')
      }
    } else if (args.length >= 2) {
      this.fromArray(args);
    } else { // zero arguments
      // uninitialized RegType ... fine with me
    }
  }
}

exports.RegType = RegType;

RegType.prototype.toString = function() {
  var type_string = _u(this.serviceType) + "." + _u(this.protocol);
  if ('fullyQualified' in this && this.fullyQualified) {
    type_string += '.'
  }
  if (this.subtypes.length > 0) {
    var subtypes = this.subtypes.map(function(t) { return _u(t) });
    subtypes.unshift(type_string);
    type_string = subtypes.join(',');
  }
  return type_string;
}

RegType.prototype.fromString = function fromString(string) {
  var subtypes = string.split(',')
    , primary_string = subtypes.shift()
    , service_tokens = primary_string.split('.')
    , service_type = service_tokens.shift()
    , protocol = service_tokens.shift()
    ;
  checkProtocolU(protocol);
  checkFormat(service_type);
  subtypes.forEach(function(t) { checkFormat(t) });
  if (service_tokens.length === 1 && service_tokens[0] === '') {
    // trailing dot
    this.fullyQualified = true
  } else if (service_tokens.length > 0) {
    throw new Error("trailing tokens '" + service_tokens.join('.') + "' in " +
        "regtype string '" + string + "'");
  }

  this.serviceType = service_type.substr(1);
  this.protocol = protocol.substr(1);
  this.subtypes = subtypes.map(function(t){ return t.substr(1) });
}

RegType.prototype.toArray = function toArray() {
  return [this.serviceType, this.protocol].concat(this.subtypes);
}

RegType.prototype.fromArray = function fromArray(array) {
  var service_type = _uu(array.shift())
    , protocol = _uu(array.shift())
    , subtypes = array.map(function(t) { return _uu(t) })
    ;
  checkLengthAndCharset(service_type);
  checkProtocol(protocol);
  subtypes.forEach(function(t) { checkLengthAndCharset(t) });

  this.serviceType = service_type;
  this.protocol = protocol;
  this.subtypes = subtypes
}

RegType.prototype.fromJSON = function fromJSON(obj) {
  if ( ! ('serviceType' in obj)) {
    throw new Error('required property serviceType is missing');
  }
  if ( ! ('protocol' in obj)) {
    throw new Error('required property protocol is missing');
  }

  var service_type   = _uu(obj.serviceType)
    , protocol       = _uu(obj.protocol)
    , subtypes       = 'subtypes' in obj ? 
                        obj.subtypes.map(function(t) { return _uu(t) }) :
                        []
    ;

  checkLengthAndCharset(service_type);
  checkProtocol(protocol);
  subtypes.forEach(function(t) { checkLengthAndCharset(t) });

  this.serviceType = service_type;
  this.protocol = protocol;
  this.subtypes = subtypes;
  if ('fullyQualified' in obj) {
    this.fullyQualified = obj.fullyQualified;
  }
}

exports.makeRegType = function makeRegType() {
  if (arguments.length === 1 && arguments[0] instanceof RegType) {
    return arguments[0];
  }
  return new RegType(Array.prototype.slice.call(arguments));
}

exports.isProtocol = function isProtocol(str) {
  return str === 'tcp' || str === '_tcp' || str === 'udp' || str === '_udp';
}

function _u(str) { return "_" + str; }
function _uu(str) { return str[0] === '_' ? str.substr(1) : str; }

var charset_regex = /[^-a-zA-Z0-9]/;
function checkLengthAndCharset(str) {
  if (str.length === 0) {
    throw new Error('type ' + str + ' must not be empty');
  }
  if (str.length > 14) {
    throw new Error('type ' + str + ' has more than 14 characters');
  }
  if (str.match(charset_regex)) {
    throw new Error('type ' + str + ' may only contain alphanumeric ' + 
        'characters and hyphens');
  }
}

var format_regex = /_[-a-zA-Z0-9]+/;
function checkFormat(str) {
  if (str.length > 15) {
    throw new Error('type ' + str + ' has more than 14 characters');
  }
  if ( ! str.match(format_regex)) {
    throw new Error('type ' + str + ' must start with an underscore ' +
        'followed by alphanumeric characters and hyphens only');
  }
}

function checkProtocolU(str) {
  if ( ! (str === '_tcp' || str === '_udp')) {
    throw new Error("protocol must be either '_tcp' or '_udp' but is '" +
        str + "'");
  }
}

function checkProtocol(str) {
  if ( ! (str === 'tcp' || str === 'udp')) {
    throw new Error("protocol must be either '_tcp' or '_udp' but is '" +
        str + "'");
  }
}

