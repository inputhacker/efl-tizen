/*
 * This is an automatically generated file using a python script.
 * ($EFL_HOME/src/utils/evas/generate_gl_thread_api.py)
 * Recommend that you modify data files ($EFL_HOME/src/utils/evas/gl_api_def.txt)
 * and make use of scripts if you need to fix them.
 */
GLenum (*glGetError_thread_cmd)(void) = NULL;
void (*glVertexAttribPointer_thread_cmd)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) = NULL;
void (*glEnableVertexAttribArray_thread_cmd)(GLuint index) = NULL;
void (*glDisableVertexAttribArray_thread_cmd)(GLuint index) = NULL;
void (*glDrawArrays_thread_cmd)(GLenum mode, GLint first, GLsizei count) = NULL;
void (*glDrawElements_thread_cmd)(GLenum mode, GLsizei count, GLenum type, const void *indices) = NULL;
void (*glGenBuffers_thread_cmd)(GLsizei n, GLuint *buffers) = NULL;
void (*glDeleteBuffers_thread_cmd)(GLsizei n, const GLuint *buffers) = NULL;
void (*glBindBuffer_thread_cmd)(GLenum target, GLuint buffer) = NULL;
void (*glBufferData_thread_cmd)(GLenum target, GLsizeiptr size, const void *data, GLenum usage) = NULL;
void (*glMapBuffer_orig_evas_set)(void *func) = NULL;
void *(*glMapBuffer_orig_evas_get)(void) = NULL;
void * (*glMapBuffer_thread_cmd)(GLenum target, GLenum access) = NULL;
void (*glUnmapBuffer_orig_evas_set)(void *func) = NULL;
void *(*glUnmapBuffer_orig_evas_get)(void) = NULL;
GLboolean (*glUnmapBuffer_thread_cmd)(GLenum target) = NULL;
GLuint (*glCreateShader_thread_cmd)(GLenum type) = NULL;
void (*glShaderSource_thread_cmd)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) = NULL;
void (*glCompileShader_thread_cmd)(GLuint shader) = NULL;
void (*glReleaseShaderCompiler_orig_evas_set)(void *func) = NULL;
void *(*glReleaseShaderCompiler_orig_evas_get)(void) = NULL;
void (*glReleaseShaderCompiler_thread_cmd)(void) = NULL;
void (*glDeleteShader_thread_cmd)(GLuint shader) = NULL;
GLuint (*glCreateProgram_thread_cmd)(void) = NULL;
void (*glAttachShader_thread_cmd)(GLuint program, GLuint shader) = NULL;
void (*glDetachShader_thread_cmd)(GLuint program, GLuint shader) = NULL;
void (*glLinkProgram_thread_cmd)(GLuint program) = NULL;
void (*glUseProgram_thread_cmd)(GLuint program) = NULL;
void (*glProgramParameteri_orig_evas_set)(void *func) = NULL;
void *(*glProgramParameteri_orig_evas_get)(void) = NULL;
void (*glProgramParameteri_thread_cmd)(GLuint program, GLenum pname, GLint value) = NULL;
void (*glDeleteProgram_thread_cmd)(GLuint program) = NULL;
void (*glGetProgramBinary_orig_evas_set)(void *func) = NULL;
void *(*glGetProgramBinary_orig_evas_get)(void) = NULL;
void (*glGetProgramBinary_thread_cmd)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary) = NULL;
void (*glProgramBinary_orig_evas_set)(void *func) = NULL;
void *(*glProgramBinary_orig_evas_get)(void) = NULL;
void (*glProgramBinary_thread_cmd)(GLuint program, GLenum binaryFormat, const void *binary, GLint length) = NULL;
void (*glGetActiveAttrib_thread_cmd)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) = NULL;
GLint (*glGetAttribLocation_thread_cmd)(GLuint program, const GLchar *name) = NULL;
void (*glBindAttribLocation_thread_cmd)(GLuint program, GLuint index, const GLchar *name) = NULL;
GLint (*glGetUniformLocation_thread_cmd)(GLuint program, const GLchar *name) = NULL;
void (*glUniform1f_thread_cmd)(GLint location, GLfloat v0) = NULL;
void (*glUniform1i_thread_cmd)(GLint location, GLint v0) = NULL;
void (*glUniform2f_thread_cmd)(GLint location, GLfloat v0, GLfloat v1) = NULL;
void (*glUniform2i_thread_cmd)(GLint location, GLint v0, GLint v1) = NULL;
void (*glUniform3f_thread_cmd)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) = NULL;
void (*glUniform3i_thread_cmd)(GLint location, GLint v0, GLint v1, GLint v2) = NULL;
void (*glUniform4f_thread_cmd)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) = NULL;
void (*glUniform4i_thread_cmd)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) = NULL;
void (*glUniform1fv_thread_cmd)(GLint location, GLsizei count, const GLfloat *value) = NULL;
void (*glUniform1iv_thread_cmd)(GLint location, GLsizei count, const GLint *value) = NULL;
void (*glUniform2fv_thread_cmd)(GLint location, GLsizei count, const GLfloat *value) = NULL;
void (*glUniform2iv_thread_cmd)(GLint location, GLsizei count, const GLint *value) = NULL;
void (*glUniform3fv_thread_cmd)(GLint location, GLsizei count, const GLfloat *value) = NULL;
void (*glUniform3iv_thread_cmd)(GLint location, GLsizei count, const GLint *value) = NULL;
void (*glUniform4fv_thread_cmd)(GLint location, GLsizei count, const GLfloat *value) = NULL;
void (*glUniform4iv_thread_cmd)(GLint location, GLsizei count, const GLint *value) = NULL;
void (*glUniformMatrix2fv_thread_cmd)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = NULL;
void (*glUniformMatrix3fv_thread_cmd)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = NULL;
void (*glUniformMatrix4fv_thread_cmd)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = NULL;
void (*glViewport_thread_cmd)(GLint x, GLint y, GLsizei width, GLsizei height) = NULL;
void (*glEnable_thread_cmd)(GLenum cap) = NULL;
void (*glDisable_thread_cmd)(GLenum cap) = NULL;
void (*glLineWidth_thread_cmd)(GLfloat width) = NULL;
void (*glPolygonOffset_thread_cmd)(GLfloat factor, GLfloat units) = NULL;
void (*glPixelStorei_thread_cmd)(GLenum pname, GLint param) = NULL;
void (*glActiveTexture_thread_cmd)(GLenum texture) = NULL;
void (*glGenTextures_thread_cmd)(GLsizei n, GLuint *textures) = NULL;
void (*glBindTexture_thread_cmd)(GLenum target, GLuint texture) = NULL;
void (*glDeleteTextures_thread_cmd)(GLsizei n, const GLuint *textures) = NULL;
void (*glTexImage2D_thread_cmd)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) = NULL;
void (*glTexSubImage2D_thread_cmd)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) = NULL;
void (*glCompressedTexImage2D_thread_cmd)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) = NULL;
void (*glCompressedTexSubImage2D_thread_cmd)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) = NULL;
void (*glTexParameterf_thread_cmd)(GLenum target, GLenum pname, GLfloat param) = NULL;
void (*glTexParameterfv_thread_cmd)(GLenum target, GLenum pname, const GLfloat *params) = NULL;
void (*glTexParameteri_thread_cmd)(GLenum target, GLenum pname, GLint param) = NULL;
void (*glTexParameteriv_thread_cmd)(GLenum target, GLenum pname, const GLint *params) = NULL;
void (*glScissor_thread_cmd)(GLint x, GLint y, GLsizei width, GLsizei height) = NULL;
void (*glBlendFunc_thread_cmd)(GLenum sfactor, GLenum dfactor) = NULL;
void (*glBlendColor_thread_cmd)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = NULL;
void (*glDepthMask_thread_cmd)(GLboolean flag) = NULL;
void (*glClear_thread_cmd)(GLbitfield mask) = NULL;
void (*glClearColor_thread_cmd)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = NULL;
void (*glReadPixels_thread_cmd)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels) = NULL;
void (*glGenFramebuffers_orig_evas_set)(void *func) = NULL;
void *(*glGenFramebuffers_orig_evas_get)(void) = NULL;
void (*glGenFramebuffers_thread_cmd)(GLsizei n, GLuint *framebuffers) = NULL;
void (*glBindFramebuffer_orig_evas_set)(void *func) = NULL;
void *(*glBindFramebuffer_orig_evas_get)(void) = NULL;
void (*glBindFramebuffer_thread_cmd)(GLenum target, GLuint framebuffer) = NULL;
void (*glDeleteFramebuffers_orig_evas_set)(void *func) = NULL;
void *(*glDeleteFramebuffers_orig_evas_get)(void) = NULL;
void (*glDeleteFramebuffers_thread_cmd)(GLsizei n, const GLuint *framebuffers) = NULL;
void (*glGenRenderbuffers_thread_cmd)(GLsizei n, GLuint *renderbuffers) = NULL;
void (*glBindRenderbuffer_thread_cmd)(GLenum target, GLuint renderbuffer) = NULL;
void (*glDeleteRenderbuffers_thread_cmd)(GLsizei n, const GLuint *renderbuffers) = NULL;
void (*glRenderbufferStorage_thread_cmd)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) = NULL;
void (*glFramebufferRenderbuffer_thread_cmd)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) = NULL;
void (*glFramebufferTexture2D_orig_evas_set)(void *func) = NULL;
void *(*glFramebufferTexture2D_orig_evas_get)(void) = NULL;
void (*glFramebufferTexture2D_thread_cmd)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) = NULL;
void (*glFramebufferTexture2DMultisample_orig_evas_set)(void *func) = NULL;
void *(*glFramebufferTexture2DMultisample_orig_evas_get)(void) = NULL;
void (*glFramebufferTexture2DMultisample_thread_cmd)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples) = NULL;
GLenum (*glCheckFramebufferStatus_thread_cmd)(GLenum target) = NULL;
void (*glFlush_thread_cmd)(void) = NULL;
void (*glFinish_thread_cmd)(void) = NULL;
void (*glHint_thread_cmd)(GLenum target, GLenum mode) = NULL;
const GLubyte * (*glGetString_thread_cmd)(GLenum name) = NULL;
void (*glGetBooleanv_thread_cmd)(GLenum pname, GLboolean *data) = NULL;
void (*glGetFloatv_thread_cmd)(GLenum pname, GLfloat *data) = NULL;
void (*glGetIntegerv_thread_cmd)(GLenum pname, GLint *data) = NULL;
GLboolean (*glIsBuffer_thread_cmd)(GLint buffer) = NULL;
void (*glGetBufferParameteriv_thread_cmd)(GLenum target, GLenum pname, GLint *params) = NULL;
GLboolean (*glIsShader_thread_cmd)(GLuint shader) = NULL;
void (*glGetShaderiv_thread_cmd)(GLuint shader, GLenum pname, GLint *params) = NULL;
void (*glGetAttachedShaders_thread_cmd)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) = NULL;
void (*glGetShaderInfoLog_thread_cmd)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = NULL;
void (*glGetShaderSource_thread_cmd)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) = NULL;
void (*glGetShaderPrecisionFormat_thread_cmd)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) = NULL;
void (*glGetVertexAttribfv_thread_cmd)(GLuint index, GLenum pname, GLfloat *params) = NULL;
void (*glGetVertexAttribiv_thread_cmd)(GLuint index, GLenum pname, GLint *params) = NULL;
GLboolean (*glIsProgram_thread_cmd)(GLuint program) = NULL;
void (*glGetProgramInfoLog_thread_cmd)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = NULL;
void (*glGetProgramiv_thread_cmd)(GLuint program, GLenum pname, GLint *params) = NULL;
GLboolean (*glIsFramebuffer_thread_cmd)(GLint framebuffer) = NULL;
void (*glGetFramebufferParameteriv_orig_evas_set)(void *func) = NULL;
void *(*glGetFramebufferParameteriv_orig_evas_get)(void) = NULL;
void (*glGetFramebufferParameteriv_thread_cmd)(GLenum target, GLenum pname, GLint *params) = NULL;
GLboolean (*glIsRenderbuffer_thread_cmd)(GLint renderbuffer) = NULL;
void (*glGetRenderbufferParameteriv_thread_cmd)(GLenum target, GLenum pname, GLint *params) = NULL;
GLboolean (*glIsTexture_thread_cmd)(GLint texture) = NULL;
void (*glStartTiling_orig_evas_set)(void *func) = NULL;
void *(*glStartTiling_orig_evas_get)(void) = NULL;
void (*glStartTiling_thread_cmd)(GLuint a, GLuint b, GLuint c, GLuint d, GLuint e) = NULL;
void (*glEndTiling_orig_evas_set)(void *func) = NULL;
void *(*glEndTiling_orig_evas_get)(void) = NULL;
void (*glEndTiling_thread_cmd)(GLuint a) = NULL;
void (*glActivateTile_orig_evas_set)(void *func) = NULL;
void *(*glActivateTile_orig_evas_get)(void) = NULL;
void (*glActivateTile_thread_cmd)(GLuint a, GLuint b, GLuint c, GLuint d, GLuint e) = NULL;
void (*glEGLImageTargetTexture2DOES_orig_evas_set)(void *func) = NULL;
void *(*glEGLImageTargetTexture2DOES_orig_evas_get)(void) = NULL;
void (*glEGLImageTargetTexture2DOES_thread_cmd)(GLenum target, void *image) = NULL;

#ifndef GL_GLES
void (*glAlphaFunc_thread_cmd)(GLenum func, GLclampf ref) = NULL;
#endif

#ifndef GL_GLES
void (*glGetTexLevelParameteriv_thread_cmd)(GLenum target, GLint level, GLenum pname, GLint *params) = NULL;
#endif

#ifndef GL_GLES
void (*glRenderbufferStorageMultisample_orig_evas_set)(void *func) = NULL;
void *(*glRenderbufferStorageMultisample_orig_evas_get)(void) = NULL;
void (*glRenderbufferStorageMultisample_thread_cmd)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) = NULL;
#endif
void (*glGetStringi_orig_evas_set)(void *func) = NULL;
void *(*glGetStringi_orig_evas_get)(void) = NULL;
const GLubyte * (*glGetStringi_thread_cmd)(GLenum name, GLuint index) = NULL;


void
_gl_thread_link_gl_generated_init()
{
#define LINK2GENERIC(sym) \
   sym = dlsym(RTLD_DEFAULT, #sym); \
   if (!sym) ERR("Could not find function '%s'", #sym);

   LINK2GENERIC(glGetError_thread_cmd);
   LINK2GENERIC(glVertexAttribPointer_thread_cmd);
   LINK2GENERIC(glEnableVertexAttribArray_thread_cmd);
   LINK2GENERIC(glDisableVertexAttribArray_thread_cmd);
   LINK2GENERIC(glDrawArrays_thread_cmd);
   LINK2GENERIC(glDrawElements_thread_cmd);
   LINK2GENERIC(glGenBuffers_thread_cmd);
   LINK2GENERIC(glDeleteBuffers_thread_cmd);
   LINK2GENERIC(glBindBuffer_thread_cmd);
   LINK2GENERIC(glBufferData_thread_cmd);
   LINK2GENERIC(glMapBuffer_orig_evas_set);
   LINK2GENERIC(glMapBuffer_orig_evas_get);
   LINK2GENERIC(glMapBuffer_thread_cmd);
   LINK2GENERIC(glUnmapBuffer_orig_evas_set);
   LINK2GENERIC(glUnmapBuffer_orig_evas_get);
   LINK2GENERIC(glUnmapBuffer_thread_cmd);
   LINK2GENERIC(glCreateShader_thread_cmd);
   LINK2GENERIC(glShaderSource_thread_cmd);
   LINK2GENERIC(glCompileShader_thread_cmd);
   LINK2GENERIC(glReleaseShaderCompiler_orig_evas_set);
   LINK2GENERIC(glReleaseShaderCompiler_orig_evas_get);
   LINK2GENERIC(glReleaseShaderCompiler_thread_cmd);
   LINK2GENERIC(glDeleteShader_thread_cmd);
   LINK2GENERIC(glCreateProgram_thread_cmd);
   LINK2GENERIC(glAttachShader_thread_cmd);
   LINK2GENERIC(glDetachShader_thread_cmd);
   LINK2GENERIC(glLinkProgram_thread_cmd);
   LINK2GENERIC(glUseProgram_thread_cmd);
   LINK2GENERIC(glProgramParameteri_orig_evas_set);
   LINK2GENERIC(glProgramParameteri_orig_evas_get);
   LINK2GENERIC(glProgramParameteri_thread_cmd);
   LINK2GENERIC(glDeleteProgram_thread_cmd);
   LINK2GENERIC(glGetProgramBinary_orig_evas_set);
   LINK2GENERIC(glGetProgramBinary_orig_evas_get);
   LINK2GENERIC(glGetProgramBinary_thread_cmd);
   LINK2GENERIC(glProgramBinary_orig_evas_set);
   LINK2GENERIC(glProgramBinary_orig_evas_get);
   LINK2GENERIC(glProgramBinary_thread_cmd);
   LINK2GENERIC(glGetActiveAttrib_thread_cmd);
   LINK2GENERIC(glGetAttribLocation_thread_cmd);
   LINK2GENERIC(glBindAttribLocation_thread_cmd);
   LINK2GENERIC(glGetUniformLocation_thread_cmd);
   LINK2GENERIC(glUniform1f_thread_cmd);
   LINK2GENERIC(glUniform1i_thread_cmd);
   LINK2GENERIC(glUniform2f_thread_cmd);
   LINK2GENERIC(glUniform2i_thread_cmd);
   LINK2GENERIC(glUniform3f_thread_cmd);
   LINK2GENERIC(glUniform3i_thread_cmd);
   LINK2GENERIC(glUniform4f_thread_cmd);
   LINK2GENERIC(glUniform4i_thread_cmd);
   LINK2GENERIC(glUniform1fv_thread_cmd);
   LINK2GENERIC(glUniform1iv_thread_cmd);
   LINK2GENERIC(glUniform2fv_thread_cmd);
   LINK2GENERIC(glUniform2iv_thread_cmd);
   LINK2GENERIC(glUniform3fv_thread_cmd);
   LINK2GENERIC(glUniform3iv_thread_cmd);
   LINK2GENERIC(glUniform4fv_thread_cmd);
   LINK2GENERIC(glUniform4iv_thread_cmd);
   LINK2GENERIC(glUniformMatrix2fv_thread_cmd);
   LINK2GENERIC(glUniformMatrix3fv_thread_cmd);
   LINK2GENERIC(glUniformMatrix4fv_thread_cmd);
   LINK2GENERIC(glViewport_thread_cmd);
   LINK2GENERIC(glEnable_thread_cmd);
   LINK2GENERIC(glDisable_thread_cmd);
   LINK2GENERIC(glLineWidth_thread_cmd);
   LINK2GENERIC(glPolygonOffset_thread_cmd);
   LINK2GENERIC(glPixelStorei_thread_cmd);
   LINK2GENERIC(glActiveTexture_thread_cmd);
   LINK2GENERIC(glGenTextures_thread_cmd);
   LINK2GENERIC(glBindTexture_thread_cmd);
   LINK2GENERIC(glDeleteTextures_thread_cmd);
   LINK2GENERIC(glTexImage2D_thread_cmd);
   LINK2GENERIC(glTexSubImage2D_thread_cmd);
   LINK2GENERIC(glCompressedTexImage2D_thread_cmd);
   LINK2GENERIC(glCompressedTexSubImage2D_thread_cmd);
   LINK2GENERIC(glTexParameterf_thread_cmd);
   LINK2GENERIC(glTexParameterfv_thread_cmd);
   LINK2GENERIC(glTexParameteri_thread_cmd);
   LINK2GENERIC(glTexParameteriv_thread_cmd);
   LINK2GENERIC(glScissor_thread_cmd);
   LINK2GENERIC(glBlendFunc_thread_cmd);
   LINK2GENERIC(glBlendColor_thread_cmd);
   LINK2GENERIC(glDepthMask_thread_cmd);
   LINK2GENERIC(glClear_thread_cmd);
   LINK2GENERIC(glClearColor_thread_cmd);
   LINK2GENERIC(glReadPixels_thread_cmd);
   LINK2GENERIC(glGenFramebuffers_orig_evas_set);
   LINK2GENERIC(glGenFramebuffers_orig_evas_get);
   LINK2GENERIC(glGenFramebuffers_thread_cmd);
   LINK2GENERIC(glBindFramebuffer_orig_evas_set);
   LINK2GENERIC(glBindFramebuffer_orig_evas_get);
   LINK2GENERIC(glBindFramebuffer_thread_cmd);
   LINK2GENERIC(glDeleteFramebuffers_orig_evas_set);
   LINK2GENERIC(glDeleteFramebuffers_orig_evas_get);
   LINK2GENERIC(glDeleteFramebuffers_thread_cmd);
   LINK2GENERIC(glGenRenderbuffers_thread_cmd);
   LINK2GENERIC(glBindRenderbuffer_thread_cmd);
   LINK2GENERIC(glDeleteRenderbuffers_thread_cmd);
   LINK2GENERIC(glRenderbufferStorage_thread_cmd);
   LINK2GENERIC(glFramebufferRenderbuffer_thread_cmd);
   LINK2GENERIC(glFramebufferTexture2D_orig_evas_set);
   LINK2GENERIC(glFramebufferTexture2D_orig_evas_get);
   LINK2GENERIC(glFramebufferTexture2D_thread_cmd);
   LINK2GENERIC(glFramebufferTexture2DMultisample_orig_evas_set);
   LINK2GENERIC(glFramebufferTexture2DMultisample_orig_evas_get);
   LINK2GENERIC(glFramebufferTexture2DMultisample_thread_cmd);
   LINK2GENERIC(glCheckFramebufferStatus_thread_cmd);
   LINK2GENERIC(glFlush_thread_cmd);
   LINK2GENERIC(glFinish_thread_cmd);
   LINK2GENERIC(glHint_thread_cmd);
   LINK2GENERIC(glGetString_thread_cmd);
   LINK2GENERIC(glGetBooleanv_thread_cmd);
   LINK2GENERIC(glGetFloatv_thread_cmd);
   LINK2GENERIC(glGetIntegerv_thread_cmd);
   LINK2GENERIC(glIsBuffer_thread_cmd);
   LINK2GENERIC(glGetBufferParameteriv_thread_cmd);
   LINK2GENERIC(glIsShader_thread_cmd);
   LINK2GENERIC(glGetShaderiv_thread_cmd);
   LINK2GENERIC(glGetAttachedShaders_thread_cmd);
   LINK2GENERIC(glGetShaderInfoLog_thread_cmd);
   LINK2GENERIC(glGetShaderSource_thread_cmd);
   LINK2GENERIC(glGetShaderPrecisionFormat_thread_cmd);
   LINK2GENERIC(glGetVertexAttribfv_thread_cmd);
   LINK2GENERIC(glGetVertexAttribiv_thread_cmd);
   LINK2GENERIC(glIsProgram_thread_cmd);
   LINK2GENERIC(glGetProgramInfoLog_thread_cmd);
   LINK2GENERIC(glGetProgramiv_thread_cmd);
   LINK2GENERIC(glIsFramebuffer_thread_cmd);
   LINK2GENERIC(glGetFramebufferParameteriv_orig_evas_set);
   LINK2GENERIC(glGetFramebufferParameteriv_orig_evas_get);
   LINK2GENERIC(glGetFramebufferParameteriv_thread_cmd);
   LINK2GENERIC(glIsRenderbuffer_thread_cmd);
   LINK2GENERIC(glGetRenderbufferParameteriv_thread_cmd);
   LINK2GENERIC(glIsTexture_thread_cmd);
   LINK2GENERIC(glStartTiling_orig_evas_set);
   LINK2GENERIC(glStartTiling_orig_evas_get);
   LINK2GENERIC(glStartTiling_thread_cmd);
   LINK2GENERIC(glEndTiling_orig_evas_set);
   LINK2GENERIC(glEndTiling_orig_evas_get);
   LINK2GENERIC(glEndTiling_thread_cmd);
   LINK2GENERIC(glActivateTile_orig_evas_set);
   LINK2GENERIC(glActivateTile_orig_evas_get);
   LINK2GENERIC(glActivateTile_thread_cmd);
   LINK2GENERIC(glEGLImageTargetTexture2DOES_orig_evas_set);
   LINK2GENERIC(glEGLImageTargetTexture2DOES_orig_evas_get);
   LINK2GENERIC(glEGLImageTargetTexture2DOES_thread_cmd);

#ifndef GL_GLES
   LINK2GENERIC(glAlphaFunc_thread_cmd);
#endif


#ifndef GL_GLES
   LINK2GENERIC(glGetTexLevelParameteriv_thread_cmd);
#endif


#ifndef GL_GLES
   LINK2GENERIC(glRenderbufferStorageMultisample_orig_evas_set);
   LINK2GENERIC(glRenderbufferStorageMultisample_orig_evas_get);
   LINK2GENERIC(glRenderbufferStorageMultisample_thread_cmd);
#endif

   LINK2GENERIC(glGetStringi_orig_evas_set);
   LINK2GENERIC(glGetStringi_orig_evas_get);
   LINK2GENERIC(glGetStringi_thread_cmd);
}
