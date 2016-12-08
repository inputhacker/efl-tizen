/* DO NOT MODIFY THIS FILE AS IT IS AUTO-GENERATED */


#define EVAS_GL_NO_GL_H_CHECK 1
#include "Evas_GL.h"

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_EVAS_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_EVAS_BUILD */
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif /* ! _WIN32 */

EAPI GLenum glGetError_evgl_thread_cmd(void);
EAPI void glVertexAttribPointer_evgl_thread_cmd(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
EAPI void glEnableVertexAttribArray_evgl_thread_cmd(GLuint index);
EAPI void glDisableVertexAttribArray_evgl_thread_cmd(GLuint index);
EAPI void glDrawArrays_evgl_thread_cmd(GLenum mode, GLint first, GLsizei count);
EAPI void glDrawElements_evgl_thread_cmd(GLenum mode, GLsizei count, GLenum type, const void *indices);
EAPI void glGenBuffers_evgl_thread_cmd(GLsizei n, GLuint *buffers);
EAPI void glDeleteBuffers_evgl_thread_cmd(GLsizei n, const GLuint *buffers);
EAPI void glBindBuffer_evgl_thread_cmd(GLenum target, GLuint buffer);
EAPI void glBufferData_evgl_thread_cmd(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
EAPI GLuint glCreateShader_evgl_thread_cmd(GLenum type);
EAPI void glShaderSource_evgl_thread_cmd(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
EAPI void glCompileShader_evgl_thread_cmd(GLuint shader);
EAPI void glDeleteShader_evgl_thread_cmd(GLuint shader);
EAPI GLuint glCreateProgram_evgl_thread_cmd(void);
EAPI void glAttachShader_evgl_thread_cmd(GLuint program, GLuint shader);
EAPI void glDetachShader_evgl_thread_cmd(GLuint program, GLuint shader);
EAPI void glLinkProgram_evgl_thread_cmd(GLuint program);
EAPI void glUseProgram_evgl_thread_cmd(GLuint program);
EAPI void glDeleteProgram_evgl_thread_cmd(GLuint program);

EAPI void glGetProgramBinary_orig_evgl_set(void *func);
EAPI void *glGetProgramBinary_orig_evgl_get(void);
EAPI void glGetProgramBinary_evgl_thread_cmd(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);

EAPI void glProgramBinary_orig_evgl_set(void *func);
EAPI void *glProgramBinary_orig_evgl_get(void);
EAPI void glProgramBinary_evgl_thread_cmd(GLuint program, GLenum binaryFormat, const void *binary, GLint length);
EAPI void glGetActiveAttrib_evgl_thread_cmd(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
EAPI GLint glGetAttribLocation_evgl_thread_cmd(GLuint program, const GLchar *name);
EAPI void glBindAttribLocation_evgl_thread_cmd(GLuint program, GLuint index, const GLchar *name);
EAPI GLint glGetUniformLocation_evgl_thread_cmd(GLuint program, const GLchar *name);
EAPI void glUniform1f_evgl_thread_cmd(GLint location, GLfloat v0);
EAPI void glUniform1i_evgl_thread_cmd(GLint location, GLint v0);
EAPI void glUniform2f_evgl_thread_cmd(GLint location, GLfloat v0, GLfloat v1);
EAPI void glUniform2i_evgl_thread_cmd(GLint location, GLint v0, GLint v1);
EAPI void glUniform3f_evgl_thread_cmd(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
EAPI void glUniform3i_evgl_thread_cmd(GLint location, GLint v0, GLint v1, GLint v2);
EAPI void glUniform4f_evgl_thread_cmd(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
EAPI void glUniform4i_evgl_thread_cmd(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
EAPI void glUniform1fv_evgl_thread_cmd(GLint location, GLsizei count, const GLfloat *value);
EAPI void glUniform1iv_evgl_thread_cmd(GLint location, GLsizei count, const GLint *value);
EAPI void glUniform2fv_evgl_thread_cmd(GLint location, GLsizei count, const GLfloat *value);
EAPI void glUniform2iv_evgl_thread_cmd(GLint location, GLsizei count, const GLint *value);
EAPI void glUniform3fv_evgl_thread_cmd(GLint location, GLsizei count, const GLfloat *value);
EAPI void glUniform3iv_evgl_thread_cmd(GLint location, GLsizei count, const GLint *value);
EAPI void glUniform4fv_evgl_thread_cmd(GLint location, GLsizei count, const GLfloat *value);
EAPI void glUniform4iv_evgl_thread_cmd(GLint location, GLsizei count, const GLint *value);
EAPI void glUniformMatrix2fv_evgl_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glUniformMatrix3fv_evgl_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glUniformMatrix4fv_evgl_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glViewport_evgl_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height);
EAPI void glEnable_evgl_thread_cmd(GLenum cap);
EAPI void glDisable_evgl_thread_cmd(GLenum cap);
EAPI void glLineWidth_evgl_thread_cmd(GLfloat width);
EAPI void glPolygonOffset_evgl_thread_cmd(GLfloat factor, GLfloat units);
EAPI void glPixelStorei_evgl_thread_cmd(GLenum pname, GLint param);
EAPI void glActiveTexture_evgl_thread_cmd(GLenum texture);
EAPI void glGenTextures_evgl_thread_cmd(GLsizei n, GLuint *textures);
EAPI void glBindTexture_evgl_thread_cmd(GLenum target, GLuint texture);
EAPI void glDeleteTextures_evgl_thread_cmd(GLsizei n, const GLuint *textures);
EAPI void glTexImage2D_evgl_thread_cmd(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
EAPI void glTexSubImage2D_evgl_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
EAPI void glCompressedTexImage2D_evgl_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
EAPI void glCompressedTexSubImage2D_evgl_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
EAPI void glTexParameterf_evgl_thread_cmd(GLenum target, GLenum pname, GLfloat param);
EAPI void glTexParameterfv_evgl_thread_cmd(GLenum target, GLenum pname, const GLfloat *params);
EAPI void glTexParameteri_evgl_thread_cmd(GLenum target, GLenum pname, GLint param);
EAPI void glTexParameteriv_evgl_thread_cmd(GLenum target, GLenum pname, const GLint *params);
EAPI void glScissor_evgl_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height);
EAPI void glBlendFunc_evgl_thread_cmd(GLenum sfactor, GLenum dfactor);
EAPI void glBlendColor_evgl_thread_cmd(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
EAPI void glDepthMask_evgl_thread_cmd(GLboolean flag);
EAPI void glClear_evgl_thread_cmd(GLbitfield mask);
EAPI void glClearColor_evgl_thread_cmd(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
EAPI void glReadPixels_evgl_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
EAPI void glGenFramebuffers_evgl_thread_cmd(GLsizei n, GLuint *framebuffers);
EAPI void glBindFramebuffer_evgl_thread_cmd(GLenum target, GLuint framebuffer);
EAPI void glDeleteFramebuffers_evgl_thread_cmd(GLsizei n, const GLuint *framebuffers);
EAPI void glGenRenderbuffers_evgl_thread_cmd(GLsizei n, GLuint *renderbuffers);
EAPI void glBindRenderbuffer_evgl_thread_cmd(GLenum target, GLuint renderbuffer);
EAPI void glDeleteRenderbuffers_evgl_thread_cmd(GLsizei n, const GLuint *renderbuffers);
EAPI void glRenderbufferStorage_evgl_thread_cmd(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
EAPI void glFramebufferRenderbuffer_evgl_thread_cmd(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
EAPI void glFramebufferTexture2D_evgl_thread_cmd(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
EAPI GLenum glCheckFramebufferStatus_evgl_thread_cmd(GLenum target);
EAPI void glFlush_evgl_thread_cmd(void);
EAPI void glFinish_evgl_thread_cmd(void);
EAPI void glHint_evgl_thread_cmd(GLenum target, GLenum mode);
EAPI const GLubyte * glGetString_evgl_thread_cmd(GLenum name);
EAPI void glGetBooleanv_evgl_thread_cmd(GLenum pname, GLboolean *data);
EAPI void glGetFloatv_evgl_thread_cmd(GLenum pname, GLfloat *data);
EAPI void glGetIntegerv_evgl_thread_cmd(GLenum pname, GLint *data);
EAPI GLboolean glIsBuffer_evgl_thread_cmd(GLint buffer);
EAPI void glGetBufferParameteriv_evgl_thread_cmd(GLenum target, GLenum pname, GLint *params);
EAPI GLboolean glIsShader_evgl_thread_cmd(GLuint shader);
EAPI void glGetShaderiv_evgl_thread_cmd(GLuint shader, GLenum pname, GLint *params);
EAPI void glGetAttachedShaders_evgl_thread_cmd(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
EAPI void glGetShaderInfoLog_evgl_thread_cmd(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
EAPI void glGetShaderSource_evgl_thread_cmd(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
EAPI void glGetShaderPrecisionFormat_evgl_thread_cmd(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
EAPI void glGetVertexAttribfv_evgl_thread_cmd(GLuint index, GLenum pname, GLfloat *params);
EAPI void glGetVertexAttribiv_evgl_thread_cmd(GLuint index, GLenum pname, GLint *params);
EAPI GLboolean glIsProgram_evgl_thread_cmd(GLuint program);
EAPI void glGetProgramInfoLog_evgl_thread_cmd(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
EAPI void glGetProgramiv_evgl_thread_cmd(GLuint program, GLenum pname, GLint *params);
EAPI GLboolean glIsFramebuffer_evgl_thread_cmd(GLint framebuffer);
EAPI GLboolean glIsRenderbuffer_evgl_thread_cmd(GLint renderbuffer);
EAPI void glGetRenderbufferParameteriv_evgl_thread_cmd(GLenum target, GLenum pname, GLint *params);
EAPI GLboolean glIsTexture_evgl_thread_cmd(GLint texture);
