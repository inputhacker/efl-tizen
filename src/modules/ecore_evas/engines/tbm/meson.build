engine_src = files(['ecore_evas_tbm.c'])
engine_deps = [ecore_input, libtbm]

engine_include_dir += include_directories(join_paths('..', '..', '..', 'evas', 'engines', 'software_tbm'))
engine_include_dir += include_directories(join_paths('..', '..', '..', 'evas', 'engines', 'gl_tbm'))

shared_module(mod_full_name, engine_src,
  include_directories : config_dir + [engine_include_dir],
  dependencies : [eina, ecore_evas, ecore_input_evas] + engine_deps,
  install : true,
  install_dir : mod_install_dir,
  name_suffix : sys_mod_extension
)
