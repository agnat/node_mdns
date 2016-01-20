# mdns -- node.js Service Discovery

* Package: mdns
* Description: multicast DNS service discovery
* Installation: `npm install mdns` (see below)
* Documentation: [mdns user guide](http://agnat.github.com/node_mdns/user_guide.html)
* License: [MIT](http://github.com/agnat/node_mdns/blob/master/LICENSE)
* Donations: [![Flattr this git repository](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=agnat&url=https://github.com/agnat/node_mdns&title=node_mdns&tags=github&category=software)

mdns adds multicast DNS service discovery, also known as zeroconf or bonjour to Node.js. It provides an object based interface to announce and browse services on the local network.

## Synopsis

```js
// import the module
var mdns = require('mdns');

// advertise a http server on port 4321
var ad = mdns.createAdvertisement(mdns.tcp('http'), 4321);
ad.start();

// watch all http servers
var browser = mdns.createBrowser(mdns.tcp('http'));
browser.on('serviceUp', function(service) {
  console.log("service up: ", service);
});
browser.on('serviceDown', function(service) {
  console.log("service down: ", service);
});
browser.start();

// discover all available service types
var all_the_types = mdns.browseThemAll(); // all_the_types is just another browser...
```

## Installation

On Linux and other systems using the avahi daemon the avahi dns_sd compat library and its header files are required.  On debianesque systems the package name is `libavahi-compat-libdnssd-dev`.  On other platforms Apple's [mDNSResponder](http://opensource.apple.com/tarballs/mDNSResponder/) is recommended. Care should be taken not to install more than one mDNS stack on a system.

On Windows you are going to need Apples "Bonjour SDK for Windows". You can download it either from Apple (registration required) or various unofficial sources. Take your pick. After installing the SDK restart your shell or command prompt and make sure the `BONJOUR_SDK_HOME` environment variable is set. You'll also need a compiler. Microsoft Visual Studio Express will do. On Windows node >=0.7.9 is required.

mdns is available as a npm package:

```sh
npm install mdns
```

If you want to play with the latest source code, here is a more manual approach:

```sh
git clone http://github.com/agnat/node_mdns
cd node_mdns
npm link && npm test
```

In case you want to run or even publish your package using the development version of mdns you may set the version field to a tarball URL:

```json
{ "name": "discomvobulator"
, "version": "0.0.1"
, "description": "covers all your discomvobulation needs"
, "dependencies":
  { "mdns": "https://github.com/agnat/node_mdns/tarball/master"
  }
}
```

## Documentation

See the [user guide](http://agnat.github.com/node_mdns/user_guide.html).

## Contributors

In random order:

* Orlando Vazquez ([orlandov](https://github.com/orlandov))
* Ryan Dahl ([ry](https://github.com/ry))
* Dominic Tarr ([dominictarr](https://github.com/dominictarr))
* Emil Stenqvist ([emilisto](https://github.com/emilisto))
* Toby Ealden ([TobyEalden](https://github.com/TobyEalden))
* Ron Korving ([ronkorving](https://github.com/ronkorving))
* Don Park ([donpark](https://github.com/donpark))
* Cong Liu ([ghostoy](https://github.com/ghostoy))
* Tian Zhang ([KhaosT](https://github.com/KhaosT))

Your name is missing on the list? Shame on me. Please open an issue.

## Bugs and Contributions

If you find a bug, please report it using the [issue tracker](http://github.com/agnat/node_mdns/issues).
