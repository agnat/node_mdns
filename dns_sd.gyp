{ 'targets': [
    { 'target_name': 'dns_sd'
    , 'sources': [ 'src/dns_sd.cpp'
                 , 'src/dns_service_browse.cpp'
                 , 'src/dns_service_enumerate_domains.cpp'
                 , 'src/dns_service_get_addr_info.cpp'
                 , 'src/dns_service_process_result.cpp'
                 , 'src/dns_service_ref.cpp'
                 , 'src/dns_service_ref_deallocate.cpp'
                 , 'src/dns_service_ref_sock_fd.cpp'
                 , 'src/dns_service_register.cpp'
                 , 'src/dns_service_resolve.cpp'
                 , 'src/mdns_utils.cpp'
                 , 'src/txt_record_ref.cpp'
                 , 'src/txt_record_create.cpp'
                 , 'src/txt_record_deallocate.cpp'
                 , 'src/txt_record_set_value.cpp'
                 , 'src/txt_record_get_length.cpp'
                 , 'src/txt_record_buffer_to_object.cpp'
                 ]
    , 'conditions': [
        [ 'OS!="mac" and OS!="win"', {
          'libraries': [ '-ldns_sd' ]
        }]
      , ['OS=="win"', {
          'sources'     : [ 'src/winsock_watcher.cpp' ]
        , 'include_dirs': [ '$(BONJOUR_SDK_HOME)Include' ]
        , 'libraries'   : [ '-l$(BONJOUR_SDK_HOME)Lib/$(Platform)/dnssd.lib'
                          , '-lws2_32.lib'
                          ]
        }]
      ]
    , 'msbuild_settings': {
        'ClCompile': { 'ExceptionHandling': 'Sync' }
      , 'Link'     : { 'IgnoreSpecificDefaultLibraries': [ 'LIBCMT' ] }
      }
    , 'configurations': {
        'Release': {
            'xcode_settings': { 'GCC_OPTIMIZATION_LEVEL': 3 }
        }
      , 'Debug': {
            'xcode_settings': { 'GCC_OPTIMIZATION_LEVEL': 0 }
        }
      , 'Coverage': {
            'xcode_settings': {
                'GCC_OPTIMIZATION_LEVEL': 0
              , 'OTHER_LDFLAGS': ['--coverage']
              , 'OTHER_CFLAGS':  ['--coverage']
            }
        }
      }
    , 'actions': [ {
        'action_name': 'jslib'
      , 'inputs': [ 'lib/advertisement.js'
                  , 'lib/browser.js'
                  , 'lib/io_watcher.js'
                  , 'lib/mdns.js'
                  , 'lib/mdns_service.js'
                  , 'lib/mdns_test.js'
                  , 'lib/resolver_sequence_tasks.js'
                  , 'lib/service_type.js'
                  ]
      , 'outputs': [ '<(PRODUCT_DIR)/advertisement.js'
                   , '<(PRODUCT_DIR)/browser.js'
                   , '<(PRODUCT_DIR)/io_watcher.js'
                   , '<(PRODUCT_DIR)/mdns.js'
                   , '<(PRODUCT_DIR)/mdns_service.js'
                   , '<(PRODUCT_DIR)/mdns_test.js'
                   , '<(PRODUCT_DIR)/resolver_sequence_tasks.js'
                   , '<(PRODUCT_DIR)/service_type.js'
                   ]
      , 'conditions': [
          [ 'OS=="win"',
            { 'action': ['xcopy', '/e', '/f', '/y', '<@(_inputs)',  '<(PRODUCT_DIR)'] }
          , { 'action': ['cp', '-v', '<@(_inputs)', '<(PRODUCT_DIR)'] }
          ]
        ]
      }]
    }
  ]
}

# vim: filetype=python shiftwidth=2 softtabstop=2 :
