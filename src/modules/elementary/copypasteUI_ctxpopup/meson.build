src = files([
  'cbhm_helper.h',
  'cbhm_helper.c',
  'copypaste.c',
])

shared_module(mod,
    src,
    dependencies: [elementary, ecore_wl2, eio],
    install: true,
    install_dir : mod_install_dir
)
