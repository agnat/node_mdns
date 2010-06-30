import sys
import Options
from os import unlink, link, popen
from os.path import exists 

srcdir = '.'
blddir = 'build'
VERSION = '0.0.1'

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')
  if sys.platform != 'darwin':
    conf.env.LIB_DNSSD = 'dns_sd'


def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = 'binding'
  obj.source = 'src/binding.cc', 'src/mdns_utils.cpp', 'src/mdns_base.cpp', \
    'src/advertisement.cpp', 'src/browser.cpp', 'src/resolver.cpp'
  obj.uselib = "DNSSD"
                

def shutdown():
  # HACK to get binding.node out of build directory.
  # better way to do this?
  if Options.commands['clean']:
    if exists('lib/mdns/binding.node'): unlink('lib/mdns/binding.node')
  else:
    if exists('build/default/binding.node') and not exists('lib/binding.node'):
      link('build/default/binding.node', 'lib/binding.node')

