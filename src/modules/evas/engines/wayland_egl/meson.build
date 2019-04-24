engine_src = files([
  'evas_engine.c',
  'evas_engine.h',
  'evas_wl_main.c'
])

foreach src : gl_thread_engine_src
  engine_src += files(join_paths('..','gl_common', src))
endforeach

engine_deps = [
   ecore_wl2, gl_deps,
   dependency('wayland-egl'),
   dependency('wayland-client'),
   dependency('wayland-egl-tizen'),
]

engine_include_dir = include_directories(join_paths('..','wayland_common'))


if get_option('evas-modules') == 'shared'
  shared_module(mod_full_name, engine_src,
        include_directories : config_dir + [engine_include_dir],
        dependencies : [eina, evas] + engine_deps,
        install : true,
        install_dir : mod_install_dir,
        name_suffix : sys_mod_extension
      )
      module_files += join_paths(mod_install_dir, 'lib'+mod_full_name + '.' + sys_mod_extension)
endif
