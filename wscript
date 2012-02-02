import os, shutil, subprocess, Scripting, Options

out = 'out'

def set_options(opt):
  opt.tool_options('compiler_cxx')
  opt.tool_options('node_addon')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')

  if conf.check(header_name='dns_sd.h', mandatory=True):
    conf.check(lib='dns_sd', uselib_store='DNS_SD')
 
    conf.check(function_name='DNSServiceGetAddrInfo',
               header_name="dns_sd.h",
               uselib='DNS_SD')

  # HACK: The current node master builds a i386 binary while older releases used
  # the compilers default x86_64. node-waf still targets x86_64 resulting in an
  # incompatible binary. Building the add-on as a universal binary for both
  # architectures solves this for now ...
  if conf.env.DEST_OS == 'darwin':
    universal_flags = ['-arch', 'i386', '-arch', 'x86_64']
    conf.env.append_value('CXXFLAGS', universal_flags)
    conf.env.append_value('LINKFLAGS_MACBUNDLE', universal_flags)

def build(ctx):
  ctx.add_subdirs('src')

def distclean(ctx):
  # TODO: find out how to use waf's node class here ...
  Scripting.distclean(ctx)
  symlink = os.path.join('lib', 'dns_sd.node')
  if os.path.lexists(symlink):
    os.unlink(symlink)
  if os.path.exists('node_modules'):
      shutil.rmtree('node_modules')

def test(ctx):
  subprocess.call(['utils/testrun'])

# vim: set filetype=python :
