engine_src = files([
  'evas_gl_private.h',
  'evas_gl_common.h',
  'evas_gl_define.h',
  'evas_gl_context.c',
  'evas_gl_file_cache.c',
  'evas_gl_shader.c',
  'evas_gl_rectangle.c',
  'evas_gl_texture.c',
  'evas_gl_preload.c',
  'evas_gl_image.c',
  'evas_gl_font.c',
  'evas_gl_polygon.c',
  'evas_gl_line.c',
  'evas_gl_core.c',
  'evas_gl_api_ext.h',
  'evas_gl_api_ext_def.h',
  'evas_gl_core.h',
  'evas_gl_core_private.h',
  'evas_gl_api.c',
  'evas_gl_api_def.h',
  'evas_gl_api_gles1.c',
  'evas_gl_api_gles3_def.h',
  'evas_gl_api_ext.c',
  'evas_gl_3d_common.h',
  'evas_gl_3d_private.h',
  'evas_gl_3d.c',
  'evas_gl_3d_renderer.c',
  'evas_gl_3d_shader.c',
  #join_paths('shader_3d', 'evas_gl_3d_shaders.x'),
  #join_paths('shader','evas_gl_shaders.x'),
])

engine_src_gl_thread = files([
  'evas_gl_thread.h',
  'evas_gl_thread.c',
  'evas_gl_thread_egl.h',
  'evas_gl_thread_egl.c',
  'evas_gl_thread_glx.h',
  'evas_gl_thread_glx.c',
  'evas_gl_thread_gl.h',
  'evas_gl_thread_gl.c',
])

engine_src += engine_src_gl_thread

engine_deps = [gl_deps]

if get_option('evas-modules') == 'shared' and not evas_force_static.contains(engine)
  shared_module(mod_full_name, engine_src,
        include_directories : config_dir + [engine_include_dir],
        dependencies : [eina, evas_pre] + engine_deps,
        install : true,
        install_dir : mod_install_dir,
        name_suffix : sys_mod_extension
      )
      module_files += join_paths(mod_install_dir, 'lib'+mod_full_name + '.' + sys_mod_extension)
endif
