if (process.platform === 'win32') {
  var IOWatcher = exports.IOWatcher = require('./dns_sd').WinsockWatcher;

  IOWatcher.prototype.start = function start() {}
  IOWatcher.prototype.stop  = function stop() {}

} else {
  exports.IOWatcher = process.binding('io_watcher').IOWatcher;
}
