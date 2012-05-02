exports.IOWatcher = process.platform === 'win32'
                  ? require('./dns_sd').WinsockWatcher
                  : process.binding('io_watcher').IOWatcher
