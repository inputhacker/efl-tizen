if dependency('libtbm', required: false).found() == true and dependency('libdrm', required: false).found()
  ecore_buffer_x11_dri3_mod = shared_module('module.so',
    'ecore_buffer_x11_dri3.c',
    dependencies: [ecore, ecore_buffer, ecore_x],
    install: true,
    install_dir: join_paths(dir_lib, 'ecore_buffer', 'modules', 'x11_dri3', version_name)
  )
endif