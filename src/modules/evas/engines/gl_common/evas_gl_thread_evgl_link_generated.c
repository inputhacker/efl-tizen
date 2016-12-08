/* DO NOT MODIFY THIS FILE AS IT IS AUTO-GENERATED */

GLenum (*glGetError_evgl_thread_cmd)(void) = NULL;
void (*glVertexAttribPointer_evgl_thread_cmd)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) = NULL;
void (*glEnableVertexAttribArray_evgl_thread_cmd)(GLuint index) = NULL;
void (*glDisableVertexAttribArray_evgl_thread_cmd)(GLuint index) = NULL;
void (*glDrawArrays_evgl_thread_cmd)(GLenum mode, GLint first, GLsizei count) = NULL;
void (*glDrawElements_evgl_thread_cmd)(GLenum mode, GLsizei count, GLenum type, const void *indices) = NULL;
void (*glGenBuffers_evgl_thread_cmd)(GLsizei n, GLuint *buffers) = NULL;
void (*glDeleteBuffers_evgl_thread_cmd)(GLsizei n, const GLuint *buffers) = NULL;
void (*glBindBuffer_evgl_thread_cmd)(GLenum target, GLuint buffer) = NULL;
void (*glBufferData_evgl_thread_cmd)(GLenum target, GLsizeiptr size, const void *data, GLenum usage) = NULL;
GLuint (*glCreateShader_evgl_thread_cmd)(GLenum type) = NULL;
void (*glShaderSource_evgl_thread_cmd)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) = NULL;
void (*glCompileShader_evgl_thread_cmd)(GLuint shader) = NULL;
void (*glDeleteShader_evgl_thread_cmd)(GLuint shader) = NULL;
GLuint (*glCreateProgram_evgl_thread_cmd)(void) = NULL;
void (*glAttachShader_evgl_thread_cmd)(GLuint program, GLuint shader) = NULL;
void (*glDetachShader_evgl_thread_cmd)(GLuint program, GLuint shader) = NULL;
void (*glLinkProgram_evgl_thread_cmd)(GLuint program) = NULL;
void (*glUseProgram_evgl_thread_cmd)(GLuint program) = NULL;
void (*glDeleteProgram_evgl_thread_cmd)(GLuint program) = NULL;
void (*glGetProgramBinary_orig_evgl_set)(void *func) = NULL;
void *(*glGetProgramBinary_orig_evgl_get)(void) = NULL;
void (*glGetProgramBinary_evgl_thread_cmd)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary) = NULL;
void (*glProgramBinary_orig_evgl_set)(void *func) = NULL;
void *(*glProgramBinary_orig_evgl_get)(void) = NULL;
void (*glProgramBinary_evgl_thread_cmd)(GLuint program, GLenum binaryFormat, const void *binary, GLint length) = NULL;
void (*glGetActiveAttrib_evgl_thread_cmd)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) = NULL;
GLint (*glGetAttribLocation_evgl_thread_cmd)(GLuint program, const GLchar *name) = NULL;
void (*glBindAttribLocation_evgl_thread_cmd)(GLuint program, GLuint index, const GLchar *name) = NULL;
GLint (*glGetUniformLocation_evgl_thread_cmd)(GLuint program, const GLchar *name) = NULL;
void (*glUniform1f_evgl_thread_cmd)(GLint location, GLfloat v0) = NULL;
void (*glUniform1i_evgl_thread_cmd)(GLint location, GLint v0) = NULL;
void (*glUniform2f_evgl_thread_cmd)(GLint location, GLfloat v0, GLfloat v1) = NULL;
void (*glUniform2i_evgl_thread_cmd)(GLint location, GLint v0, GLint v1) = NULL;
void (*glUniform3f_evgl_thread_cmd)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) = NULL;
void (*glUniform3i_evgl_thread_cmd)(GLint location, GLint v0, GLint v1, GLint v2) = NULL;
void (*glUniform4f_evgl_thread_cmd)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) = NULL;
void (*glUniform4i_evgl_thread_cmd)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) = NULL;
void (*glUniform1fv_evgl_thread_cmd)(GLint location, GLsizei count, const GLfloat *value) = NULL;
void (*glUniform1iv_evgl_thread_cmd)(GLint location, GLsizei count, const GLint *value) = NULL;
void (*glUniform2fv_evgl_thread_cmd)(GLint location, GLsizei count, const GLfloat *value) = NULL;
void (*glUniform2iv_evgl_thread_cmd)(GLint location, GLsizei count, const GLint *value) = NULL;
void (*glUniform3fv_evgl_thread_cmd)(GLint location, GLsizei count, const GLfloat *value) = NULL;
void (*glUniform3iv_evgl_thread_cmd)(GLint location, GLsizei count, const GLint *value) = NULL;
void (*glUniform4fv_evgl_thread_cmd)(GLint location, GLsizei count, const GLfloat *value) = NULL;
void (*glUniform4iv_evgl_thread_cmd)(GLint location, GLsizei count, const GLint *value) = NULL;
void (*glUniformMatrix2fv_evgl_thread_cmd)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = NULL;
void (*glUniformMatrix3fv_evgl_thread_cmd)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = NULL;
void (*glUniformMatrix4fv_evgl_thread_cmd)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = NULL;
void (*glViewport_evgl_thread_cmd)(GLint x, GLint y, GLsizei width, GLsizei height) = NULL;
void (*glEnable_evgl_thread_cmd)(GLenum cap) = NULL;
void (*glDisable_evgl_thread_cmd)(GLenum cap) = NULL;
void (*glLineWidth_evgl_thread_cmd)(GLfloat width) = NULL;
void (*glPolygonOffset_evgl_thread_cmd)(GLfloat factor, GLfloat units) = NULL;
void (*glPixelStorei_evgl_thread_cmd)(GLenum pname, GLint param) = NULL;
void (*glActiveTexture_evgl_thread_cmd)(GLenum texture) = NULL;
void (*glGenTextures_evgl_thread_cmd)(GLsizei n, GLuint *textures) = NULL;
void (*glBindTexture_evgl_thread_cmd)(GLenum target, GLuint texture) = NULL;
void (*glDeleteTextures_evgl_thread_cmd)(GLsizei n, const GLuint *textures) = NULL;
void (*glTexImage2D_evgl_thread_cmd)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) = NULL;
void (*glTexSubImage2D_evgl_thread_cmd)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) = NULL;
void (*glCompressedTexImage2D_evgl_thread_cmd)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) = NULL;
void (*glCompressedTexSubImage2D_evgl_thread_cmd)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) = NULL;
void (*glTexParameterf_evgl_thread_cmd)(GLenum target, GLenum pname, GLfloat param) = NULL;
void (*glTexParameterfv_evgl_thread_cmd)(GLenum target, GLenum pname, const GLfloat *params) = NULL;
void (*glTexParameteri_evgl_thread_cmd)(GLenum target, GLenum pname, GLint param) = NULL;
void (*glTexParameteriv_evgl_thread_cmd)(GLenum target, GLenum pname, const GLint *params) = NULL;
void (*glScissor_evgl_thread_cmd)(GLint x, GLint y, GLsizei width, GLsizei height) = NULL;
void (*glBlendFunc_evgl_thread_cmd)(GLenum sfactor, GLenum dfactor) = NULL;
void (*glBlendColor_evgl_thread_cmd)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = NULL;
void (*glDepthMask_evgl_thread_cmd)(GLboolean flag) = NULL;
void (*glClear_evgl_thread_cmd)(GLbitfield mask) = NULL;
void (*glClearColor_evgl_thread_cmd)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = NULL;
void (*glReadPixels_evgl_thread_cmd)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels) = NULL;
void (*glGenFramebuffers_evgl_thread_cmd)(GLsizei n, GLuint *framebuffers) = NULL;
void (*glBindFramebuffer_evgl_thread_cmd)(GLenum target, GLuint framebuffer) = NULL;
void (*glDeleteFramebuffers_evgl_thread_cmd)(GLsizei n, const GLuint *framebuffers) = NULL;
void (*glGenRenderbuffers_evgl_thread_cmd)(GLsizei n, GLuint *renderbuffers) = NULL;
void (*glBindRenderbuffer_evgl_thread_cmd)(GLenum target, GLuint renderbuffer) = NULL;
void (*glDeleteRenderbuffers_evgl_thread_cmd)(GLsizei n, const GLuint *renderbuffers) = NULL;
void (*glRenderbufferStorage_evgl_thread_cmd)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) = NULL;
void (*glFramebufferRenderbuffer_evgl_thread_cmd)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) = NULL;
void (*glFramebufferTexture2D_evgl_thread_cmd)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) = NULL;
GLenum (*glCheckFramebufferStatus_evgl_thread_cmd)(GLenum target) = NULL;
void (*glFlush_evgl_thread_cmd)(void) = NULL;
void (*glFinish_evgl_thread_cmd)(void) = NULL;
void (*glHint_evgl_thread_cmd)(GLenum target, GLenum mode) = NULL;
const GLubyte * (*glGetString_evgl_thread_cmd)(GLenum name) = NULL;
void (*glGetBooleanv_evgl_thread_cmd)(GLenum pname, GLboolean *data) = NULL;
void (*glGetFloatv_evgl_thread_cmd)(GLenum pname, GLfloat *data) = NULL;
void (*glGetIntegerv_evgl_thread_cmd)(GLenum pname, GLint *data) = NULL;
GLboolean (*glIsBuffer_evgl_thread_cmd)(GLint buffer) = NULL;
void (*glGetBufferParameteriv_evgl_thread_cmd)(GLenum target, GLenum pname, GLint *params) = NULL;
GLboolean (*glIsShader_evgl_thread_cmd)(GLuint shader) = NULL;
void (*glGetShaderiv_evgl_thread_cmd)(GLuint shader, GLenum pname, GLint *params) = NULL;
void (*glGetAttachedShaders_evgl_thread_cmd)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) = NULL;
void (*glGetShaderInfoLog_evgl_thread_cmd)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = NULL;
void (*glGetShaderSource_evgl_thread_cmd)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) = NULL;
void (*glGetShaderPrecisionFormat_evgl_thread_cmd)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) = NULL;
void (*glGetVertexAttribfv_evgl_thread_cmd)(GLuint index, GLenum pname, GLfloat *params) = NULL;
void (*glGetVertexAttribiv_evgl_thread_cmd)(GLuint index, GLenum pname, GLint *params) = NULL;
GLboolean (*glIsProgram_evgl_thread_cmd)(GLuint program) = NULL;
void (*glGetProgramInfoLog_evgl_thread_cmd)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = NULL;
void (*glGetProgramiv_evgl_thread_cmd)(GLuint program, GLenum pname, GLint *params) = NULL;
GLboolean (*glIsFramebuffer_evgl_thread_cmd)(GLint framebuffer) = NULL;
GLboolean (*glIsRenderbuffer_evgl_thread_cmd)(GLint renderbuffer) = NULL;
void (*glGetRenderbufferParameteriv_evgl_thread_cmd)(GLenum target, GLenum pname, GLint *params) = NULL;
GLboolean (*glIsTexture_evgl_thread_cmd)(GLint texture) = NULL;


void _evgl_thread_link_init()
{
#define LINK2GENERIC(sym) \
   sym = dlsym(RTLD_DEFAULT, #sym); \
   if (!sym) ERR("Could not find function '%s'", #sym);

   LINK2GENERIC(glGetError_evgl_thread_cmd);
   LINK2GENERIC(glVertexAttribPointer_evgl_thread_cmd);
   LINK2GENERIC(glEnableVertexAttribArray_evgl_thread_cmd);
   LINK2GENERIC(glDisableVertexAttribArray_evgl_thread_cmd);
   LINK2GENERIC(glDrawArrays_evgl_thread_cmd);
   LINK2GENERIC(glDrawElements_evgl_thread_cmd);
   LINK2GENERIC(glGenBuffers_evgl_thread_cmd);
   LINK2GENERIC(glDeleteBuffers_evgl_thread_cmd);
   LINK2GENERIC(glBindBuffer_evgl_thread_cmd);
   LINK2GENERIC(glBufferData_evgl_thread_cmd);
   LINK2GENERIC(glCreateShader_evgl_thread_cmd);
   LINK2GENERIC(glShaderSource_evgl_thread_cmd);
   LINK2GENERIC(glCompileShader_evgl_thread_cmd);
   LINK2GENERIC(glDeleteShader_evgl_thread_cmd);
   LINK2GENERIC(glCreateProgram_evgl_thread_cmd);
   LINK2GENERIC(glAttachShader_evgl_thread_cmd);
   LINK2GENERIC(glDetachShader_evgl_thread_cmd);
   LINK2GENERIC(glLinkProgram_evgl_thread_cmd);
   LINK2GENERIC(glUseProgram_evgl_thread_cmd);
   LINK2GENERIC(glDeleteProgram_evgl_thread_cmd);
   LINK2GENERIC(glGetProgramBinary_orig_evgl_set);
   LINK2GENERIC(glGetProgramBinary_orig_evgl_get);
   LINK2GENERIC(glGetProgramBinary_evgl_thread_cmd);
   LINK2GENERIC(glProgramBinary_orig_evgl_set);
   LINK2GENERIC(glProgramBinary_orig_evgl_get);
   LINK2GENERIC(glProgramBinary_evgl_thread_cmd);
   LINK2GENERIC(glGetActiveAttrib_evgl_thread_cmd);
   LINK2GENERIC(glGetAttribLocation_evgl_thread_cmd);
   LINK2GENERIC(glBindAttribLocation_evgl_thread_cmd);
   LINK2GENERIC(glGetUniformLocation_evgl_thread_cmd);
   LINK2GENERIC(glUniform1f_evgl_thread_cmd);
   LINK2GENERIC(glUniform1i_evgl_thread_cmd);
   LINK2GENERIC(glUniform2f_evgl_thread_cmd);
   LINK2GENERIC(glUniform2i_evgl_thread_cmd);
   LINK2GENERIC(glUniform3f_evgl_thread_cmd);
   LINK2GENERIC(glUniform3i_evgl_thread_cmd);
   LINK2GENERIC(glUniform4f_evgl_thread_cmd);
   LINK2GENERIC(glUniform4i_evgl_thread_cmd);
   LINK2GENERIC(glUniform1fv_evgl_thread_cmd);
   LINK2GENERIC(glUniform1iv_evgl_thread_cmd);
   LINK2GENERIC(glUniform2fv_evgl_thread_cmd);
   LINK2GENERIC(glUniform2iv_evgl_thread_cmd);
   LINK2GENERIC(glUniform3fv_evgl_thread_cmd);
   LINK2GENERIC(glUniform3iv_evgl_thread_cmd);
   LINK2GENERIC(glUniform4fv_evgl_thread_cmd);
   LINK2GENERIC(glUniform4iv_evgl_thread_cmd);
   LINK2GENERIC(glUniformMatrix2fv_evgl_thread_cmd);
   LINK2GENERIC(glUniformMatrix3fv_evgl_thread_cmd);
   LINK2GENERIC(glUniformMatrix4fv_evgl_thread_cmd);
   LINK2GENERIC(glViewport_evgl_thread_cmd);
   LINK2GENERIC(glEnable_evgl_thread_cmd);
   LINK2GENERIC(glDisable_evgl_thread_cmd);
   LINK2GENERIC(glLineWidth_evgl_thread_cmd);
   LINK2GENERIC(glPolygonOffset_evgl_thread_cmd);
   LINK2GENERIC(glPixelStorei_evgl_thread_cmd);
   LINK2GENERIC(glActiveTexture_evgl_thread_cmd);
   LINK2GENERIC(glGenTextures_evgl_thread_cmd);
   LINK2GENERIC(glBindTexture_evgl_thread_cmd);
   LINK2GENERIC(glDeleteTextures_evgl_thread_cmd);
   LINK2GENERIC(glTexImage2D_evgl_thread_cmd);
   LINK2GENERIC(glTexSubImage2D_evgl_thread_cmd);
   LINK2GENERIC(glCompressedTexImage2D_evgl_thread_cmd);
   LINK2GENERIC(glCompressedTexSubImage2D_evgl_thread_cmd);
   LINK2GENERIC(glTexParameterf_evgl_thread_cmd);
   LINK2GENERIC(glTexParameterfv_evgl_thread_cmd);
   LINK2GENERIC(glTexParameteri_evgl_thread_cmd);
   LINK2GENERIC(glTexParameteriv_evgl_thread_cmd);
   LINK2GENERIC(glScissor_evgl_thread_cmd);
   LINK2GENERIC(glBlendFunc_evgl_thread_cmd);
   LINK2GENERIC(glBlendColor_evgl_thread_cmd);
   LINK2GENERIC(glDepthMask_evgl_thread_cmd);
   LINK2GENERIC(glClear_evgl_thread_cmd);
   LINK2GENERIC(glClearColor_evgl_thread_cmd);
   LINK2GENERIC(glReadPixels_evgl_thread_cmd);
   LINK2GENERIC(glGenFramebuffers_evgl_thread_cmd);
   LINK2GENERIC(glBindFramebuffer_evgl_thread_cmd);
   LINK2GENERIC(glDeleteFramebuffers_evgl_thread_cmd);
   LINK2GENERIC(glGenRenderbuffers_evgl_thread_cmd);
   LINK2GENERIC(glBindRenderbuffer_evgl_thread_cmd);
   LINK2GENERIC(glDeleteRenderbuffers_evgl_thread_cmd);
   LINK2GENERIC(glRenderbufferStorage_evgl_thread_cmd);
   LINK2GENERIC(glFramebufferRenderbuffer_evgl_thread_cmd);
   LINK2GENERIC(glFramebufferTexture2D_evgl_thread_cmd);
   LINK2GENERIC(glCheckFramebufferStatus_evgl_thread_cmd);
   LINK2GENERIC(glFlush_evgl_thread_cmd);
   LINK2GENERIC(glFinish_evgl_thread_cmd);
   LINK2GENERIC(glHint_evgl_thread_cmd);
   LINK2GENERIC(glGetString_evgl_thread_cmd);
   LINK2GENERIC(glGetBooleanv_evgl_thread_cmd);
   LINK2GENERIC(glGetFloatv_evgl_thread_cmd);
   LINK2GENERIC(glGetIntegerv_evgl_thread_cmd);
   LINK2GENERIC(glIsBuffer_evgl_thread_cmd);
   LINK2GENERIC(glGetBufferParameteriv_evgl_thread_cmd);
   LINK2GENERIC(glIsShader_evgl_thread_cmd);
   LINK2GENERIC(glGetShaderiv_evgl_thread_cmd);
   LINK2GENERIC(glGetAttachedShaders_evgl_thread_cmd);
   LINK2GENERIC(glGetShaderInfoLog_evgl_thread_cmd);
   LINK2GENERIC(glGetShaderSource_evgl_thread_cmd);
   LINK2GENERIC(glGetShaderPrecisionFormat_evgl_thread_cmd);
   LINK2GENERIC(glGetVertexAttribfv_evgl_thread_cmd);
   LINK2GENERIC(glGetVertexAttribiv_evgl_thread_cmd);
   LINK2GENERIC(glIsProgram_evgl_thread_cmd);
   LINK2GENERIC(glGetProgramInfoLog_evgl_thread_cmd);
   LINK2GENERIC(glGetProgramiv_evgl_thread_cmd);
   LINK2GENERIC(glIsFramebuffer_evgl_thread_cmd);
   LINK2GENERIC(glIsRenderbuffer_evgl_thread_cmd);
   LINK2GENERIC(glGetRenderbufferParameteriv_evgl_thread_cmd);
   LINK2GENERIC(glIsTexture_evgl_thread_cmd);
}
