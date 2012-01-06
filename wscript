import os

# nice, but requires python 2.6 ... 
#import json
#package = json.load(open('package.json'))
#APPNAME = 'node_' + package['name'] # used by 'node-waf dist'
#VERSION = package['version']        # dito

APPNAME = 'node_mdns'
VERSION = '0.0.4'

def set_options(opt):
  opt.tool_options('compiler_cxx')
  opt.tool_options('node_addon')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')

  if conf.check(header_name='dns_sd.h', mandatory=True):
    conf.define('NODE_MDNS_DNS_SD_BACKEND', 1)
 
    conf.check(lib='dns_sd', uselib_store='DNS_SD')
 
    conf.check(function_name='DNSServiceGetAddrInfo',
               header_name="dns_sd.h",
               uselib='DNS_SD')

  conf.write_config_header('node_mdns_config.h')

def build(ctx):
  ctx.add_subdirs('src')

def clean(ctx):
  symlink = os.path.join('lib', 'binding.node') # still not DRY ...
  if os.path.lexists(symlink):
    os.unlink(symlink)

# vim: set filetype=python :
