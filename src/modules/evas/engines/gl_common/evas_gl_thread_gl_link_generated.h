/*
 * This is an automatically generated file using a python script.
 * ($EFL_HOME/src/utils/evas/generate_gl_thread_api.py)
 * Recommend that you modify data files ($EFL_HOME/src/utils/evas/gl_api_def.txt)
 * and make use of scripts if you need to fix them.
 */
extern GLenum (*glGetError_thread_cmd)(void);
extern void (*glVertexAttribPointer_thread_cmd)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
extern void (*glEnableVertexAttribArray_thread_cmd)(GLuint index);
extern void (*glDisableVertexAttribArray_thread_cmd)(GLuint index);
extern void (*glDrawArrays_thread_cmd)(GLenum mode, GLint first, GLsizei count);
extern void (*glDrawElements_thread_cmd)(GLenum mode, GLsizei count, GLenum type, const void *indices);
extern void (*glGenBuffers_thread_cmd)(GLsizei n, GLuint *buffers);
extern void (*glDeleteBuffers_thread_cmd)(GLsizei n, const GLuint *buffers);
extern void (*glBindBuffer_thread_cmd)(GLenum target, GLuint buffer);
extern void (*glBufferData_thread_cmd)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
extern void (*glMapBuffer_orig_evas_set)(void *func);
extern void *(*glMapBuffer_orig_evas_get)(void);
extern void * (*glMapBuffer_thread_cmd)(GLenum target, GLenum access);
extern void (*glUnmapBuffer_orig_evas_set)(void *func);
extern void *(*glUnmapBuffer_orig_evas_get)(void);
extern GLboolean (*glUnmapBuffer_thread_cmd)(GLenum target);
extern GLuint (*glCreateShader_thread_cmd)(GLenum type);
extern void (*glShaderSource_thread_cmd)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
extern void (*glCompileShader_thread_cmd)(GLuint shader);
extern void (*glReleaseShaderCompiler_orig_evas_set)(void *func);
extern void *(*glReleaseShaderCompiler_orig_evas_get)(void);
extern void (*glReleaseShaderCompiler_thread_cmd)(void);
extern void (*glDeleteShader_thread_cmd)(GLuint shader);
extern GLuint (*glCreateProgram_thread_cmd)(void);
extern void (*glAttachShader_thread_cmd)(GLuint program, GLuint shader);
extern void (*glDetachShader_thread_cmd)(GLuint program, GLuint shader);
extern void (*glLinkProgram_thread_cmd)(GLuint program);
extern void (*glUseProgram_thread_cmd)(GLuint program);
extern void (*glProgramParameteri_orig_evas_set)(void *func);
extern void *(*glProgramParameteri_orig_evas_get)(void);
extern void (*glProgramParameteri_thread_cmd)(GLuint program, GLenum pname, GLint value);
extern void (*glDeleteProgram_thread_cmd)(GLuint program);
extern void (*glGetProgramBinary_orig_evas_set)(void *func);
extern void *(*glGetProgramBinary_orig_evas_get)(void);
extern void (*glGetProgramBinary_thread_cmd)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
extern void (*glProgramBinary_orig_evas_set)(void *func);
extern void *(*glProgramBinary_orig_evas_get)(void);
extern void (*glProgramBinary_thread_cmd)(GLuint program, GLenum binaryFormat, const void *binary, GLint length);
extern void (*glGetActiveAttrib_thread_cmd)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern GLint (*glGetAttribLocation_thread_cmd)(GLuint program, const GLchar *name);
extern void (*glBindAttribLocation_thread_cmd)(GLuint program, GLuint index, const GLchar *name);
extern GLint (*glGetUniformLocation_thread_cmd)(GLuint program, const GLchar *name);
extern void (*glUniform1f_thread_cmd)(GLint location, GLfloat v0);
extern void (*glUniform1i_thread_cmd)(GLint location, GLint v0);
extern void (*glUniform2f_thread_cmd)(GLint location, GLfloat v0, GLfloat v1);
extern void (*glUniform2i_thread_cmd)(GLint location, GLint v0, GLint v1);
extern void (*glUniform3f_thread_cmd)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void (*glUniform3i_thread_cmd)(GLint location, GLint v0, GLint v1, GLint v2);
extern void (*glUniform4f_thread_cmd)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void (*glUniform4i_thread_cmd)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void (*glUniform1fv_thread_cmd)(GLint location, GLsizei count, const GLfloat *value);
extern void (*glUniform1iv_thread_cmd)(GLint location, GLsizei count, const GLint *value);
extern void (*glUniform2fv_thread_cmd)(GLint location, GLsizei count, const GLfloat *value);
extern void (*glUniform2iv_thread_cmd)(GLint location, GLsizei count, const GLint *value);
extern void (*glUniform3fv_thread_cmd)(GLint location, GLsizei count, const GLfloat *value);
extern void (*glUniform3iv_thread_cmd)(GLint location, GLsizei count, const GLint *value);
extern void (*glUniform4fv_thread_cmd)(GLint location, GLsizei count, const GLfloat *value);
extern void (*glUniform4iv_thread_cmd)(GLint location, GLsizei count, const GLint *value);
extern void (*glUniformMatrix2fv_thread_cmd)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (*glUniformMatrix3fv_thread_cmd)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (*glUniformMatrix4fv_thread_cmd)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (*glViewport_thread_cmd)(GLint x, GLint y, GLsizei width, GLsizei height);
extern void (*glEnable_thread_cmd)(GLenum cap);
extern void (*glDisable_thread_cmd)(GLenum cap);
extern void (*glLineWidth_thread_cmd)(GLfloat width);
extern void (*glPolygonOffset_thread_cmd)(GLfloat factor, GLfloat units);
extern void (*glPixelStorei_thread_cmd)(GLenum pname, GLint param);
extern void (*glActiveTexture_thread_cmd)(GLenum texture);
extern void (*glGenTextures_thread_cmd)(GLsizei n, GLuint *textures);
extern void (*glBindTexture_thread_cmd)(GLenum target, GLuint texture);
extern void (*glDeleteTextures_thread_cmd)(GLsizei n, const GLuint *textures);
extern void (*glTexImage2D_thread_cmd)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
extern void (*glTexSubImage2D_thread_cmd)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
extern void (*glCompressedTexImage2D_thread_cmd)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
extern void (*glCompressedTexSubImage2D_thread_cmd)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
extern void (*glTexParameterf_thread_cmd)(GLenum target, GLenum pname, GLfloat param);
extern void (*glTexParameterfv_thread_cmd)(GLenum target, GLenum pname, const GLfloat *params);
extern void (*glTexParameteri_thread_cmd)(GLenum target, GLenum pname, GLint param);
extern void (*glTexParameteriv_thread_cmd)(GLenum target, GLenum pname, const GLint *params);
extern void (*glScissor_thread_cmd)(GLint x, GLint y, GLsizei width, GLsizei height);
extern void (*glBlendFunc_thread_cmd)(GLenum sfactor, GLenum dfactor);
extern void (*glBlendColor_thread_cmd)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void (*glDepthMask_thread_cmd)(GLboolean flag);
extern void (*glClear_thread_cmd)(GLbitfield mask);
extern void (*glClearColor_thread_cmd)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void (*glReadPixels_thread_cmd)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
extern void (*glGenFramebuffers_orig_evas_set)(void *func);
extern void *(*glGenFramebuffers_orig_evas_get)(void);
extern void (*glGenFramebuffers_thread_cmd)(GLsizei n, GLuint *framebuffers);
extern void (*glBindFramebuffer_orig_evas_set)(void *func);
extern void *(*glBindFramebuffer_orig_evas_get)(void);
extern void (*glBindFramebuffer_thread_cmd)(GLenum target, GLuint framebuffer);
extern void (*glDeleteFramebuffers_orig_evas_set)(void *func);
extern void *(*glDeleteFramebuffers_orig_evas_get)(void);
extern void (*glDeleteFramebuffers_thread_cmd)(GLsizei n, const GLuint *framebuffers);
extern void (*glGenRenderbuffers_thread_cmd)(GLsizei n, GLuint *renderbuffers);
extern void (*glBindRenderbuffer_thread_cmd)(GLenum target, GLuint renderbuffer);
extern void (*glDeleteRenderbuffers_thread_cmd)(GLsizei n, const GLuint *renderbuffers);
extern void (*glRenderbufferStorage_thread_cmd)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void (*glFramebufferRenderbuffer_thread_cmd)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void (*glFramebufferTexture2D_orig_evas_set)(void *func);
extern void *(*glFramebufferTexture2D_orig_evas_get)(void);
extern void (*glFramebufferTexture2D_thread_cmd)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void (*glFramebufferTexture2DMultisample_orig_evas_set)(void *func);
extern void *(*glFramebufferTexture2DMultisample_orig_evas_get)(void);
extern void (*glFramebufferTexture2DMultisample_thread_cmd)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples);
extern GLenum (*glCheckFramebufferStatus_thread_cmd)(GLenum target);
extern void (*glFlush_thread_cmd)(void);
extern void (*glFinish_thread_cmd)(void);
extern void (*glHint_thread_cmd)(GLenum target, GLenum mode);
extern const GLubyte * (*glGetString_thread_cmd)(GLenum name);
extern void (*glGetBooleanv_thread_cmd)(GLenum pname, GLboolean *data);
extern void (*glGetFloatv_thread_cmd)(GLenum pname, GLfloat *data);
extern void (*glGetIntegerv_thread_cmd)(GLenum pname, GLint *data);
extern GLboolean (*glIsBuffer_thread_cmd)(GLint buffer);
extern void (*glGetBufferParameteriv_thread_cmd)(GLenum target, GLenum pname, GLint *params);
extern GLboolean (*glIsShader_thread_cmd)(GLuint shader);
extern void (*glGetShaderiv_thread_cmd)(GLuint shader, GLenum pname, GLint *params);
extern void (*glGetAttachedShaders_thread_cmd)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
extern void (*glGetShaderInfoLog_thread_cmd)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (*glGetShaderSource_thread_cmd)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
extern void (*glGetShaderPrecisionFormat_thread_cmd)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
extern void (*glGetVertexAttribfv_thread_cmd)(GLuint index, GLenum pname, GLfloat *params);
extern void (*glGetVertexAttribiv_thread_cmd)(GLuint index, GLenum pname, GLint *params);
extern GLboolean (*glIsProgram_thread_cmd)(GLuint program);
extern void (*glGetProgramInfoLog_thread_cmd)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (*glGetProgramiv_thread_cmd)(GLuint program, GLenum pname, GLint *params);
extern GLboolean (*glIsFramebuffer_thread_cmd)(GLint framebuffer);
extern void (*glGetFramebufferParameteriv_orig_evas_set)(void *func);
extern void *(*glGetFramebufferParameteriv_orig_evas_get)(void);
extern void (*glGetFramebufferParameteriv_thread_cmd)(GLenum target, GLenum pname, GLint *params);
extern GLboolean (*glIsRenderbuffer_thread_cmd)(GLint renderbuffer);
extern void (*glGetRenderbufferParameteriv_thread_cmd)(GLenum target, GLenum pname, GLint *params);
extern GLboolean (*glIsTexture_thread_cmd)(GLint texture);
extern void (*glStartTiling_orig_evas_set)(void *func);
extern void *(*glStartTiling_orig_evas_get)(void);
extern void (*glStartTiling_thread_cmd)(GLuint a, GLuint b, GLuint c, GLuint d, GLuint e);
extern void (*glEndTiling_orig_evas_set)(void *func);
extern void *(*glEndTiling_orig_evas_get)(void);
extern void (*glEndTiling_thread_cmd)(GLuint a);
extern void (*glActivateTile_orig_evas_set)(void *func);
extern void *(*glActivateTile_orig_evas_get)(void);
extern void (*glActivateTile_thread_cmd)(GLuint a, GLuint b, GLuint c, GLuint d, GLuint e);
extern void (*glEGLImageTargetTexture2DOES_orig_evas_set)(void *func);
extern void *(*glEGLImageTargetTexture2DOES_orig_evas_get)(void);
extern void (*glEGLImageTargetTexture2DOES_thread_cmd)(GLenum target, void *image);

#ifndef GL_GLES
extern void (*glAlphaFunc_thread_cmd)(GLenum func, GLclampf ref);
#endif

#ifndef GL_GLES
extern void (*glGetTexLevelParameteriv_thread_cmd)(GLenum target, GLint level, GLenum pname, GLint *params);
#endif

#ifndef GL_GLES
extern void (*glRenderbufferStorageMultisample_orig_evas_set)(void *func);
extern void *(*glRenderbufferStorageMultisample_orig_evas_get)(void);
extern void (*glRenderbufferStorageMultisample_thread_cmd)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
#endif
extern void (*glGetStringi_orig_evas_set)(void *func);
extern void *(*glGetStringi_orig_evas_get)(void);
extern const GLubyte * (*glGetStringi_thread_cmd)(GLenum name, GLuint index);


extern void _gl_thread_link_gl_generated_init();
