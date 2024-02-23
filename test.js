// import the module
const mdns = require('./');

// advertise a http server on port 4321
// const ad = mdns.createAdvertisement(mdns.tcp('http'), 4321);
// ad.start();

// watch all http servers
const browser = mdns.createBrowser(
    new mdns.ServiceType({
        name: 'androidtvremote2',
        protocol: 'tcp',
    }),
);
browser.on('serviceUp', service => {
  console.log("service up: ", service);
});
browser.on('serviceDown', service => {
  console.log("service down: ", service);
});
browser.start();

// discover all available service types
const all_the_types = mdns.browseThemAll(); // all_the_types is just another browser...
