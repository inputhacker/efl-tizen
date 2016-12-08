/* DO NOT MODIFY THIS FILE AS IT IS AUTO-GENERATED */


EAPI GLenum glGetError_thread_cmd(void);
EAPI void glVertexAttribPointer_thread_cmd(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
EAPI void glEnableVertexAttribArray_thread_cmd(GLuint index);
EAPI void glDisableVertexAttribArray_thread_cmd(GLuint index);
EAPI void glDrawArrays_thread_cmd(GLenum mode, GLint first, GLsizei count);
EAPI void glDrawElements_thread_cmd(GLenum mode, GLsizei count, GLenum type, const void *indices);
EAPI void glGenBuffers_thread_cmd(GLsizei n, GLuint *buffers);
EAPI void glDeleteBuffers_thread_cmd(GLsizei n, const GLuint *buffers);
EAPI void glBindBuffer_thread_cmd(GLenum target, GLuint buffer);
EAPI void glBufferData_thread_cmd(GLenum target, GLsizeiptr size, const void *data, GLenum usage);

EAPI void glMapBuffer_orig_evas_set(void *func);
EAPI void *glMapBuffer_orig_evas_get(void);
EAPI void * glMapBuffer_thread_cmd(GLenum target, GLenum access);

EAPI void glUnmapBuffer_orig_evas_set(void *func);
EAPI void *glUnmapBuffer_orig_evas_get(void);
EAPI GLboolean glUnmapBuffer_thread_cmd(GLenum target);
EAPI GLuint glCreateShader_thread_cmd(GLenum type);
EAPI void glShaderSource_thread_cmd(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
EAPI void glCompileShader_thread_cmd(GLuint shader);

EAPI void glReleaseShaderCompiler_orig_evas_set(void *func);
EAPI void *glReleaseShaderCompiler_orig_evas_get(void);
EAPI void glReleaseShaderCompiler_thread_cmd(void);
EAPI void glDeleteShader_thread_cmd(GLuint shader);
EAPI GLuint glCreateProgram_thread_cmd(void);
EAPI void glAttachShader_thread_cmd(GLuint program, GLuint shader);
EAPI void glDetachShader_thread_cmd(GLuint program, GLuint shader);
EAPI void glLinkProgram_thread_cmd(GLuint program);
EAPI void glUseProgram_thread_cmd(GLuint program);

EAPI void glProgramParameteri_orig_evas_set(void *func);
EAPI void *glProgramParameteri_orig_evas_get(void);
EAPI void glProgramParameteri_thread_cmd(GLuint program, GLenum pname, GLint value);
EAPI void glDeleteProgram_thread_cmd(GLuint program);

EAPI void glGetProgramBinary_orig_evas_set(void *func);
EAPI void *glGetProgramBinary_orig_evas_get(void);
EAPI void glGetProgramBinary_thread_cmd(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);

EAPI void glProgramBinary_orig_evas_set(void *func);
EAPI void *glProgramBinary_orig_evas_get(void);
EAPI void glProgramBinary_thread_cmd(GLuint program, GLenum binaryFormat, const void *binary, GLint length);
EAPI void glGetActiveAttrib_thread_cmd(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
EAPI GLint glGetAttribLocation_thread_cmd(GLuint program, const GLchar *name);
EAPI void glBindAttribLocation_thread_cmd(GLuint program, GLuint index, const GLchar *name);
EAPI GLint glGetUniformLocation_thread_cmd(GLuint program, const GLchar *name);
EAPI void glUniform1f_thread_cmd(GLint location, GLfloat v0);
EAPI void glUniform1i_thread_cmd(GLint location, GLint v0);
EAPI void glUniform2f_thread_cmd(GLint location, GLfloat v0, GLfloat v1);
EAPI void glUniform2i_thread_cmd(GLint location, GLint v0, GLint v1);
EAPI void glUniform3f_thread_cmd(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
EAPI void glUniform3i_thread_cmd(GLint location, GLint v0, GLint v1, GLint v2);
EAPI void glUniform4f_thread_cmd(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
EAPI void glUniform4i_thread_cmd(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
EAPI void glUniform1fv_thread_cmd(GLint location, GLsizei count, const GLfloat *value);
EAPI void glUniform1iv_thread_cmd(GLint location, GLsizei count, const GLint *value);
EAPI void glUniform2fv_thread_cmd(GLint location, GLsizei count, const GLfloat *value);
EAPI void glUniform2iv_thread_cmd(GLint location, GLsizei count, const GLint *value);
EAPI void glUniform3fv_thread_cmd(GLint location, GLsizei count, const GLfloat *value);
EAPI void glUniform3iv_thread_cmd(GLint location, GLsizei count, const GLint *value);
EAPI void glUniform4fv_thread_cmd(GLint location, GLsizei count, const GLfloat *value);
EAPI void glUniform4iv_thread_cmd(GLint location, GLsizei count, const GLint *value);
EAPI void glUniformMatrix2fv_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glUniformMatrix3fv_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glUniformMatrix4fv_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glViewport_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height);
EAPI void glEnable_thread_cmd(GLenum cap);
EAPI void glDisable_thread_cmd(GLenum cap);
EAPI void glLineWidth_thread_cmd(GLfloat width);
EAPI void glPolygonOffset_thread_cmd(GLfloat factor, GLfloat units);
EAPI void glPixelStorei_thread_cmd(GLenum pname, GLint param);
EAPI void glActiveTexture_thread_cmd(GLenum texture);
EAPI void glGenTextures_thread_cmd(GLsizei n, GLuint *textures);
EAPI void glBindTexture_thread_cmd(GLenum target, GLuint texture);
EAPI void glDeleteTextures_thread_cmd(GLsizei n, const GLuint *textures);
EAPI void glTexImage2D_thread_cmd(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
EAPI void glTexSubImage2D_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
EAPI void glCompressedTexImage2D_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
EAPI void glCompressedTexSubImage2D_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
EAPI void glTexParameterf_thread_cmd(GLenum target, GLenum pname, GLfloat param);
EAPI void glTexParameterfv_thread_cmd(GLenum target, GLenum pname, const GLfloat *params);
EAPI void glTexParameteri_thread_cmd(GLenum target, GLenum pname, GLint param);
EAPI void glTexParameteriv_thread_cmd(GLenum target, GLenum pname, const GLint *params);
EAPI void glScissor_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height);
EAPI void glBlendFunc_thread_cmd(GLenum sfactor, GLenum dfactor);
EAPI void glBlendColor_thread_cmd(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
EAPI void glDepthMask_thread_cmd(GLboolean flag);
EAPI void glClear_thread_cmd(GLbitfield mask);
EAPI void glClearColor_thread_cmd(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
EAPI void glReadPixels_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);

EAPI void glGenFramebuffers_orig_evas_set(void *func);
EAPI void *glGenFramebuffers_orig_evas_get(void);
EAPI void glGenFramebuffers_thread_cmd(GLsizei n, GLuint *framebuffers);

EAPI void glBindFramebuffer_orig_evas_set(void *func);
EAPI void *glBindFramebuffer_orig_evas_get(void);
EAPI void glBindFramebuffer_thread_cmd(GLenum target, GLuint framebuffer);

EAPI void glDeleteFramebuffers_orig_evas_set(void *func);
EAPI void *glDeleteFramebuffers_orig_evas_get(void);
EAPI void glDeleteFramebuffers_thread_cmd(GLsizei n, const GLuint *framebuffers);
EAPI void glGenRenderbuffers_thread_cmd(GLsizei n, GLuint *renderbuffers);
EAPI void glBindRenderbuffer_thread_cmd(GLenum target, GLuint renderbuffer);
EAPI void glDeleteRenderbuffers_thread_cmd(GLsizei n, const GLuint *renderbuffers);
EAPI void glRenderbufferStorage_thread_cmd(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
EAPI void glFramebufferRenderbuffer_thread_cmd(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

EAPI void glFramebufferTexture2D_orig_evas_set(void *func);
EAPI void *glFramebufferTexture2D_orig_evas_get(void);
EAPI void glFramebufferTexture2D_thread_cmd(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

EAPI void glFramebufferTexture2DMultisample_orig_evas_set(void *func);
EAPI void *glFramebufferTexture2DMultisample_orig_evas_get(void);
EAPI void glFramebufferTexture2DMultisample_thread_cmd(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples);
EAPI GLenum glCheckFramebufferStatus_thread_cmd(GLenum target);
EAPI void glFlush_thread_cmd(void);
EAPI void glFinish_thread_cmd(void);
EAPI void glHint_thread_cmd(GLenum target, GLenum mode);
EAPI const GLubyte * glGetString_thread_cmd(GLenum name);
EAPI void glGetBooleanv_thread_cmd(GLenum pname, GLboolean *data);
EAPI void glGetFloatv_thread_cmd(GLenum pname, GLfloat *data);
EAPI void glGetIntegerv_thread_cmd(GLenum pname, GLint *data);
EAPI GLboolean glIsBuffer_thread_cmd(GLint buffer);
EAPI void glGetBufferParameteriv_thread_cmd(GLenum target, GLenum pname, GLint *params);
EAPI GLboolean glIsShader_thread_cmd(GLuint shader);
EAPI void glGetShaderiv_thread_cmd(GLuint shader, GLenum pname, GLint *params);
EAPI void glGetAttachedShaders_thread_cmd(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
EAPI void glGetShaderInfoLog_thread_cmd(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
EAPI void glGetShaderSource_thread_cmd(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
EAPI void glGetShaderPrecisionFormat_thread_cmd(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
EAPI void glGetVertexAttribfv_thread_cmd(GLuint index, GLenum pname, GLfloat *params);
EAPI void glGetVertexAttribiv_thread_cmd(GLuint index, GLenum pname, GLint *params);
EAPI GLboolean glIsProgram_thread_cmd(GLuint program);
EAPI void glGetProgramInfoLog_thread_cmd(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
EAPI void glGetProgramiv_thread_cmd(GLuint program, GLenum pname, GLint *params);
EAPI GLboolean glIsFramebuffer_thread_cmd(GLint framebuffer);

EAPI void glGetFramebufferParameteriv_orig_evas_set(void *func);
EAPI void *glGetFramebufferParameteriv_orig_evas_get(void);
EAPI void glGetFramebufferParameteriv_thread_cmd(GLenum target, GLenum pname, GLint *params);
EAPI GLboolean glIsRenderbuffer_thread_cmd(GLint renderbuffer);
EAPI void glGetRenderbufferParameteriv_thread_cmd(GLenum target, GLenum pname, GLint *params);
EAPI GLboolean glIsTexture_thread_cmd(GLint texture);

EAPI void glStartTiling_orig_evas_set(void *func);
EAPI void *glStartTiling_orig_evas_get(void);
EAPI void glStartTiling_thread_cmd(GLuint a, GLuint b, GLuint c, GLuint d, GLuint e);

EAPI void glEndTiling_orig_evas_set(void *func);
EAPI void *glEndTiling_orig_evas_get(void);
EAPI void glEndTiling_thread_cmd(GLuint a);

EAPI void glActivateTile_orig_evas_set(void *func);
EAPI void *glActivateTile_orig_evas_get(void);
EAPI void glActivateTile_thread_cmd(GLuint a, GLuint b, GLuint c, GLuint d, GLuint e);

EAPI void glEGLImageTargetTexture2DOES_orig_evas_set(void *func);
EAPI void *glEGLImageTargetTexture2DOES_orig_evas_get(void);
EAPI void glEGLImageTargetTexture2DOES_thread_cmd(GLenum target, void *image);
#ifndef GL_GLES

EAPI void glAlphaFunc_thread_cmd(GLenum func, GLclampf ref);
#endif

#ifndef GL_GLES

EAPI void glGetTexLevelParameteriv_thread_cmd(GLenum target, GLint level, GLenum pname, GLint *params);
#endif

#ifndef GL_GLES


EAPI void glRenderbufferStorageMultisample_orig_evas_set(void *func);
EAPI void *glRenderbufferStorageMultisample_orig_evas_get(void);
EAPI void glRenderbufferStorageMultisample_thread_cmd(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
#endif


EAPI void glGetStringi_orig_evas_set(void *func);
EAPI void *glGetStringi_orig_evas_get(void);
EAPI const GLubyte * glGetStringi_thread_cmd(GLenum name, GLuint index);
