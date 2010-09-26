import sys
import Options
import os

# nice, but requires python 2.6 ... 
#import json
#package = json.load(open('package.json'))
#APPNAME = 'node_' + package['name'] # used by 'node-waf dist'
#VERSION = package['version']        # dito

APPNAME = 'node_mdns'
VERSION = '0.0.3'

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

  conf.write_config_header('node_mdns_config.h');


def post_build(ctx):
  #print("=== post")
  if not os.path.exists('lib/binding.node'):
      os.symlink( '../build/default/binding.node', 'lib/binding.node')

def build(bld):
  bld.add_post_fun(post_build)
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = 'binding'
  obj.uselib = 'DNSSD'
  obj.includes = '.'
  obj.source = ['src/binding.cpp',
                'src/dns_service_browse.cpp',
                'src/dns_service_enumerate_domains.cpp',
                'src/dns_service_process_result.cpp',
                'src/dns_service_ref.cpp',
                'src/dns_service_ref_deallocate.cpp',
                'src/dns_service_ref_sock_fd.cpp',
                'src/dns_service_register.cpp',
                'src/dns_service_resolve.cpp',
                'src/mdns_utils.cpp']
  if bld.env.HAVE_DNSSERVICEGETADDRINFO:
      obj.source.append('src/dns_service_get_addr_info.cpp')
  
# vim: set filetype=python :
