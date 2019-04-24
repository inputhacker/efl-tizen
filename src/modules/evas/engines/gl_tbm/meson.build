engine_src = files([
  'evas_engine.c',
  'evas_engine.h',
  'evas_tbm_main.c',
])

engine_header = [
    'Evas_Engine_GL_Tbm.h'
]

engine_deps = [gl_deps, libtbm]
engine_include_dir = include_directories(join_paths('..','software_generic'))

if get_option('evas-modules') == 'shared'
  shared_module(mod_full_name, engine_src + engine_header,
        include_directories : config_dir + [engine_include_dir],
        dependencies : [eina, evas] + engine_deps,
        install : true,
        install_dir : mod_install_dir,
        name_suffix : sys_mod_extension
      )

  install_headers(engine_header,
    install_dir: dir_package_include
  )

  module_files += join_paths(mod_install_dir, 'lib'+mod_full_name + '.' + sys_mod_extension)
endif
