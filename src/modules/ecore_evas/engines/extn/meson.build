engine_src = files(['ecore_evas_extn.c', 'ecore_evas_extn_buf.c', 'ecore_evas_extn_engine.h'])
engine_deps = [ecore_input, ecore_ipc]

# TIZEN_ONLY
engine_deps += [ecore_wl2, dependency('tizen-extension-client')]
# END

shared_module(mod_full_name, engine_src,
  include_directories : config_dir + [engine_include_dir],
  dependencies : [eina, ecore_evas, ecore_input_evas, rt] + engine_deps,
  install : true,
  install_dir : mod_install_dir,
  name_suffix : sys_mod_extension
)
