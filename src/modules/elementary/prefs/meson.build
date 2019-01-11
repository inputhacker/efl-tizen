src = files([
  'private.h',
  'prefs_iface.c',
  'elm_button.c',
  'elm_check.c',
  'elm_datetime.c',
  'elm_entry.c',
  'elm_label.c',
  'elm_separator.c',
  'elm_slider.c',
  'elm_spinner.c',
  'elm_swallow.c',
  'elm_vertical_box.c',
  'elm_horizontal_box.c',
  'elm_vertical_frame.c',
  'elm_horizontal_frame.c'
])

edc_files = [
  'elm_prefs_swallow.edc',
]

foreach edc_file : edc_files
  custom_target('edje_cc_' + edc_file,
    input : edc_file,
    output : '@BASENAME@.edj',
    command : edje_cc_exe + [ '-beta', '-fastcomp',
              '-id', meson.current_source_dir(),
              '-fd', meson.current_source_dir(),
              '-sd', meson.current_source_dir(),
              '-vd', meson.current_source_dir(),
              '-dd', meson.current_source_dir(),
              '-md', meson.current_source_dir(),
              '-td', meson.current_source_dir(),
              '@INPUT@', '@OUTPUT@'],
    depends : edje_cc,
    install: true,
    install_dir : mod_install_dir)
endforeach

shared_module(mod,
    src,
    dependencies: [elementary, ecore_wl2, eio],
    install: true,
    install_dir : mod_install_dir
)
