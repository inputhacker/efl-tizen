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


extern void (*orig_evgl_api_glActiveTexture)(GLenum texture);
EAPI void glActiveTexture_evgl_api_thread_cmd(GLenum texture);

extern void (*orig_evgl_api_glAttachShader)(GLuint program, GLuint shader);
EAPI void glAttachShader_evgl_api_thread_cmd(GLuint program, GLuint shader);

extern void (*orig_evgl_api_glBindAttribLocation)(GLuint program, GLuint index, const char* name);
EAPI void glBindAttribLocation_evgl_api_thread_cmd(GLuint program, GLuint index, const char* name);

extern void (*orig_evgl_api_glBindBuffer)(GLenum target, GLuint buffer);
EAPI void glBindBuffer_evgl_api_thread_cmd(GLenum target, GLuint buffer);

extern void (*orig_evgl_api_glBindFramebuffer)(GLenum target, GLuint framebuffer);
EAPI void glBindFramebuffer_evgl_api_thread_cmd(GLenum target, GLuint framebuffer);

extern void (*orig_evgl_api_glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
EAPI void glBindRenderbuffer_evgl_api_thread_cmd(GLenum target, GLuint renderbuffer);

extern void (*orig_evgl_api_glBindTexture)(GLenum target, GLuint texture);
EAPI void glBindTexture_evgl_api_thread_cmd(GLenum target, GLuint texture);

extern void (*orig_evgl_api_glBlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
EAPI void glBlendColor_evgl_api_thread_cmd(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

extern void (*orig_evgl_api_glBlendEquation)(GLenum mode);
EAPI void glBlendEquation_evgl_api_thread_cmd(GLenum mode);

extern void (*orig_evgl_api_glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
EAPI void glBlendEquationSeparate_evgl_api_thread_cmd(GLenum modeRGB, GLenum modeAlpha);

extern void (*orig_evgl_api_glBlendFunc)(GLenum sfactor, GLenum dfactor);
EAPI void glBlendFunc_evgl_api_thread_cmd(GLenum sfactor, GLenum dfactor);

extern void (*orig_evgl_api_glBlendFuncSeparate)(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
EAPI void glBlendFuncSeparate_evgl_api_thread_cmd(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

extern void (*orig_evgl_api_glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
EAPI void glBufferData_evgl_api_thread_cmd(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

extern void (*orig_evgl_api_glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
EAPI void glBufferSubData_evgl_api_thread_cmd(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);

extern GLenum (*orig_evgl_api_glCheckFramebufferStatus)(GLenum target);
EAPI GLenum glCheckFramebufferStatus_evgl_api_thread_cmd(GLenum target);

extern void (*orig_evgl_api_glClear)(GLbitfield mask);
EAPI void glClear_evgl_api_thread_cmd(GLbitfield mask);

extern void (*orig_evgl_api_glClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
EAPI void glClearColor_evgl_api_thread_cmd(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

extern void (*orig_evgl_api_glClearDepthf)(GLclampf depth);
EAPI void glClearDepthf_evgl_api_thread_cmd(GLclampf depth);

extern void (*orig_evgl_api_glClearStencil)(GLint s);
EAPI void glClearStencil_evgl_api_thread_cmd(GLint s);

extern void (*orig_evgl_api_glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
EAPI void glColorMask_evgl_api_thread_cmd(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

extern void (*orig_evgl_api_glCompileShader)(GLuint shader);
EAPI void glCompileShader_evgl_api_thread_cmd(GLuint shader);

extern void (*orig_evgl_api_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
EAPI void glCompressedTexImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);

extern void (*orig_evgl_api_glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
EAPI void glCompressedTexSubImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);

extern void (*orig_evgl_api_glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
EAPI void glCopyTexImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);

extern void (*orig_evgl_api_glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
EAPI void glCopyTexSubImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

extern GLuint (*orig_evgl_api_glCreateProgram)(void);
EAPI GLuint glCreateProgram_evgl_api_thread_cmd(void);

extern GLuint (*orig_evgl_api_glCreateShader)(GLenum type);
EAPI GLuint glCreateShader_evgl_api_thread_cmd(GLenum type);

extern void (*orig_evgl_api_glCullFace)(GLenum mode);
EAPI void glCullFace_evgl_api_thread_cmd(GLenum mode);

extern void (*orig_evgl_api_glDeleteBuffers)(GLsizei n, const GLuint* buffers);
EAPI void glDeleteBuffers_evgl_api_thread_cmd(GLsizei n, const GLuint* buffers);

extern void (*orig_evgl_api_glDeleteFramebuffers)(GLsizei n, const GLuint* framebuffers);
EAPI void glDeleteFramebuffers_evgl_api_thread_cmd(GLsizei n, const GLuint* framebuffers);

extern void (*orig_evgl_api_glDeleteProgram)(GLuint program);
EAPI void glDeleteProgram_evgl_api_thread_cmd(GLuint program);

extern void (*orig_evgl_api_glDeleteRenderbuffers)(GLsizei n, const GLuint* renderbuffers);
EAPI void glDeleteRenderbuffers_evgl_api_thread_cmd(GLsizei n, const GLuint* renderbuffers);

extern void (*orig_evgl_api_glDeleteShader)(GLuint shader);
EAPI void glDeleteShader_evgl_api_thread_cmd(GLuint shader);

extern void (*orig_evgl_api_glDeleteTextures)(GLsizei n, const GLuint* textures);
EAPI void glDeleteTextures_evgl_api_thread_cmd(GLsizei n, const GLuint* textures);

extern void (*orig_evgl_api_glDepthFunc)(GLenum func);
EAPI void glDepthFunc_evgl_api_thread_cmd(GLenum func);

extern void (*orig_evgl_api_glDepthMask)(GLboolean flag);
EAPI void glDepthMask_evgl_api_thread_cmd(GLboolean flag);

extern void (*orig_evgl_api_glDepthRangef)(GLclampf zNear, GLclampf zFar);
EAPI void glDepthRangef_evgl_api_thread_cmd(GLclampf zNear, GLclampf zFar);

extern void (*orig_evgl_api_glDetachShader)(GLuint program, GLuint shader);
EAPI void glDetachShader_evgl_api_thread_cmd(GLuint program, GLuint shader);

extern void (*orig_evgl_api_glDisable)(GLenum cap);
EAPI void glDisable_evgl_api_thread_cmd(GLenum cap);

extern void (*orig_evgl_api_glDisableVertexAttribArray)(GLuint index);
EAPI void glDisableVertexAttribArray_evgl_api_thread_cmd(GLuint index);

extern void (*orig_evgl_api_glDrawArrays)(GLenum mode, GLint first, GLsizei count);
EAPI void glDrawArrays_evgl_api_thread_cmd(GLenum mode, GLint first, GLsizei count);

extern void (*orig_evgl_api_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);
EAPI void glDrawElements_evgl_api_thread_cmd(GLenum mode, GLsizei count, GLenum type, const void* indices);

extern void (*orig_evgl_api_glEnable)(GLenum cap);
EAPI void glEnable_evgl_api_thread_cmd(GLenum cap);

extern void (*orig_evgl_api_glEnableVertexAttribArray)(GLuint index);
EAPI void glEnableVertexAttribArray_evgl_api_thread_cmd(GLuint index);

extern void (*orig_evgl_api_glFinish)(void);
EAPI void glFinish_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glFlush)(void);
EAPI void glFlush_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
EAPI void glFramebufferRenderbuffer_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

extern void (*orig_evgl_api_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
EAPI void glFramebufferTexture2D_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

extern void (*orig_evgl_api_glFrontFace)(GLenum mode);
EAPI void glFrontFace_evgl_api_thread_cmd(GLenum mode);

extern void (*orig_evgl_api_glGenBuffers)(GLsizei n, GLuint* buffers);
EAPI void glGenBuffers_evgl_api_thread_cmd(GLsizei n, GLuint* buffers);

extern void (*orig_evgl_api_glGenerateMipmap)(GLenum target);
EAPI void glGenerateMipmap_evgl_api_thread_cmd(GLenum target);

extern void (*orig_evgl_api_glGenFramebuffers)(GLsizei n, GLuint* framebuffers);
EAPI void glGenFramebuffers_evgl_api_thread_cmd(GLsizei n, GLuint* framebuffers);

extern void (*orig_evgl_api_glGenRenderbuffers)(GLsizei n, GLuint* renderbuffers);
EAPI void glGenRenderbuffers_evgl_api_thread_cmd(GLsizei n, GLuint* renderbuffers);

extern void (*orig_evgl_api_glGenTextures)(GLsizei n, GLuint* textures);
EAPI void glGenTextures_evgl_api_thread_cmd(GLsizei n, GLuint* textures);

extern void (*orig_evgl_api_glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
EAPI void glGetActiveAttrib_evgl_api_thread_cmd(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);

extern void (*orig_evgl_api_glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
EAPI void glGetActiveUniform_evgl_api_thread_cmd(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);

extern void (*orig_evgl_api_glGetAttachedShaders)(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
EAPI void glGetAttachedShaders_evgl_api_thread_cmd(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);

extern int (*orig_evgl_api_glGetAttribLocation)(GLuint program, const char* name);
EAPI int glGetAttribLocation_evgl_api_thread_cmd(GLuint program, const char* name);

extern void (*orig_evgl_api_glGetBooleanv)(GLenum pname, GLboolean* params);
EAPI void glGetBooleanv_evgl_api_thread_cmd(GLenum pname, GLboolean* params);

extern void (*orig_evgl_api_glGetBufferParameteriv)(GLenum target, GLenum pname, GLint* params);
EAPI void glGetBufferParameteriv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint* params);

extern GLenum (*orig_evgl_api_glGetError)(void);
EAPI GLenum glGetError_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glGetFloatv)(GLenum pname, GLfloat* params);
EAPI void glGetFloatv_evgl_api_thread_cmd(GLenum pname, GLfloat* params);

extern void (*orig_evgl_api_glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint* params);
EAPI void glGetFramebufferAttachmentParameteriv_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum pname, GLint* params);

extern void (*orig_evgl_api_glGetIntegerv)(GLenum pname, GLint* params);
EAPI void glGetIntegerv_evgl_api_thread_cmd(GLenum pname, GLint* params);

extern void (*orig_evgl_api_glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
EAPI void glGetProgramiv_evgl_api_thread_cmd(GLuint program, GLenum pname, GLint* params);

extern void (*orig_evgl_api_glGetProgramInfoLog)(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
EAPI void glGetProgramInfoLog_evgl_api_thread_cmd(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);

extern void (*orig_evgl_api_glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint* params);
EAPI void glGetRenderbufferParameteriv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint* params);

extern void (*orig_evgl_api_glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
EAPI void glGetShaderiv_evgl_api_thread_cmd(GLuint shader, GLenum pname, GLint* params);

extern void (*orig_evgl_api_glGetShaderInfoLog)(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
EAPI void glGetShaderInfoLog_evgl_api_thread_cmd(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);

extern void (*orig_evgl_api_glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
EAPI void glGetShaderPrecisionFormat_evgl_api_thread_cmd(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);

extern void (*orig_evgl_api_glGetShaderSource)(GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
EAPI void glGetShaderSource_evgl_api_thread_cmd(GLuint shader, GLsizei bufsize, GLsizei* length, char* source);

extern const GLubyte* (*orig_evgl_api_glGetString)(GLenum name);
EAPI const GLubyte* glGetString_evgl_api_thread_cmd(GLenum name);

extern void (*orig_evgl_api_glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat* params);
EAPI void glGetTexParameterfv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfloat* params);

extern void (*orig_evgl_api_glGetTexParameteriv)(GLenum target, GLenum pname, GLint* params);
EAPI void glGetTexParameteriv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint* params);

extern void (*orig_evgl_api_glGetUniformfv)(GLuint program, GLint location, GLfloat* params);
EAPI void glGetUniformfv_evgl_api_thread_cmd(GLuint program, GLint location, GLfloat* params);

extern void (*orig_evgl_api_glGetUniformiv)(GLuint program, GLint location, GLint* params);
EAPI void glGetUniformiv_evgl_api_thread_cmd(GLuint program, GLint location, GLint* params);

extern int (*orig_evgl_api_glGetUniformLocation)(GLuint program, const char* name);
EAPI int glGetUniformLocation_evgl_api_thread_cmd(GLuint program, const char* name);

extern void (*orig_evgl_api_glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat* params);
EAPI void glGetVertexAttribfv_evgl_api_thread_cmd(GLuint index, GLenum pname, GLfloat* params);

extern void (*orig_evgl_api_glGetVertexAttribiv)(GLuint index, GLenum pname, GLint* params);
EAPI void glGetVertexAttribiv_evgl_api_thread_cmd(GLuint index, GLenum pname, GLint* params);

extern void (*orig_evgl_api_glGetVertexAttribPointerv)(GLuint index, GLenum pname, void** pointer);
EAPI void glGetVertexAttribPointerv_evgl_api_thread_cmd(GLuint index, GLenum pname, void** pointer);

extern void (*orig_evgl_api_glHint)(GLenum target, GLenum mode);
EAPI void glHint_evgl_api_thread_cmd(GLenum target, GLenum mode);

extern GLboolean (*orig_evgl_api_glIsBuffer)(GLuint buffer);
EAPI GLboolean glIsBuffer_evgl_api_thread_cmd(GLuint buffer);

extern GLboolean (*orig_evgl_api_glIsEnabled)(GLenum cap);
EAPI GLboolean glIsEnabled_evgl_api_thread_cmd(GLenum cap);

extern GLboolean (*orig_evgl_api_glIsFramebuffer)(GLuint framebuffer);
EAPI GLboolean glIsFramebuffer_evgl_api_thread_cmd(GLuint framebuffer);

extern GLboolean (*orig_evgl_api_glIsProgram)(GLuint program);
EAPI GLboolean glIsProgram_evgl_api_thread_cmd(GLuint program);

extern GLboolean (*orig_evgl_api_glIsRenderbuffer)(GLuint renderbuffer);
EAPI GLboolean glIsRenderbuffer_evgl_api_thread_cmd(GLuint renderbuffer);

extern GLboolean (*orig_evgl_api_glIsShader)(GLuint shader);
EAPI GLboolean glIsShader_evgl_api_thread_cmd(GLuint shader);

extern GLboolean (*orig_evgl_api_glIsTexture)(GLuint texture);
EAPI GLboolean glIsTexture_evgl_api_thread_cmd(GLuint texture);

extern void (*orig_evgl_api_glLineWidth)(GLfloat width);
EAPI void glLineWidth_evgl_api_thread_cmd(GLfloat width);

extern void (*orig_evgl_api_glLinkProgram)(GLuint program);
EAPI void glLinkProgram_evgl_api_thread_cmd(GLuint program);

extern void (*orig_evgl_api_glPixelStorei)(GLenum pname, GLint param);
EAPI void glPixelStorei_evgl_api_thread_cmd(GLenum pname, GLint param);

extern void (*orig_evgl_api_glPolygonOffset)(GLfloat factor, GLfloat units);
EAPI void glPolygonOffset_evgl_api_thread_cmd(GLfloat factor, GLfloat units);

extern void (*orig_evgl_api_glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
EAPI void glReadPixels_evgl_api_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);

extern void (*orig_evgl_api_glReleaseShaderCompiler)(void);
EAPI void glReleaseShaderCompiler_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
EAPI void glRenderbufferStorage_evgl_api_thread_cmd(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

extern void (*orig_evgl_api_glSampleCoverage)(GLclampf value, GLboolean invert);
EAPI void glSampleCoverage_evgl_api_thread_cmd(GLclampf value, GLboolean invert);

extern void (*orig_evgl_api_glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
EAPI void glScissor_evgl_api_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height);

extern void (*orig_evgl_api_glShaderBinary)(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length);
EAPI void glShaderBinary_evgl_api_thread_cmd(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length);

extern void (*orig_evgl_api_glShaderSource)(GLuint shader, GLsizei count, const char* const * string, const GLint* length);
EAPI void glShaderSource_evgl_api_thread_cmd(GLuint shader, GLsizei count, const char* const * string, const GLint* length);

extern void (*orig_evgl_api_glStencilFunc)(GLenum func, GLint ref, GLuint mask);
EAPI void glStencilFunc_evgl_api_thread_cmd(GLenum func, GLint ref, GLuint mask);

extern void (*orig_evgl_api_glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
EAPI void glStencilFuncSeparate_evgl_api_thread_cmd(GLenum face, GLenum func, GLint ref, GLuint mask);

extern void (*orig_evgl_api_glStencilMask)(GLuint mask);
EAPI void glStencilMask_evgl_api_thread_cmd(GLuint mask);

extern void (*orig_evgl_api_glStencilMaskSeparate)(GLenum face, GLuint mask);
EAPI void glStencilMaskSeparate_evgl_api_thread_cmd(GLenum face, GLuint mask);

extern void (*orig_evgl_api_glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
EAPI void glStencilOp_evgl_api_thread_cmd(GLenum fail, GLenum zfail, GLenum zpass);

extern void (*orig_evgl_api_glStencilOpSeparate)(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
EAPI void glStencilOpSeparate_evgl_api_thread_cmd(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);

extern void (*orig_evgl_api_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
EAPI void glTexImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);

extern void (*orig_evgl_api_glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
EAPI void glTexParameterf_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfloat param);

extern void (*orig_evgl_api_glTexParameterfv)(GLenum target, GLenum pname, const GLfloat* params);
EAPI void glTexParameterfv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfloat* params);

extern void (*orig_evgl_api_glTexParameteri)(GLenum target, GLenum pname, GLint param);
EAPI void glTexParameteri_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint param);

extern void (*orig_evgl_api_glTexParameteriv)(GLenum target, GLenum pname, const GLint* params);
EAPI void glTexParameteriv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLint* params);

extern void (*orig_evgl_api_glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
EAPI void glTexSubImage2D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);

extern void (*orig_evgl_api_glUniform1f)(GLint location, GLfloat x);
EAPI void glUniform1f_evgl_api_thread_cmd(GLint location, GLfloat x);

extern void (*orig_evgl_api_glUniform1fv)(GLint location, GLsizei count, const GLfloat* v);
EAPI void glUniform1fv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLfloat* v);

extern void (*orig_evgl_api_glUniform1i)(GLint location, GLint x);
EAPI void glUniform1i_evgl_api_thread_cmd(GLint location, GLint x);

extern void (*orig_evgl_api_glUniform1iv)(GLint location, GLsizei count, const GLint* v);
EAPI void glUniform1iv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLint* v);

extern void (*orig_evgl_api_glUniform2f)(GLint location, GLfloat x, GLfloat y);
EAPI void glUniform2f_evgl_api_thread_cmd(GLint location, GLfloat x, GLfloat y);

extern void (*orig_evgl_api_glUniform2fv)(GLint location, GLsizei count, const GLfloat* v);
EAPI void glUniform2fv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLfloat* v);

extern void (*orig_evgl_api_glUniform2i)(GLint location, GLint x, GLint y);
EAPI void glUniform2i_evgl_api_thread_cmd(GLint location, GLint x, GLint y);

extern void (*orig_evgl_api_glUniform2iv)(GLint location, GLsizei count, const GLint* v);
EAPI void glUniform2iv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLint* v);

extern void (*orig_evgl_api_glUniform3f)(GLint location, GLfloat x, GLfloat y, GLfloat z);
EAPI void glUniform3f_evgl_api_thread_cmd(GLint location, GLfloat x, GLfloat y, GLfloat z);

extern void (*orig_evgl_api_glUniform3fv)(GLint location, GLsizei count, const GLfloat* v);
EAPI void glUniform3fv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLfloat* v);

extern void (*orig_evgl_api_glUniform3i)(GLint location, GLint x, GLint y, GLint z);
EAPI void glUniform3i_evgl_api_thread_cmd(GLint location, GLint x, GLint y, GLint z);

extern void (*orig_evgl_api_glUniform3iv)(GLint location, GLsizei count, const GLint* v);
EAPI void glUniform3iv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLint* v);

extern void (*orig_evgl_api_glUniform4f)(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
EAPI void glUniform4f_evgl_api_thread_cmd(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

extern void (*orig_evgl_api_glUniform4fv)(GLint location, GLsizei count, const GLfloat* v);
EAPI void glUniform4fv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLfloat* v);

extern void (*orig_evgl_api_glUniform4i)(GLint location, GLint x, GLint y, GLint z, GLint w);
EAPI void glUniform4i_evgl_api_thread_cmd(GLint location, GLint x, GLint y, GLint z, GLint w);

extern void (*orig_evgl_api_glUniform4iv)(GLint location, GLsizei count, const GLint* v);
EAPI void glUniform4iv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLint* v);

extern void (*orig_evgl_api_glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
EAPI void glUniformMatrix2fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

extern void (*orig_evgl_api_glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
EAPI void glUniformMatrix3fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

extern void (*orig_evgl_api_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
EAPI void glUniformMatrix4fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

extern void (*orig_evgl_api_glUseProgram)(GLuint program);
EAPI void glUseProgram_evgl_api_thread_cmd(GLuint program);

extern void (*orig_evgl_api_glValidateProgram)(GLuint program);
EAPI void glValidateProgram_evgl_api_thread_cmd(GLuint program);

extern void (*orig_evgl_api_glVertexAttrib1f)(GLuint indx, GLfloat x);
EAPI void glVertexAttrib1f_evgl_api_thread_cmd(GLuint indx, GLfloat x);

extern void (*orig_evgl_api_glVertexAttrib1fv)(GLuint indx, const GLfloat* values);
EAPI void glVertexAttrib1fv_evgl_api_thread_cmd(GLuint indx, const GLfloat* values);

extern void (*orig_evgl_api_glVertexAttrib2f)(GLuint indx, GLfloat x, GLfloat y);
EAPI void glVertexAttrib2f_evgl_api_thread_cmd(GLuint indx, GLfloat x, GLfloat y);

extern void (*orig_evgl_api_glVertexAttrib2fv)(GLuint indx, const GLfloat* values);
EAPI void glVertexAttrib2fv_evgl_api_thread_cmd(GLuint indx, const GLfloat* values);

extern void (*orig_evgl_api_glVertexAttrib3f)(GLuint indx, GLfloat x, GLfloat y, GLfloat z);
EAPI void glVertexAttrib3f_evgl_api_thread_cmd(GLuint indx, GLfloat x, GLfloat y, GLfloat z);

extern void (*orig_evgl_api_glVertexAttrib3fv)(GLuint indx, const GLfloat* values);
EAPI void glVertexAttrib3fv_evgl_api_thread_cmd(GLuint indx, const GLfloat* values);

extern void (*orig_evgl_api_glVertexAttrib4f)(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
EAPI void glVertexAttrib4f_evgl_api_thread_cmd(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

extern void (*orig_evgl_api_glVertexAttrib4fv)(GLuint indx, const GLfloat* values);
EAPI void glVertexAttrib4fv_evgl_api_thread_cmd(GLuint indx, const GLfloat* values);

extern void (*orig_evgl_api_glVertexAttribPointer)(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
EAPI void glVertexAttribPointer_evgl_api_thread_cmd(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);

extern void (*orig_evgl_api_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
EAPI void glViewport_evgl_api_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height);

extern void (*orig_evgl_api_glEvasGLImageTargetTexture2DOES)(GLenum target, EvasGLImage image);
EAPI void glEvasGLImageTargetTexture2DOES_evgl_api_thread_cmd(GLenum target, EvasGLImage image);

extern void (*orig_evgl_api_glEvasGLImageTargetRenderbufferStorageOES)(GLenum target, EvasGLImage image);
EAPI void glEvasGLImageTargetRenderbufferStorageOES_evgl_api_thread_cmd(GLenum target, EvasGLImage image);

extern void (*orig_evgl_api_glGetProgramBinaryOES)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
EAPI void glGetProgramBinaryOES_evgl_api_thread_cmd(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);

extern void (*orig_evgl_api_glProgramBinaryOES)(GLuint program, GLenum binaryFormat, const void *binary, GLint length);
EAPI void glProgramBinaryOES_evgl_api_thread_cmd(GLuint program, GLenum binaryFormat, const void *binary, GLint length);

extern void * (*orig_evgl_api_glMapBufferOES)(GLenum target, GLenum access);
EAPI void * glMapBufferOES_evgl_api_thread_cmd(GLenum target, GLenum access);

extern GLboolean (*orig_evgl_api_glUnmapBufferOES)(GLenum target);
EAPI GLboolean glUnmapBufferOES_evgl_api_thread_cmd(GLenum target);

extern void (*orig_evgl_api_glGetBufferPointervOES)(GLenum target, GLenum pname, void** params);
EAPI void glGetBufferPointervOES_evgl_api_thread_cmd(GLenum target, GLenum pname, void** params);

extern void (*orig_evgl_api_glTexImage3DOES)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
EAPI void glTexImage3DOES_evgl_api_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);

extern void (*orig_evgl_api_glTexSubImage3DOES)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
EAPI void glTexSubImage3DOES_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);

extern void (*orig_evgl_api_glCopyTexSubImage3DOES)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
EAPI void glCopyTexSubImage3DOES_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

extern void (*orig_evgl_api_glCompressedTexImage3DOES)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
EAPI void glCompressedTexImage3DOES_evgl_api_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);

extern void (*orig_evgl_api_glCompressedTexSubImage3DOES)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
EAPI void glCompressedTexSubImage3DOES_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);

extern void (*orig_evgl_api_glFramebufferTexture3DOES)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
EAPI void glFramebufferTexture3DOES_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);

extern void (*orig_evgl_api_glGetPerfMonitorGroupsAMD)(GLint* numGroups, GLsizei groupsSize, GLuint* groups);
EAPI void glGetPerfMonitorGroupsAMD_evgl_api_thread_cmd(GLint* numGroups, GLsizei groupsSize, GLuint* groups);

extern void (*orig_evgl_api_glGetPerfMonitorCountersAMD)(GLuint group, GLint* numCounters, GLint* maxActiveCounters, GLsizei counterSize, GLuint* counters);
EAPI void glGetPerfMonitorCountersAMD_evgl_api_thread_cmd(GLuint group, GLint* numCounters, GLint* maxActiveCounters, GLsizei counterSize, GLuint* counters);

extern void (*orig_evgl_api_glGetPerfMonitorGroupStringAMD)(GLuint group, GLsizei bufSize, GLsizei* length, char* groupString);
EAPI void glGetPerfMonitorGroupStringAMD_evgl_api_thread_cmd(GLuint group, GLsizei bufSize, GLsizei* length, char* groupString);

extern void (*orig_evgl_api_glGetPerfMonitorCounterStringAMD)(GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, char* counterString);
EAPI void glGetPerfMonitorCounterStringAMD_evgl_api_thread_cmd(GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, char* counterString);

extern void (*orig_evgl_api_glGetPerfMonitorCounterInfoAMD)(GLuint group, GLuint counter, GLenum pname, void* data);
EAPI void glGetPerfMonitorCounterInfoAMD_evgl_api_thread_cmd(GLuint group, GLuint counter, GLenum pname, void* data);

extern void (*orig_evgl_api_glGenPerfMonitorsAMD)(GLsizei n, GLuint* monitors);
EAPI void glGenPerfMonitorsAMD_evgl_api_thread_cmd(GLsizei n, GLuint* monitors);

extern void (*orig_evgl_api_glDeletePerfMonitorsAMD)(GLsizei n, GLuint* monitors);
EAPI void glDeletePerfMonitorsAMD_evgl_api_thread_cmd(GLsizei n, GLuint* monitors);

extern void (*orig_evgl_api_glSelectPerfMonitorCountersAMD)(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* countersList);
EAPI void glSelectPerfMonitorCountersAMD_evgl_api_thread_cmd(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* countersList);

extern void (*orig_evgl_api_glBeginPerfMonitorAMD)(GLuint monitor);
EAPI void glBeginPerfMonitorAMD_evgl_api_thread_cmd(GLuint monitor);

extern void (*orig_evgl_api_glEndPerfMonitorAMD)(GLuint monitor);
EAPI void glEndPerfMonitorAMD_evgl_api_thread_cmd(GLuint monitor);

extern void (*orig_evgl_api_glGetPerfMonitorCounterDataAMD)(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint* bytesWritten);
EAPI void glGetPerfMonitorCounterDataAMD_evgl_api_thread_cmd(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint* bytesWritten);

extern void (*orig_evgl_api_glDiscardFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum* attachments);
EAPI void glDiscardFramebuffer_evgl_api_thread_cmd(GLenum target, GLsizei numAttachments, const GLenum* attachments);

extern void (*orig_evgl_api_glDiscardFramebufferEXT)(GLenum target, GLsizei numAttachments, const GLenum* attachments);
EAPI void glDiscardFramebufferEXT_evgl_api_thread_cmd(GLenum target, GLsizei numAttachments, const GLenum* attachments);

extern void (*orig_evgl_api_glMultiDrawArrays)(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount);
EAPI void glMultiDrawArrays_evgl_api_thread_cmd(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount);

extern void (*orig_evgl_api_glMultiDrawArraysEXT)(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount);
EAPI void glMultiDrawArraysEXT_evgl_api_thread_cmd(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount);

extern void (*orig_evgl_api_glMultiDrawElements)(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount);
EAPI void glMultiDrawElements_evgl_api_thread_cmd(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount);

extern void (*orig_evgl_api_glMultiDrawElementsEXT)(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount);
EAPI void glMultiDrawElementsEXT_evgl_api_thread_cmd(GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount);

extern void (*orig_evgl_api_glDeleteFencesNV)(GLsizei n, const GLuint* fences);
EAPI void glDeleteFencesNV_evgl_api_thread_cmd(GLsizei n, const GLuint* fences);

extern void (*orig_evgl_api_glGenFencesNV)(GLsizei n, GLuint* fences);
EAPI void glGenFencesNV_evgl_api_thread_cmd(GLsizei n, GLuint* fences);

extern GLboolean (*orig_evgl_api_glIsFenceNV)(GLuint fence);
EAPI GLboolean glIsFenceNV_evgl_api_thread_cmd(GLuint fence);

extern GLboolean (*orig_evgl_api_glTestFenceNV)(GLuint fence);
EAPI GLboolean glTestFenceNV_evgl_api_thread_cmd(GLuint fence);

extern void (*orig_evgl_api_glGetFenceivNV)(GLuint fence, GLenum pname, GLint* params);
EAPI void glGetFenceivNV_evgl_api_thread_cmd(GLuint fence, GLenum pname, GLint* params);

extern void (*orig_evgl_api_glFinishFenceNV)(GLuint fence);
EAPI void glFinishFenceNV_evgl_api_thread_cmd(GLuint fence);

extern void (*orig_evgl_api_glSetFenceNV)(GLuint a, GLenum b);
EAPI void glSetFenceNV_evgl_api_thread_cmd(GLuint a, GLenum b);

extern void (*orig_evgl_api_glGetDriverControlsQCOM)(GLint* num, GLsizei size, GLuint* driverControls);
EAPI void glGetDriverControlsQCOM_evgl_api_thread_cmd(GLint* num, GLsizei size, GLuint* driverControls);

extern void (*orig_evgl_api_glGetDriverControlStringQCOM)(GLuint driverControl, GLsizei bufSize, GLsizei* length, char* driverControlString);
EAPI void glGetDriverControlStringQCOM_evgl_api_thread_cmd(GLuint driverControl, GLsizei bufSize, GLsizei* length, char* driverControlString);

extern void (*orig_evgl_api_glEnableDriverControlQCOM)(GLuint driverControl);
EAPI void glEnableDriverControlQCOM_evgl_api_thread_cmd(GLuint driverControl);

extern void (*orig_evgl_api_glDisableDriverControlQCOM)(GLuint driverControl);
EAPI void glDisableDriverControlQCOM_evgl_api_thread_cmd(GLuint driverControl);

extern void (*orig_evgl_api_glExtGetTexturesQCOM)(GLuint* textures, GLint maxTextures, GLint* numTextures);
EAPI void glExtGetTexturesQCOM_evgl_api_thread_cmd(GLuint* textures, GLint maxTextures, GLint* numTextures);

extern void (*orig_evgl_api_glExtGetBuffersQCOM)(GLuint* buffers, GLint maxBuffers, GLint* numBuffers);
EAPI void glExtGetBuffersQCOM_evgl_api_thread_cmd(GLuint* buffers, GLint maxBuffers, GLint* numBuffers);

extern void (*orig_evgl_api_glExtGetRenderbuffersQCOM)(GLuint* renderbuffers, GLint maxRenderbuffers, GLint* numRenderbuffers);
EAPI void glExtGetRenderbuffersQCOM_evgl_api_thread_cmd(GLuint* renderbuffers, GLint maxRenderbuffers, GLint* numRenderbuffers);

extern void (*orig_evgl_api_glExtGetFramebuffersQCOM)(GLuint* framebuffers, GLint maxFramebuffers, GLint* numFramebuffers);
EAPI void glExtGetFramebuffersQCOM_evgl_api_thread_cmd(GLuint* framebuffers, GLint maxFramebuffers, GLint* numFramebuffers);

extern void (*orig_evgl_api_glExtGetTexLevelParameterivQCOM)(GLuint texture, GLenum face, GLint level, GLenum pname, GLint* params);
EAPI void glExtGetTexLevelParameterivQCOM_evgl_api_thread_cmd(GLuint texture, GLenum face, GLint level, GLenum pname, GLint* params);

extern void (*orig_evgl_api_glExtTexObjectStateOverrideiQCOM)(GLenum target, GLenum pname, GLint param);
EAPI void glExtTexObjectStateOverrideiQCOM_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint param);

extern void (*orig_evgl_api_glExtGetTexSubImageQCOM)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void* texels);
EAPI void glExtGetTexSubImageQCOM_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void* texels);

extern void (*orig_evgl_api_glExtGetBufferPointervQCOM)(GLenum target, void** params);
EAPI void glExtGetBufferPointervQCOM_evgl_api_thread_cmd(GLenum target, void** params);

extern void (*orig_evgl_api_glExtGetShadersQCOM)(GLuint* shaders, GLint maxShaders, GLint* numShaders);
EAPI void glExtGetShadersQCOM_evgl_api_thread_cmd(GLuint* shaders, GLint maxShaders, GLint* numShaders);

extern void (*orig_evgl_api_glExtGetProgramsQCOM)(GLuint* programs, GLint maxPrograms, GLint* numPrograms);
EAPI void glExtGetProgramsQCOM_evgl_api_thread_cmd(GLuint* programs, GLint maxPrograms, GLint* numPrograms);

extern GLboolean (*orig_evgl_api_glExtIsProgramBinaryQCOM)(GLuint program);
EAPI GLboolean glExtIsProgramBinaryQCOM_evgl_api_thread_cmd(GLuint program);

extern void (*orig_evgl_api_glExtGetProgramBinarySourceQCOM)(GLuint program, GLenum shadertype, char* source, GLint* length);
EAPI void glExtGetProgramBinarySourceQCOM_evgl_api_thread_cmd(GLuint program, GLenum shadertype, char* source, GLint* length);

extern void (*orig_evgl_api_glAlphaFunc)(GLenum func, GLclampf ref);
EAPI void glAlphaFunc_evgl_api_thread_cmd(GLenum func, GLclampf ref);

extern void (*orig_evgl_api_glClipPlanef)(GLenum plane, const GLfloat *equation);
EAPI void glClipPlanef_evgl_api_thread_cmd(GLenum plane, const GLfloat *equation);

extern void (*orig_evgl_api_glColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
EAPI void glColor4f_evgl_api_thread_cmd(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

extern void (*orig_evgl_api_glFogf)(GLenum pname, GLfloat param);
EAPI void glFogf_evgl_api_thread_cmd(GLenum pname, GLfloat param);

extern void (*orig_evgl_api_glFogfv)(GLenum pname, const GLfloat *params);
EAPI void glFogfv_evgl_api_thread_cmd(GLenum pname, const GLfloat *params);

extern void (*orig_evgl_api_glFrustumf)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
EAPI void glFrustumf_evgl_api_thread_cmd(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

extern void (*orig_evgl_api_glGetClipPlanef)(GLenum pname, GLfloat eqn[4]);
EAPI void glGetClipPlanef_evgl_api_thread_cmd(GLenum pname, GLfloat eqn[4]);

extern void (*orig_evgl_api_glGetLightfv)(GLenum light, GLenum pname, GLfloat *params);
EAPI void glGetLightfv_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfloat *params);

extern void (*orig_evgl_api_glGetMaterialfv)(GLenum face, GLenum pname, GLfloat *params);
EAPI void glGetMaterialfv_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfloat *params);

extern void (*orig_evgl_api_glGetTexEnvfv)(GLenum env, GLenum pname, GLfloat *params);
EAPI void glGetTexEnvfv_evgl_api_thread_cmd(GLenum env, GLenum pname, GLfloat *params);

extern void (*orig_evgl_api_glLightModelf)(GLenum pname, GLfloat param);
EAPI void glLightModelf_evgl_api_thread_cmd(GLenum pname, GLfloat param);

extern void (*orig_evgl_api_glLightModelfv)(GLenum pname, const GLfloat *params);
EAPI void glLightModelfv_evgl_api_thread_cmd(GLenum pname, const GLfloat *params);

extern void (*orig_evgl_api_glLightf)(GLenum light, GLenum pname, GLfloat param);
EAPI void glLightf_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfloat param);

extern void (*orig_evgl_api_glLightfv)(GLenum light, GLenum pname, const GLfloat *params);
EAPI void glLightfv_evgl_api_thread_cmd(GLenum light, GLenum pname, const GLfloat *params);

extern void (*orig_evgl_api_glLoadMatrixf)(const GLfloat *m);
EAPI void glLoadMatrixf_evgl_api_thread_cmd(const GLfloat *m);

extern void (*orig_evgl_api_glMaterialf)(GLenum face, GLenum pname, GLfloat param);
EAPI void glMaterialf_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfloat param);

extern void (*orig_evgl_api_glMaterialfv)(GLenum face, GLenum pname, const GLfloat *params);
EAPI void glMaterialfv_evgl_api_thread_cmd(GLenum face, GLenum pname, const GLfloat *params);

extern void (*orig_evgl_api_glMultMatrixf)(const GLfloat *m);
EAPI void glMultMatrixf_evgl_api_thread_cmd(const GLfloat *m);

extern void (*orig_evgl_api_glMultiTexCoord4f)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
EAPI void glMultiTexCoord4f_evgl_api_thread_cmd(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);

extern void (*orig_evgl_api_glNormal3f)(GLfloat nx, GLfloat ny, GLfloat nz);
EAPI void glNormal3f_evgl_api_thread_cmd(GLfloat nx, GLfloat ny, GLfloat nz);

extern void (*orig_evgl_api_glOrthof)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
EAPI void glOrthof_evgl_api_thread_cmd(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

extern void (*orig_evgl_api_glPointParameterf)(GLenum pname, GLfloat param);
EAPI void glPointParameterf_evgl_api_thread_cmd(GLenum pname, GLfloat param);

extern void (*orig_evgl_api_glPointParameterfv)(GLenum pname, const GLfloat *params);
EAPI void glPointParameterfv_evgl_api_thread_cmd(GLenum pname, const GLfloat *params);

extern void (*orig_evgl_api_glPointSize)(GLfloat size);
EAPI void glPointSize_evgl_api_thread_cmd(GLfloat size);

extern void (*orig_evgl_api_glPointSizePointerOES)(GLenum type, GLsizei stride, const GLvoid * pointer);
EAPI void glPointSizePointerOES_evgl_api_thread_cmd(GLenum type, GLsizei stride, const GLvoid * pointer);

extern void (*orig_evgl_api_glRotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
EAPI void glRotatef_evgl_api_thread_cmd(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

extern void (*orig_evgl_api_glScalef)(GLfloat x, GLfloat y, GLfloat z);
EAPI void glScalef_evgl_api_thread_cmd(GLfloat x, GLfloat y, GLfloat z);

extern void (*orig_evgl_api_glTexEnvf)(GLenum target, GLenum pname, GLfloat param);
EAPI void glTexEnvf_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfloat param);

extern void (*orig_evgl_api_glTexEnvfv)(GLenum target, GLenum pname, const GLfloat *params);
EAPI void glTexEnvfv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfloat *params);

extern void (*orig_evgl_api_glTranslatef)(GLfloat x, GLfloat y, GLfloat z);
EAPI void glTranslatef_evgl_api_thread_cmd(GLfloat x, GLfloat y, GLfloat z);

extern void (*orig_evgl_api_glAlphaFuncx)(GLenum func, GLclampx ref);
EAPI void glAlphaFuncx_evgl_api_thread_cmd(GLenum func, GLclampx ref);

extern void (*orig_evgl_api_glClearColorx)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
EAPI void glClearColorx_evgl_api_thread_cmd(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);

extern void (*orig_evgl_api_glClearDepthx)(GLclampx depth);
EAPI void glClearDepthx_evgl_api_thread_cmd(GLclampx depth);

extern void (*orig_evgl_api_glClientActiveTexture)(GLenum texture);
EAPI void glClientActiveTexture_evgl_api_thread_cmd(GLenum texture);

extern void (*orig_evgl_api_glClipPlanex)(GLenum plane, const GLfixed *equation);
EAPI void glClipPlanex_evgl_api_thread_cmd(GLenum plane, const GLfixed *equation);

extern void (*orig_evgl_api_glColor4ub)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
EAPI void glColor4ub_evgl_api_thread_cmd(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);

extern void (*orig_evgl_api_glColor4x)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
EAPI void glColor4x_evgl_api_thread_cmd(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);

extern void (*orig_evgl_api_glColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EAPI void glColorPointer_evgl_api_thread_cmd(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

extern void (*orig_evgl_api_glDepthRangex)(GLclampx zNear, GLclampx zFar);
EAPI void glDepthRangex_evgl_api_thread_cmd(GLclampx zNear, GLclampx zFar);

extern void (*orig_evgl_api_glDisableClientState)(GLenum array);
EAPI void glDisableClientState_evgl_api_thread_cmd(GLenum array);

extern void (*orig_evgl_api_glEnableClientState)(GLenum array);
EAPI void glEnableClientState_evgl_api_thread_cmd(GLenum array);

extern void (*orig_evgl_api_glFogx)(GLenum pname, GLfixed param);
EAPI void glFogx_evgl_api_thread_cmd(GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glFogxv)(GLenum pname, const GLfixed *params);
EAPI void glFogxv_evgl_api_thread_cmd(GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glFrustumx)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
EAPI void glFrustumx_evgl_api_thread_cmd(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);

extern void (*orig_evgl_api_glGetClipPlanex)(GLenum pname, GLfixed eqn[4]);
EAPI void glGetClipPlanex_evgl_api_thread_cmd(GLenum pname, GLfixed eqn[4]);

extern void (*orig_evgl_api_glGetFixedv)(GLenum pname, GLfixed *params);
EAPI void glGetFixedv_evgl_api_thread_cmd(GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glGetLightxv)(GLenum light, GLenum pname, GLfixed *params);
EAPI void glGetLightxv_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glGetMaterialxv)(GLenum face, GLenum pname, GLfixed *params);
EAPI void glGetMaterialxv_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glGetPointerv)(GLenum pname, GLvoid **params);
EAPI void glGetPointerv_evgl_api_thread_cmd(GLenum pname, GLvoid **params);

extern void (*orig_evgl_api_glGetTexEnviv)(GLenum env, GLenum pname, GLint *params);
EAPI void glGetTexEnviv_evgl_api_thread_cmd(GLenum env, GLenum pname, GLint *params);

extern void (*orig_evgl_api_glGetTexEnvxv)(GLenum env, GLenum pname, GLfixed *params);
EAPI void glGetTexEnvxv_evgl_api_thread_cmd(GLenum env, GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glGetTexParameterxv)(GLenum target, GLenum pname, GLfixed *params);
EAPI void glGetTexParameterxv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glLightModelx)(GLenum pname, GLfixed param);
EAPI void glLightModelx_evgl_api_thread_cmd(GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glLightModelxv)(GLenum pname, const GLfixed *params);
EAPI void glLightModelxv_evgl_api_thread_cmd(GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glLightx)(GLenum light, GLenum pname, GLfixed param);
EAPI void glLightx_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glLightxv)(GLenum light, GLenum pname, const GLfixed *params);
EAPI void glLightxv_evgl_api_thread_cmd(GLenum light, GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glLineWidthx)(GLfixed width);
EAPI void glLineWidthx_evgl_api_thread_cmd(GLfixed width);

extern void (*orig_evgl_api_glLoadIdentity)(void);
EAPI void glLoadIdentity_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glLoadMatrixx)(const GLfixed *m);
EAPI void glLoadMatrixx_evgl_api_thread_cmd(const GLfixed *m);

extern void (*orig_evgl_api_glLogicOp)(GLenum opcode);
EAPI void glLogicOp_evgl_api_thread_cmd(GLenum opcode);

extern void (*orig_evgl_api_glMaterialx)(GLenum face, GLenum pname, GLfixed param);
EAPI void glMaterialx_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glMaterialxv)(GLenum face, GLenum pname, const GLfixed *params);
EAPI void glMaterialxv_evgl_api_thread_cmd(GLenum face, GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glMatrixMode)(GLenum mode);
EAPI void glMatrixMode_evgl_api_thread_cmd(GLenum mode);

extern void (*orig_evgl_api_glMultMatrixx)(const GLfixed *m);
EAPI void glMultMatrixx_evgl_api_thread_cmd(const GLfixed *m);

extern void (*orig_evgl_api_glMultiTexCoord4x)(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
EAPI void glMultiTexCoord4x_evgl_api_thread_cmd(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);

extern void (*orig_evgl_api_glNormal3x)(GLfixed nx, GLfixed ny, GLfixed nz);
EAPI void glNormal3x_evgl_api_thread_cmd(GLfixed nx, GLfixed ny, GLfixed nz);

extern void (*orig_evgl_api_glNormalPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
EAPI void glNormalPointer_evgl_api_thread_cmd(GLenum type, GLsizei stride, const GLvoid *pointer);

extern void (*orig_evgl_api_glOrthox)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
EAPI void glOrthox_evgl_api_thread_cmd(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);

extern void (*orig_evgl_api_glPointParameterx)(GLenum pname, GLfixed param);
EAPI void glPointParameterx_evgl_api_thread_cmd(GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glPointParameterxv)(GLenum pname, const GLfixed *params);
EAPI void glPointParameterxv_evgl_api_thread_cmd(GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glPointSizex)(GLfixed size);
EAPI void glPointSizex_evgl_api_thread_cmd(GLfixed size);

extern void (*orig_evgl_api_glPolygonOffsetx)(GLfixed factor, GLfixed units);
EAPI void glPolygonOffsetx_evgl_api_thread_cmd(GLfixed factor, GLfixed units);

extern void (*orig_evgl_api_glPopMatrix)(void);
EAPI void glPopMatrix_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glPushMatrix)(void);
EAPI void glPushMatrix_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glRotatex)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
EAPI void glRotatex_evgl_api_thread_cmd(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);

extern void (*orig_evgl_api_glSampleCoveragex)(GLclampx value, GLboolean invert);
EAPI void glSampleCoveragex_evgl_api_thread_cmd(GLclampx value, GLboolean invert);

extern void (*orig_evgl_api_glScalex)(GLfixed x, GLfixed y, GLfixed z);
EAPI void glScalex_evgl_api_thread_cmd(GLfixed x, GLfixed y, GLfixed z);

extern void (*orig_evgl_api_glShadeModel)(GLenum mode);
EAPI void glShadeModel_evgl_api_thread_cmd(GLenum mode);

extern void (*orig_evgl_api_glTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EAPI void glTexCoordPointer_evgl_api_thread_cmd(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

extern void (*orig_evgl_api_glTexEnvi)(GLenum target, GLenum pname, GLint param);
EAPI void glTexEnvi_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint param);

extern void (*orig_evgl_api_glTexEnvx)(GLenum target, GLenum pname, GLfixed param);
EAPI void glTexEnvx_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glTexEnviv)(GLenum target, GLenum pname, const GLint *params);
EAPI void glTexEnviv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLint *params);

extern void (*orig_evgl_api_glTexEnvxv)(GLenum target, GLenum pname, const GLfixed *params);
EAPI void glTexEnvxv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glTexParameterx)(GLenum target, GLenum pname, GLfixed param);
EAPI void glTexParameterx_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glTexParameterxv)(GLenum target, GLenum pname, const GLfixed *params);
EAPI void glTexParameterxv_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glTranslatex)(GLfixed x, GLfixed y, GLfixed z);
EAPI void glTranslatex_evgl_api_thread_cmd(GLfixed x, GLfixed y, GLfixed z);

extern void (*orig_evgl_api_glVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EAPI void glVertexPointer_evgl_api_thread_cmd(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

extern void (*orig_evgl_api_glBlendEquationSeparateOES)(GLenum modeRGB, GLenum modeAlpha);
EAPI void glBlendEquationSeparateOES_evgl_api_thread_cmd(GLenum modeRGB, GLenum modeAlpha);

extern void (*orig_evgl_api_glBlendFuncSeparateOES)(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
EAPI void glBlendFuncSeparateOES_evgl_api_thread_cmd(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

extern void (*orig_evgl_api_glBlendEquationOES)(GLenum mode);
EAPI void glBlendEquationOES_evgl_api_thread_cmd(GLenum mode);

extern void (*orig_evgl_api_glDrawTexsOES)(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height);
EAPI void glDrawTexsOES_evgl_api_thread_cmd(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height);

extern void (*orig_evgl_api_glDrawTexiOES)(GLint x, GLint y, GLint z, GLint width, GLint height);
EAPI void glDrawTexiOES_evgl_api_thread_cmd(GLint x, GLint y, GLint z, GLint width, GLint height);

extern void (*orig_evgl_api_glDrawTexxOES)(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height);
EAPI void glDrawTexxOES_evgl_api_thread_cmd(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height);

extern void (*orig_evgl_api_glDrawTexsvOES)(const GLshort *coords);
EAPI void glDrawTexsvOES_evgl_api_thread_cmd(const GLshort *coords);

extern void (*orig_evgl_api_glDrawTexivOES)(const GLint *coords);
EAPI void glDrawTexivOES_evgl_api_thread_cmd(const GLint *coords);

extern void (*orig_evgl_api_glDrawTexxvOES)(const GLfixed *coords);
EAPI void glDrawTexxvOES_evgl_api_thread_cmd(const GLfixed *coords);

extern void (*orig_evgl_api_glDrawTexfOES)(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height);
EAPI void glDrawTexfOES_evgl_api_thread_cmd(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height);

extern void (*orig_evgl_api_glDrawTexfvOES)(const GLfloat *coords);
EAPI void glDrawTexfvOES_evgl_api_thread_cmd(const GLfloat *coords);

extern void (*orig_evgl_api_glAlphaFuncxOES)(GLenum func, GLclampx ref);
EAPI void glAlphaFuncxOES_evgl_api_thread_cmd(GLenum func, GLclampx ref);

extern void (*orig_evgl_api_glClearColorxOES)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
EAPI void glClearColorxOES_evgl_api_thread_cmd(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);

extern void (*orig_evgl_api_glClearDepthxOES)(GLclampx depth);
EAPI void glClearDepthxOES_evgl_api_thread_cmd(GLclampx depth);

extern void (*orig_evgl_api_glClipPlanexOES)(GLenum plane, const GLfixed *equation);
EAPI void glClipPlanexOES_evgl_api_thread_cmd(GLenum plane, const GLfixed *equation);

extern void (*orig_evgl_api_glColor4xOES)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
EAPI void glColor4xOES_evgl_api_thread_cmd(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);

extern void (*orig_evgl_api_glDepthRangexOES)(GLclampx zNear, GLclampx zFar);
EAPI void glDepthRangexOES_evgl_api_thread_cmd(GLclampx zNear, GLclampx zFar);

extern void (*orig_evgl_api_glFogxOES)(GLenum pname, GLfixed param);
EAPI void glFogxOES_evgl_api_thread_cmd(GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glFogxvOES)(GLenum pname, const GLfixed *params);
EAPI void glFogxvOES_evgl_api_thread_cmd(GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glFrustumxOES)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
EAPI void glFrustumxOES_evgl_api_thread_cmd(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);

extern void (*orig_evgl_api_glGetClipPlanexOES)(GLenum pname, GLfixed eqn[4]);
EAPI void glGetClipPlanexOES_evgl_api_thread_cmd(GLenum pname, GLfixed eqn[4]);

extern void (*orig_evgl_api_glGetFixedvOES)(GLenum pname, GLfixed *params);
EAPI void glGetFixedvOES_evgl_api_thread_cmd(GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glGetLightxvOES)(GLenum light, GLenum pname, GLfixed *params);
EAPI void glGetLightxvOES_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glGetMaterialxvOES)(GLenum face, GLenum pname, GLfixed *params);
EAPI void glGetMaterialxvOES_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glGetTexEnvxvOES)(GLenum env, GLenum pname, GLfixed *params);
EAPI void glGetTexEnvxvOES_evgl_api_thread_cmd(GLenum env, GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glGetTexParameterxvOES)(GLenum target, GLenum pname, GLfixed *params);
EAPI void glGetTexParameterxvOES_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glLightModelxOES)(GLenum pname, GLfixed param);
EAPI void glLightModelxOES_evgl_api_thread_cmd(GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glLightModelxvOES)(GLenum pname, const GLfixed *params);
EAPI void glLightModelxvOES_evgl_api_thread_cmd(GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glLightxOES)(GLenum light, GLenum pname, GLfixed param);
EAPI void glLightxOES_evgl_api_thread_cmd(GLenum light, GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glLightxvOES)(GLenum light, GLenum pname, const GLfixed *params);
EAPI void glLightxvOES_evgl_api_thread_cmd(GLenum light, GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glLineWidthxOES)(GLfixed width);
EAPI void glLineWidthxOES_evgl_api_thread_cmd(GLfixed width);

extern void (*orig_evgl_api_glLoadMatrixxOES)(const GLfixed *m);
EAPI void glLoadMatrixxOES_evgl_api_thread_cmd(const GLfixed *m);

extern void (*orig_evgl_api_glMaterialxOES)(GLenum face, GLenum pname, GLfixed param);
EAPI void glMaterialxOES_evgl_api_thread_cmd(GLenum face, GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glMaterialxvOES)(GLenum face, GLenum pname, const GLfixed *params);
EAPI void glMaterialxvOES_evgl_api_thread_cmd(GLenum face, GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glMultMatrixxOES)(const GLfixed *m);
EAPI void glMultMatrixxOES_evgl_api_thread_cmd(const GLfixed *m);

extern void (*orig_evgl_api_glMultiTexCoord4xOES)(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
EAPI void glMultiTexCoord4xOES_evgl_api_thread_cmd(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);

extern void (*orig_evgl_api_glNormal3xOES)(GLfixed nx, GLfixed ny, GLfixed nz);
EAPI void glNormal3xOES_evgl_api_thread_cmd(GLfixed nx, GLfixed ny, GLfixed nz);

extern void (*orig_evgl_api_glOrthoxOES)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
EAPI void glOrthoxOES_evgl_api_thread_cmd(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);

extern void (*orig_evgl_api_glPointParameterxOES)(GLenum pname, GLfixed param);
EAPI void glPointParameterxOES_evgl_api_thread_cmd(GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glPointParameterxvOES)(GLenum pname, const GLfixed *params);
EAPI void glPointParameterxvOES_evgl_api_thread_cmd(GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glPointSizexOES)(GLfixed size);
EAPI void glPointSizexOES_evgl_api_thread_cmd(GLfixed size);

extern void (*orig_evgl_api_glPolygonOffsetxOES)(GLfixed factor, GLfixed units);
EAPI void glPolygonOffsetxOES_evgl_api_thread_cmd(GLfixed factor, GLfixed units);

extern void (*orig_evgl_api_glRotatexOES)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
EAPI void glRotatexOES_evgl_api_thread_cmd(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);

extern void (*orig_evgl_api_glSampleCoveragexOES)(GLclampx value, GLboolean invert);
EAPI void glSampleCoveragexOES_evgl_api_thread_cmd(GLclampx value, GLboolean invert);

extern void (*orig_evgl_api_glScalexOES)(GLfixed x, GLfixed y, GLfixed z);
EAPI void glScalexOES_evgl_api_thread_cmd(GLfixed x, GLfixed y, GLfixed z);

extern void (*orig_evgl_api_glTexEnvxOES)(GLenum target, GLenum pname, GLfixed param);
EAPI void glTexEnvxOES_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glTexEnvxvOES)(GLenum target, GLenum pname, const GLfixed *params);
EAPI void glTexEnvxvOES_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glTexParameterxOES)(GLenum target, GLenum pname, GLfixed param);
EAPI void glTexParameterxOES_evgl_api_thread_cmd(GLenum target, GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glTexParameterxvOES)(GLenum target, GLenum pname, const GLfixed *params);
EAPI void glTexParameterxvOES_evgl_api_thread_cmd(GLenum target, GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glTranslatexOES)(GLfixed x, GLfixed y, GLfixed z);
EAPI void glTranslatexOES_evgl_api_thread_cmd(GLfixed x, GLfixed y, GLfixed z);

extern GLboolean (*orig_evgl_api_glIsRenderbufferOES)(GLuint renderbuffer);
EAPI GLboolean glIsRenderbufferOES_evgl_api_thread_cmd(GLuint renderbuffer);

extern void (*orig_evgl_api_glBindRenderbufferOES)(GLenum target, GLuint renderbuffer);
EAPI void glBindRenderbufferOES_evgl_api_thread_cmd(GLenum target, GLuint renderbuffer);

extern void (*orig_evgl_api_glDeleteRenderbuffersOES)(GLsizei n, const GLuint* renderbuffers);
EAPI void glDeleteRenderbuffersOES_evgl_api_thread_cmd(GLsizei n, const GLuint* renderbuffers);

extern void (*orig_evgl_api_glGenRenderbuffersOES)(GLsizei n, GLuint* renderbuffers);
EAPI void glGenRenderbuffersOES_evgl_api_thread_cmd(GLsizei n, GLuint* renderbuffers);

extern void (*orig_evgl_api_glRenderbufferStorageOES)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
EAPI void glRenderbufferStorageOES_evgl_api_thread_cmd(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

extern void (*orig_evgl_api_glGetRenderbufferParameterivOES)(GLenum target, GLenum pname, GLint* params);
EAPI void glGetRenderbufferParameterivOES_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint* params);

extern GLboolean (*orig_evgl_api_glIsFramebufferOES)(GLuint framebuffer);
EAPI GLboolean glIsFramebufferOES_evgl_api_thread_cmd(GLuint framebuffer);

extern void (*orig_evgl_api_glBindFramebufferOES)(GLenum target, GLuint framebuffer);
EAPI void glBindFramebufferOES_evgl_api_thread_cmd(GLenum target, GLuint framebuffer);

extern void (*orig_evgl_api_glDeleteFramebuffersOES)(GLsizei n, const GLuint* framebuffers);
EAPI void glDeleteFramebuffersOES_evgl_api_thread_cmd(GLsizei n, const GLuint* framebuffers);

extern void (*orig_evgl_api_glGenFramebuffersOES)(GLsizei n, GLuint* framebuffers);
EAPI void glGenFramebuffersOES_evgl_api_thread_cmd(GLsizei n, GLuint* framebuffers);

extern GLenum (*orig_evgl_api_glCheckFramebufferStatusOES)(GLenum target);
EAPI GLenum glCheckFramebufferStatusOES_evgl_api_thread_cmd(GLenum target);

extern void (*orig_evgl_api_glFramebufferRenderbufferOES)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
EAPI void glFramebufferRenderbufferOES_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

extern void (*orig_evgl_api_glFramebufferTexture2DOES)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
EAPI void glFramebufferTexture2DOES_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

extern void (*orig_evgl_api_glGetFramebufferAttachmentParameterivOES)(GLenum target, GLenum attachment, GLenum pname, GLint* params);
EAPI void glGetFramebufferAttachmentParameterivOES_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLenum pname, GLint* params);

extern void (*orig_evgl_api_glGenerateMipmapOES)(GLenum target);
EAPI void glGenerateMipmapOES_evgl_api_thread_cmd(GLenum target);

extern void (*orig_evgl_api_glCurrentPaletteMatrixOES)(GLuint matrixpaletteindex);
EAPI void glCurrentPaletteMatrixOES_evgl_api_thread_cmd(GLuint matrixpaletteindex);

extern void (*orig_evgl_api_glLoadPaletteFromModelViewMatrixOES)(void);
EAPI void glLoadPaletteFromModelViewMatrixOES_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glMatrixIndexPointerOES)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EAPI void glMatrixIndexPointerOES_evgl_api_thread_cmd(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

extern void (*orig_evgl_api_glWeightPointerOES)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EAPI void glWeightPointerOES_evgl_api_thread_cmd(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

extern GLbitfield (*orig_evgl_api_glQueryMatrixxOES)(GLfixed mantissa[16], GLint exponent[16]);
EAPI GLbitfield glQueryMatrixxOES_evgl_api_thread_cmd(GLfixed mantissa[16], GLint exponent[16]);

extern void (*orig_evgl_api_glDepthRangefOES)(GLclampf zNear, GLclampf zFar);
EAPI void glDepthRangefOES_evgl_api_thread_cmd(GLclampf zNear, GLclampf zFar);

extern void (*orig_evgl_api_glFrustumfOES)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
EAPI void glFrustumfOES_evgl_api_thread_cmd(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

extern void (*orig_evgl_api_glOrthofOES)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
EAPI void glOrthofOES_evgl_api_thread_cmd(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

extern void (*orig_evgl_api_glClipPlanefOES)(GLenum plane, const GLfloat *equation);
EAPI void glClipPlanefOES_evgl_api_thread_cmd(GLenum plane, const GLfloat *equation);

extern void (*orig_evgl_api_glGetClipPlanefOES)(GLenum pname, GLfloat eqn[4]);
EAPI void glGetClipPlanefOES_evgl_api_thread_cmd(GLenum pname, GLfloat eqn[4]);

extern void (*orig_evgl_api_glClearDepthfOES)(GLclampf depth);
EAPI void glClearDepthfOES_evgl_api_thread_cmd(GLclampf depth);

extern void (*orig_evgl_api_glTexGenfOES)(GLenum coord, GLenum pname, GLfloat param);
EAPI void glTexGenfOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLfloat param);

extern void (*orig_evgl_api_glTexGenfvOES)(GLenum coord, GLenum pname, const GLfloat *params);
EAPI void glTexGenfvOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, const GLfloat *params);

extern void (*orig_evgl_api_glTexGeniOES)(GLenum coord, GLenum pname, GLint param);
EAPI void glTexGeniOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLint param);

extern void (*orig_evgl_api_glTexGenivOES)(GLenum coord, GLenum pname, const GLint *params);
EAPI void glTexGenivOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, const GLint *params);

extern void (*orig_evgl_api_glTexGenxOES)(GLenum coord, GLenum pname, GLfixed param);
EAPI void glTexGenxOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLfixed param);

extern void (*orig_evgl_api_glTexGenxvOES)(GLenum coord, GLenum pname, const GLfixed *params);
EAPI void glTexGenxvOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, const GLfixed *params);

extern void (*orig_evgl_api_glGetTexGenfvOES)(GLenum coord, GLenum pname, GLfloat *params);
EAPI void glGetTexGenfvOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLfloat *params);

extern void (*orig_evgl_api_glGetTexGenivOES)(GLenum coord, GLenum pname, GLint *params);
EAPI void glGetTexGenivOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLint *params);

extern void (*orig_evgl_api_glGetTexGenxvOES)(GLenum coord, GLenum pname, GLfixed *params);
EAPI void glGetTexGenxvOES_evgl_api_thread_cmd(GLenum coord, GLenum pname, GLfixed *params);

extern void (*orig_evgl_api_glBindVertexArrayOES)(GLuint array);
EAPI void glBindVertexArrayOES_evgl_api_thread_cmd(GLuint array);

extern void (*orig_evgl_api_glDeleteVertexArraysOES)(GLsizei n, const GLuint *arrays);
EAPI void glDeleteVertexArraysOES_evgl_api_thread_cmd(GLsizei n, const GLuint *arrays);

extern void (*orig_evgl_api_glGenVertexArraysOES)(GLsizei n, GLuint *arrays);
EAPI void glGenVertexArraysOES_evgl_api_thread_cmd(GLsizei n, GLuint *arrays);

extern GLboolean (*orig_evgl_api_glIsVertexArrayOES)(GLuint array);
EAPI GLboolean glIsVertexArrayOES_evgl_api_thread_cmd(GLuint array);

extern void (*orig_evgl_api_glCopyTextureLevelsAPPLE)(GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount);
EAPI void glCopyTextureLevelsAPPLE_evgl_api_thread_cmd(GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount);

extern void (*orig_evgl_api_glRenderbufferStorageMultisampleAPPLE)(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);
EAPI void glRenderbufferStorageMultisampleAPPLE_evgl_api_thread_cmd(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);

extern void (*orig_evgl_api_glResolveMultisampleFramebufferAPPLE)(void);
EAPI void glResolveMultisampleFramebufferAPPLE_evgl_api_thread_cmd(void);

extern GLsync (*orig_evgl_api_glFenceSyncAPPLE)(GLenum condition, GLbitfield flags);
EAPI GLsync glFenceSyncAPPLE_evgl_api_thread_cmd(GLenum condition, GLbitfield flags);

extern GLboolean (*orig_evgl_api_glIsSyncAPPLE)(GLsync sync);
EAPI GLboolean glIsSyncAPPLE_evgl_api_thread_cmd(GLsync sync);

extern void (*orig_evgl_api_glDeleteSyncAPPLE)(GLsync sync);
EAPI void glDeleteSyncAPPLE_evgl_api_thread_cmd(GLsync sync);

extern GLenum (*orig_evgl_api_glClientWaitSyncAPPLE)(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);
EAPI GLenum glClientWaitSyncAPPLE_evgl_api_thread_cmd(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);

extern void (*orig_evgl_api_glWaitSyncAPPLE)(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);
EAPI void glWaitSyncAPPLE_evgl_api_thread_cmd(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);

extern void (*orig_evgl_api_glGetInteger64vAPPLE)(GLenum pname, EvasGLint64 *params);
EAPI void glGetInteger64vAPPLE_evgl_api_thread_cmd(GLenum pname, EvasGLint64 *params);

extern void (*orig_evgl_api_glGetSyncivAPPLE)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
EAPI void glGetSyncivAPPLE_evgl_api_thread_cmd(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);

extern void * (*orig_evgl_api_glMapBufferRangeEXT)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
EAPI void * glMapBufferRangeEXT_evgl_api_thread_cmd(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);

extern void (*orig_evgl_api_glFlushMappedBufferRangeEXT)(GLenum target, GLintptr offset, GLsizeiptr length);
EAPI void glFlushMappedBufferRangeEXT_evgl_api_thread_cmd(GLenum target, GLintptr offset, GLsizeiptr length);

extern void (*orig_evgl_api_glRenderbufferStorageMultisampleEXT)(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);
EAPI void glRenderbufferStorageMultisampleEXT_evgl_api_thread_cmd(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);

extern void (*orig_evgl_api_glFramebufferTexture2DMultisample)(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);
EAPI void glFramebufferTexture2DMultisample_evgl_api_thread_cmd(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);

extern void (*orig_evgl_api_glFramebufferTexture2DMultisampleEXT)(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);
EAPI void glFramebufferTexture2DMultisampleEXT_evgl_api_thread_cmd(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);

extern GLenum (*orig_evgl_api_glGetGraphicsResetStatus)(void);
EAPI GLenum glGetGraphicsResetStatus_evgl_api_thread_cmd(void);

extern GLenum (*orig_evgl_api_glGetGraphicsResetStatusEXT)(void);
EAPI GLenum glGetGraphicsResetStatusEXT_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glReadnPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
EAPI void glReadnPixels_evgl_api_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);

extern void (*orig_evgl_api_glReadnPixelsEXT)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
EAPI void glReadnPixelsEXT_evgl_api_thread_cmd(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);

extern void (*orig_evgl_api_glGetnUniformfv)(GLuint program, GLint location, GLsizei bufSize, float *params);
EAPI void glGetnUniformfv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei bufSize, float *params);

extern void (*orig_evgl_api_glGetnUniformfvEXT)(GLuint program, GLint location, GLsizei bufSize, float *params);
EAPI void glGetnUniformfvEXT_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei bufSize, float *params);

extern void (*orig_evgl_api_glGetnUniformiv)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
EAPI void glGetnUniformiv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei bufSize, GLint *params);

extern void (*orig_evgl_api_glGetnUniformivEXT)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
EAPI void glGetnUniformivEXT_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei bufSize, GLint *params);

extern void (*orig_evgl_api_glTexStorage1DEXT)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
EAPI void glTexStorage1DEXT_evgl_api_thread_cmd(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);

extern void (*orig_evgl_api_glTexStorage2DEXT)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
EAPI void glTexStorage2DEXT_evgl_api_thread_cmd(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);

extern void (*orig_evgl_api_glTexStorage3DEXT)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
EAPI void glTexStorage3DEXT_evgl_api_thread_cmd(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

extern void (*orig_evgl_api_glTextureStorage1DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
EAPI void glTextureStorage1DEXT_evgl_api_thread_cmd(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);

extern void (*orig_evgl_api_glTextureStorage2DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
EAPI void glTextureStorage2DEXT_evgl_api_thread_cmd(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);

extern void (*orig_evgl_api_glTextureStorage3DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
EAPI void glTextureStorage3DEXT_evgl_api_thread_cmd(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

extern void (*orig_evgl_api_glClipPlanefIMG)(GLenum a, const GLfloat * b);
EAPI void glClipPlanefIMG_evgl_api_thread_cmd(GLenum a, const GLfloat * b);

extern void (*orig_evgl_api_glClipPlanexIMG)(GLenum a, const GLfixed * b);
EAPI void glClipPlanexIMG_evgl_api_thread_cmd(GLenum a, const GLfixed * b);

extern void (*orig_evgl_api_glRenderbufferStorageMultisampleIMG)(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);
EAPI void glRenderbufferStorageMultisampleIMG_evgl_api_thread_cmd(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e);

extern void (*orig_evgl_api_glFramebufferTexture2DMultisampleIMG)(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);
EAPI void glFramebufferTexture2DMultisampleIMG_evgl_api_thread_cmd(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLsizei f);

extern void (*orig_evgl_api_glStartTilingQCOM)(GLuint x, GLuint y, GLuint width, GLuint height, GLbitfield preserveMask);
EAPI void glStartTilingQCOM_evgl_api_thread_cmd(GLuint x, GLuint y, GLuint width, GLuint height, GLbitfield preserveMask);

extern void (*orig_evgl_api_glEndTilingQCOM)(GLbitfield preserveMask);
EAPI void glEndTilingQCOM_evgl_api_thread_cmd(GLbitfield preserveMask);

extern void (*orig_evgl_api_glBeginQuery)(GLenum target, GLuint id);
EAPI void glBeginQuery_evgl_api_thread_cmd(GLenum target, GLuint id);

extern void (*orig_evgl_api_glBeginTransformFeedback)(GLenum primitiveMode);
EAPI void glBeginTransformFeedback_evgl_api_thread_cmd(GLenum primitiveMode);

extern void (*orig_evgl_api_glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
EAPI void glBindBufferBase_evgl_api_thread_cmd(GLenum target, GLuint index, GLuint buffer);

extern void (*orig_evgl_api_glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
EAPI void glBindBufferRange_evgl_api_thread_cmd(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);

extern void (*orig_evgl_api_glBindSampler)(GLuint unit, GLuint sampler);
EAPI void glBindSampler_evgl_api_thread_cmd(GLuint unit, GLuint sampler);

extern void (*orig_evgl_api_glBindTransformFeedback)(GLenum target, GLuint id);
EAPI void glBindTransformFeedback_evgl_api_thread_cmd(GLenum target, GLuint id);

extern void (*orig_evgl_api_glBindVertexArray)(GLuint array);
EAPI void glBindVertexArray_evgl_api_thread_cmd(GLuint array);

extern void (*orig_evgl_api_glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
EAPI void glBlitFramebuffer_evgl_api_thread_cmd(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

extern void (*orig_evgl_api_glClearBufferfi)(GLenum buffer, GLint drawBuffer, GLfloat depth, GLint stencil);
EAPI void glClearBufferfi_evgl_api_thread_cmd(GLenum buffer, GLint drawBuffer, GLfloat depth, GLint stencil);

extern void (*orig_evgl_api_glClearBufferfv)(GLenum buffer, GLint drawBuffer, const GLfloat * value);
EAPI void glClearBufferfv_evgl_api_thread_cmd(GLenum buffer, GLint drawBuffer, const GLfloat * value);

extern void (*orig_evgl_api_glClearBufferiv)(GLenum buffer, GLint drawBuffer, const GLint * value);
EAPI void glClearBufferiv_evgl_api_thread_cmd(GLenum buffer, GLint drawBuffer, const GLint * value);

extern void (*orig_evgl_api_glClearBufferuiv)(GLenum buffer, GLint drawBuffer, const GLuint * value);
EAPI void glClearBufferuiv_evgl_api_thread_cmd(GLenum buffer, GLint drawBuffer, const GLuint * value);

extern GLenum (*orig_evgl_api_glClientWaitSync)(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);
EAPI GLenum glClientWaitSync_evgl_api_thread_cmd(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);

extern void (*orig_evgl_api_glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);
EAPI void glCompressedTexImage3D_evgl_api_thread_cmd(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);

extern void (*orig_evgl_api_glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);
EAPI void glCompressedTexSubImage3D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);

extern void (*orig_evgl_api_glCopyBufferSubData)(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);
EAPI void glCopyBufferSubData_evgl_api_thread_cmd(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);

extern void (*orig_evgl_api_glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
EAPI void glCopyTexSubImage3D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

extern void (*orig_evgl_api_glDeleteQueries)(GLsizei n, const GLuint * ids);
EAPI void glDeleteQueries_evgl_api_thread_cmd(GLsizei n, const GLuint * ids);

extern void (*orig_evgl_api_glDeleteSamplers)(GLsizei n, const GLuint * samplers);
EAPI void glDeleteSamplers_evgl_api_thread_cmd(GLsizei n, const GLuint * samplers);

extern void (*orig_evgl_api_glDeleteSync)(GLsync sync);
EAPI void glDeleteSync_evgl_api_thread_cmd(GLsync sync);

extern void (*orig_evgl_api_glDeleteTransformFeedbacks)(GLsizei n, const GLuint *ids);
EAPI void glDeleteTransformFeedbacks_evgl_api_thread_cmd(GLsizei n, const GLuint *ids);

extern void (*orig_evgl_api_glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
EAPI void glDeleteVertexArrays_evgl_api_thread_cmd(GLsizei n, const GLuint *arrays);

extern void (*orig_evgl_api_glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
EAPI void glDrawArraysInstanced_evgl_api_thread_cmd(GLenum mode, GLint first, GLsizei count, GLsizei primcount);

extern void (*orig_evgl_api_glDrawBuffers)(GLsizei n, const GLenum *bufs);
EAPI void glDrawBuffers_evgl_api_thread_cmd(GLsizei n, const GLenum *bufs);

extern void (*orig_evgl_api_glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount);
EAPI void glDrawElementsInstanced_evgl_api_thread_cmd(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount);

extern void (*orig_evgl_api_glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices);
EAPI void glDrawRangeElements_evgl_api_thread_cmd(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices);

extern void (*orig_evgl_api_glEndQuery)(GLenum target);
EAPI void glEndQuery_evgl_api_thread_cmd(GLenum target);

extern void (*orig_evgl_api_glEndTransformFeedback)(void);
EAPI void glEndTransformFeedback_evgl_api_thread_cmd(void);

extern GLsync (*orig_evgl_api_glFenceSync)(GLenum condition, GLbitfield flags);
EAPI GLsync glFenceSync_evgl_api_thread_cmd(GLenum condition, GLbitfield flags);

extern GLsync (*orig_evgl_api_glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
EAPI GLsync glFlushMappedBufferRange_evgl_api_thread_cmd(GLenum target, GLintptr offset, GLsizeiptr length);

extern void (*orig_evgl_api_glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
EAPI void glFramebufferTextureLayer_evgl_api_thread_cmd(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

extern void (*orig_evgl_api_glGenQueries)(GLsizei n, GLuint * ids);
EAPI void glGenQueries_evgl_api_thread_cmd(GLsizei n, GLuint * ids);

extern void (*orig_evgl_api_glGenSamplers)(GLsizei n, GLuint *samplers);
EAPI void glGenSamplers_evgl_api_thread_cmd(GLsizei n, GLuint *samplers);

extern void (*orig_evgl_api_glGenTransformFeedbacks)(GLsizei n, GLuint *ids);
EAPI void glGenTransformFeedbacks_evgl_api_thread_cmd(GLsizei n, GLuint *ids);

extern void (*orig_evgl_api_glGenVertexArrays)(GLsizei n, GLuint *arrays);
EAPI void glGenVertexArrays_evgl_api_thread_cmd(GLsizei n, GLuint *arrays);

extern void (*orig_evgl_api_glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
EAPI void glGetActiveUniformBlockiv_evgl_api_thread_cmd(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);

extern void (*orig_evgl_api_glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
EAPI void glGetActiveUniformBlockName_evgl_api_thread_cmd(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);

extern void (*orig_evgl_api_glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
EAPI void glGetActiveUniformsiv_evgl_api_thread_cmd(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);

extern void (*orig_evgl_api_glGetBufferParameteri64v)(GLenum target, GLenum value, EvasGLint64 * data);
EAPI void glGetBufferParameteri64v_evgl_api_thread_cmd(GLenum target, GLenum value, EvasGLint64 * data);

extern void (*orig_evgl_api_glGetBufferPointerv)(GLenum target, GLenum pname, GLvoid ** params);
EAPI void glGetBufferPointerv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLvoid ** params);

extern GLint (*orig_evgl_api_glGetFragDataLocation)(GLuint program, const char * name);
EAPI GLint glGetFragDataLocation_evgl_api_thread_cmd(GLuint program, const char * name);

extern void (*orig_evgl_api_glGetInteger64i_v)(GLenum target, GLuint index, EvasGLint64 * data);
EAPI void glGetInteger64i_v_evgl_api_thread_cmd(GLenum target, GLuint index, EvasGLint64 * data);

extern void (*orig_evgl_api_glGetInteger64v)(GLenum pname, EvasGLint64 * data);
EAPI void glGetInteger64v_evgl_api_thread_cmd(GLenum pname, EvasGLint64 * data);

extern void (*orig_evgl_api_glGetIntegeri_v)(GLenum target, GLuint index, GLint * data);
EAPI void glGetIntegeri_v_evgl_api_thread_cmd(GLenum target, GLuint index, GLint * data);

extern void (*orig_evgl_api_glGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
EAPI void glGetInternalformativ_evgl_api_thread_cmd(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);

extern void (*orig_evgl_api_glGetProgramBinary)(GLuint program, GLsizei bufsize, GLsizei *length, GLenum *binaryFormat, void *binary);
EAPI void glGetProgramBinary_evgl_api_thread_cmd(GLuint program, GLsizei bufsize, GLsizei *length, GLenum *binaryFormat, void *binary);

extern void (*orig_evgl_api_glGetQueryiv)(GLenum target, GLenum pname, GLint * params);
EAPI void glGetQueryiv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint * params);

extern void (*orig_evgl_api_glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint * params);
EAPI void glGetQueryObjectuiv_evgl_api_thread_cmd(GLuint id, GLenum pname, GLuint * params);

extern void (*orig_evgl_api_glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat * params);
EAPI void glGetSamplerParameterfv_evgl_api_thread_cmd(GLuint sampler, GLenum pname, GLfloat * params);

extern void (*orig_evgl_api_glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint * params);
EAPI void glGetSamplerParameteriv_evgl_api_thread_cmd(GLuint sampler, GLenum pname, GLint * params);

extern const GLubyte * (*orig_evgl_api_glGetStringi)(GLenum name, GLuint index);
EAPI const GLubyte * glGetStringi_evgl_api_thread_cmd(GLenum name, GLuint index);

extern void (*orig_evgl_api_glGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
EAPI void glGetSynciv_evgl_api_thread_cmd(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);

extern void (*orig_evgl_api_glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, char * name);
EAPI void glGetTransformFeedbackVarying_evgl_api_thread_cmd(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, char * name);

extern GLuint (*orig_evgl_api_glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);
EAPI GLuint glGetUniformBlockIndex_evgl_api_thread_cmd(GLuint program, const GLchar *uniformBlockName);

extern void (*orig_evgl_api_glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
EAPI void glGetUniformIndices_evgl_api_thread_cmd(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);

extern void (*orig_evgl_api_glGetUniformuiv)(GLuint program, GLint location, GLuint* params);
EAPI void glGetUniformuiv_evgl_api_thread_cmd(GLuint program, GLint location, GLuint* params);

extern void (*orig_evgl_api_glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);
EAPI void glGetVertexAttribIiv_evgl_api_thread_cmd(GLuint index, GLenum pname, GLint *params);

extern void (*orig_evgl_api_glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);
EAPI void glGetVertexAttribIuiv_evgl_api_thread_cmd(GLuint index, GLenum pname, GLuint *params);

extern void (*orig_evgl_api_glInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments);
EAPI void glInvalidateFramebuffer_evgl_api_thread_cmd(GLenum target, GLsizei numAttachments, const GLenum *attachments);

extern void (*orig_evgl_api_glInvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
EAPI void glInvalidateSubFramebuffer_evgl_api_thread_cmd(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);

extern GLboolean (*orig_evgl_api_glIsQuery)(GLuint id);
EAPI GLboolean glIsQuery_evgl_api_thread_cmd(GLuint id);

extern GLboolean (*orig_evgl_api_glIsSampler)(GLuint id);
EAPI GLboolean glIsSampler_evgl_api_thread_cmd(GLuint id);

extern GLboolean (*orig_evgl_api_glIsSync)(GLsync sync);
EAPI GLboolean glIsSync_evgl_api_thread_cmd(GLsync sync);

extern GLboolean (*orig_evgl_api_glIsTransformFeedback)(GLuint id);
EAPI GLboolean glIsTransformFeedback_evgl_api_thread_cmd(GLuint id);

extern GLboolean (*orig_evgl_api_glIsVertexArray)(GLuint array);
EAPI GLboolean glIsVertexArray_evgl_api_thread_cmd(GLuint array);

extern void * (*orig_evgl_api_glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
EAPI void * glMapBufferRange_evgl_api_thread_cmd(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);

extern void (*orig_evgl_api_glPauseTransformFeedback)(void);
EAPI void glPauseTransformFeedback_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glProgramBinary)(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
EAPI void glProgramBinary_evgl_api_thread_cmd(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);

extern void (*orig_evgl_api_glProgramParameteri)(GLuint program, GLenum pname, GLint value);
EAPI void glProgramParameteri_evgl_api_thread_cmd(GLuint program, GLenum pname, GLint value);

extern void (*orig_evgl_api_glReadBuffer)(GLenum src);
EAPI void glReadBuffer_evgl_api_thread_cmd(GLenum src);

extern void (*orig_evgl_api_glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
EAPI void glRenderbufferStorageMultisample_evgl_api_thread_cmd(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

extern void (*orig_evgl_api_glResumeTransformFeedback)(void);
EAPI void glResumeTransformFeedback_evgl_api_thread_cmd(void);

extern void (*orig_evgl_api_glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
EAPI void glSamplerParameterf_evgl_api_thread_cmd(GLuint sampler, GLenum pname, GLfloat param);

extern void (*orig_evgl_api_glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat * params);
EAPI void glSamplerParameterfv_evgl_api_thread_cmd(GLuint sampler, GLenum pname, const GLfloat * params);

extern void (*orig_evgl_api_glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
EAPI void glSamplerParameteri_evgl_api_thread_cmd(GLuint sampler, GLenum pname, GLint param);

extern void (*orig_evgl_api_glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint * params);
EAPI void glSamplerParameteriv_evgl_api_thread_cmd(GLuint sampler, GLenum pname, const GLint * params);

extern void (*orig_evgl_api_glTexImage3D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data);
EAPI void glTexImage3D_evgl_api_thread_cmd(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data);

extern void (*orig_evgl_api_glTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
EAPI void glTexStorage2D_evgl_api_thread_cmd(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);

extern void (*orig_evgl_api_glTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
EAPI void glTexStorage3D_evgl_api_thread_cmd(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

extern void (*orig_evgl_api_glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data);
EAPI void glTexSubImage3D_evgl_api_thread_cmd(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data);

extern void (*orig_evgl_api_glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar *const* varyings, GLenum bufferMode);
EAPI void glTransformFeedbackVaryings_evgl_api_thread_cmd(GLuint program, GLsizei count, const GLchar *const* varyings, GLenum bufferMode);

extern void (*orig_evgl_api_glUniform1ui)(GLint location, GLuint v0);
EAPI void glUniform1ui_evgl_api_thread_cmd(GLint location, GLuint v0);

extern void (*orig_evgl_api_glUniform1uiv)(GLint location, GLsizei count, const GLuint *value);
EAPI void glUniform1uiv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLuint *value);

extern void (*orig_evgl_api_glUniform2ui)(GLint location, GLuint v0, GLuint v1);
EAPI void glUniform2ui_evgl_api_thread_cmd(GLint location, GLuint v0, GLuint v1);

extern void (*orig_evgl_api_glUniform2uiv)(GLint location, GLsizei count, const GLuint *value);
EAPI void glUniform2uiv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLuint *value);

extern void (*orig_evgl_api_glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
EAPI void glUniform3ui_evgl_api_thread_cmd(GLint location, GLuint v0, GLuint v1, GLuint v2);

extern void (*orig_evgl_api_glUniform3uiv)(GLint location, GLsizei count, const GLuint *value);
EAPI void glUniform3uiv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLuint *value);

extern void (*orig_evgl_api_glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
EAPI void glUniform4ui_evgl_api_thread_cmd(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

extern void (*orig_evgl_api_glUniform4uiv)(GLint location, GLsizei count, const GLuint *value);
EAPI void glUniform4uiv_evgl_api_thread_cmd(GLint location, GLsizei count, const GLuint *value);

extern void (*orig_evgl_api_glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
EAPI void glUniformBlockBinding_evgl_api_thread_cmd(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

extern void (*orig_evgl_api_glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glUniformMatrix2x3fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glUniformMatrix3x2fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glUniformMatrix2x4fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glUniformMatrix4x2fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glUniformMatrix3x4fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glUniformMatrix4x3fv_evgl_api_thread_cmd(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern GLboolean (*orig_evgl_api_glUnmapBuffer)(GLenum target);
EAPI GLboolean glUnmapBuffer_evgl_api_thread_cmd(GLenum target);

extern void (*orig_evgl_api_glVertexAttribDivisor)(GLuint index, GLuint divisor);
EAPI void glVertexAttribDivisor_evgl_api_thread_cmd(GLuint index, GLuint divisor);

extern void (*orig_evgl_api_glVertexAttribI4i)(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3);
EAPI void glVertexAttribI4i_evgl_api_thread_cmd(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3);

extern void (*orig_evgl_api_glVertexAttribI4iv)(GLuint index, const GLint *v);
EAPI void glVertexAttribI4iv_evgl_api_thread_cmd(GLuint index, const GLint *v);

extern void (*orig_evgl_api_glVertexAttribI4ui)(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
EAPI void glVertexAttribI4ui_evgl_api_thread_cmd(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

extern void (*orig_evgl_api_glVertexAttribI4uiv)(GLuint index, const GLuint *v);
EAPI void glVertexAttribI4uiv_evgl_api_thread_cmd(GLuint index, const GLuint *v);

extern void (*orig_evgl_api_glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EAPI void glVertexAttribIPointer_evgl_api_thread_cmd(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

extern void (*orig_evgl_api_glWaitSync)(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);
EAPI void glWaitSync_evgl_api_thread_cmd(GLsync sync, GLbitfield flags, EvasGLuint64 timeout);

extern void (*orig_evgl_api_glDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
EAPI void glDispatchCompute_evgl_api_thread_cmd(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);

extern void (*orig_evgl_api_glDispatchComputeIndirect)(GLintptr indirect);
EAPI void glDispatchComputeIndirect_evgl_api_thread_cmd(GLintptr indirect);

extern void (*orig_evgl_api_glDrawArraysIndirect)(GLenum mode, const void *indirect);
EAPI void glDrawArraysIndirect_evgl_api_thread_cmd(GLenum mode, const void *indirect);

extern void (*orig_evgl_api_glDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect);
EAPI void glDrawElementsIndirect_evgl_api_thread_cmd(GLenum mode, GLenum type, const void *indirect);

extern void (*orig_evgl_api_glFramebufferParameteri)(GLenum target, GLenum pname, GLint param);
EAPI void glFramebufferParameteri_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint param);

extern void (*orig_evgl_api_glGetFramebufferParameteriv)(GLenum target, GLenum pname, GLint *params);
EAPI void glGetFramebufferParameteriv_evgl_api_thread_cmd(GLenum target, GLenum pname, GLint *params);

extern void (*orig_evgl_api_glGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint *params);
EAPI void glGetProgramInterfaceiv_evgl_api_thread_cmd(GLuint program, GLenum programInterface, GLenum pname, GLint *params);

extern GLuint (*orig_evgl_api_glGetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar *name);
EAPI GLuint glGetProgramResourceIndex_evgl_api_thread_cmd(GLuint program, GLenum programInterface, const GLchar *name);

extern void (*orig_evgl_api_glGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
EAPI void glGetProgramResourceName_evgl_api_thread_cmd(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);

extern void (*orig_evgl_api_glGetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
EAPI void glGetProgramResourceiv_evgl_api_thread_cmd(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);

extern GLint (*orig_evgl_api_glGetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar *name);
EAPI GLint glGetProgramResourceLocation_evgl_api_thread_cmd(GLuint program, GLenum programInterface, const GLchar *name);

extern void (*orig_evgl_api_glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
EAPI void glUseProgramStages_evgl_api_thread_cmd(GLuint pipeline, GLbitfield stages, GLuint program);

extern void (*orig_evgl_api_glActiveShaderProgram)(GLuint pipeline, GLuint program);
EAPI void glActiveShaderProgram_evgl_api_thread_cmd(GLuint pipeline, GLuint program);

extern GLuint (*orig_evgl_api_glCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar *const*strings);
EAPI GLuint glCreateShaderProgramv_evgl_api_thread_cmd(GLenum type, GLsizei count, const GLchar *const*strings);

extern void (*orig_evgl_api_glBindProgramPipeline)(GLuint pipeline);
EAPI void glBindProgramPipeline_evgl_api_thread_cmd(GLuint pipeline);

extern void (*orig_evgl_api_glDeleteProgramPipelines)(GLsizei n, const GLuint *pipelines);
EAPI void glDeleteProgramPipelines_evgl_api_thread_cmd(GLsizei n, const GLuint *pipelines);

extern void (*orig_evgl_api_glGenProgramPipelines)(GLsizei n, GLuint *pipelines);
EAPI void glGenProgramPipelines_evgl_api_thread_cmd(GLsizei n, GLuint *pipelines);

extern GLboolean (*orig_evgl_api_glIsProgramPipeline)(GLuint pipeline);
EAPI GLboolean glIsProgramPipeline_evgl_api_thread_cmd(GLuint pipeline);

extern void (*orig_evgl_api_glGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint *params);
EAPI void glGetProgramPipelineiv_evgl_api_thread_cmd(GLuint pipeline, GLenum pname, GLint *params);

extern void (*orig_evgl_api_glProgramUniform1i)(GLuint program, GLint location, GLint v0);
EAPI void glProgramUniform1i_evgl_api_thread_cmd(GLuint program, GLint location, GLint v0);

extern void (*orig_evgl_api_glProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);
EAPI void glProgramUniform2i_evgl_api_thread_cmd(GLuint program, GLint location, GLint v0, GLint v1);

extern void (*orig_evgl_api_glProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
EAPI void glProgramUniform3i_evgl_api_thread_cmd(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);

extern void (*orig_evgl_api_glProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
EAPI void glProgramUniform4i_evgl_api_thread_cmd(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

extern void (*orig_evgl_api_glProgramUniform1ui)(GLuint program, GLint location, GLuint v0);
EAPI void glProgramUniform1ui_evgl_api_thread_cmd(GLuint program, GLint location, GLuint v0);

extern void (*orig_evgl_api_glProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);
EAPI void glProgramUniform2ui_evgl_api_thread_cmd(GLuint program, GLint location, GLuint v0, GLuint v1);

extern void (*orig_evgl_api_glProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
EAPI void glProgramUniform3ui_evgl_api_thread_cmd(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);

extern void (*orig_evgl_api_glProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
EAPI void glProgramUniform4ui_evgl_api_thread_cmd(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

extern void (*orig_evgl_api_glProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
EAPI void glProgramUniform1f_evgl_api_thread_cmd(GLuint program, GLint location, GLfloat v0);

extern void (*orig_evgl_api_glProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
EAPI void glProgramUniform2f_evgl_api_thread_cmd(GLuint program, GLint location, GLfloat v0, GLfloat v1);

extern void (*orig_evgl_api_glProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
EAPI void glProgramUniform3f_evgl_api_thread_cmd(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);

extern void (*orig_evgl_api_glProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
EAPI void glProgramUniform4f_evgl_api_thread_cmd(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

extern void (*orig_evgl_api_glProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
EAPI void glProgramUniform1iv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLint *value);

extern void (*orig_evgl_api_glProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
EAPI void glProgramUniform2iv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLint *value);

extern void (*orig_evgl_api_glProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
EAPI void glProgramUniform3iv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLint *value);

extern void (*orig_evgl_api_glProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
EAPI void glProgramUniform4iv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLint *value);

extern void (*orig_evgl_api_glProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
EAPI void glProgramUniform1uiv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLuint *value);

extern void (*orig_evgl_api_glProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
EAPI void glProgramUniform2uiv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLuint *value);

extern void (*orig_evgl_api_glProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
EAPI void glProgramUniform3uiv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLuint *value);

extern void (*orig_evgl_api_glProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
EAPI void glProgramUniform4uiv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLuint *value);

extern void (*orig_evgl_api_glProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
EAPI void glProgramUniform1fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
EAPI void glProgramUniform2fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
EAPI void glProgramUniform3fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
EAPI void glProgramUniform4fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glProgramUniformMatrix2fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glProgramUniformMatrix3fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glProgramUniformMatrix4fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glProgramUniformMatrix2x3fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glProgramUniformMatrix3x2fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glProgramUniformMatrix2x4fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glProgramUniformMatrix4x2fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glProgramUniformMatrix3x4fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
EAPI void glProgramUniformMatrix4x3fv_evgl_api_thread_cmd(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern void (*orig_evgl_api_glValidateProgramPipeline)(GLuint pipeline);
EAPI void glValidateProgramPipeline_evgl_api_thread_cmd(GLuint pipeline);

extern void (*orig_evgl_api_glGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
EAPI void glGetProgramPipelineInfoLog_evgl_api_thread_cmd(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

extern void (*orig_evgl_api_glBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
EAPI void glBindImageTexture_evgl_api_thread_cmd(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);

extern void (*orig_evgl_api_glGetBooleani_v)(GLenum target, GLuint index, GLboolean *data);
EAPI void glGetBooleani_v_evgl_api_thread_cmd(GLenum target, GLuint index, GLboolean *data);

extern void (*orig_evgl_api_glMemoryBarrier)(GLbitfield barriers);
EAPI void glMemoryBarrier_evgl_api_thread_cmd(GLbitfield barriers);

extern void (*orig_evgl_api_glMemoryBarrierByRegion)(GLbitfield barriers);
EAPI void glMemoryBarrierByRegion_evgl_api_thread_cmd(GLbitfield barriers);

extern void (*orig_evgl_api_glTexStorage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
EAPI void glTexStorage2DMultisample_evgl_api_thread_cmd(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);

extern void (*orig_evgl_api_glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val);
EAPI void glGetMultisamplefv_evgl_api_thread_cmd(GLenum pname, GLuint index, GLfloat *val);

extern void (*orig_evgl_api_glSampleMaski)(GLuint maskNumber, GLbitfield mask);
EAPI void glSampleMaski_evgl_api_thread_cmd(GLuint maskNumber, GLbitfield mask);

extern void (*orig_evgl_api_glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
EAPI void glGetTexLevelParameteriv_evgl_api_thread_cmd(GLenum target, GLint level, GLenum pname, GLint *params);

extern void (*orig_evgl_api_glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
EAPI void glGetTexLevelParameterfv_evgl_api_thread_cmd(GLenum target, GLint level, GLenum pname, GLfloat *params);

extern void (*orig_evgl_api_glBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
EAPI void glBindVertexBuffer_evgl_api_thread_cmd(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);

extern void (*orig_evgl_api_glVertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
EAPI void glVertexAttribFormat_evgl_api_thread_cmd(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);

extern void (*orig_evgl_api_glVertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
EAPI void glVertexAttribIFormat_evgl_api_thread_cmd(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);

extern void (*orig_evgl_api_glVertexAttribBinding)(GLuint attribindex, GLuint bindingindex);
EAPI void glVertexAttribBinding_evgl_api_thread_cmd(GLuint attribindex, GLuint bindingindex);

extern void (*orig_evgl_api_glVertexBindingDivisor)(GLuint bindingindex, GLuint divisor);
EAPI void glVertexBindingDivisor_evgl_api_thread_cmd(GLuint bindingindex, GLuint divisor);

extern void (*orig_evgl_api_glEGLImageTargetTexture2DOES)(GLenum target, void *image);
EAPI void glEGLImageTargetTexture2DOES_evgl_api_thread_cmd(GLenum target, void *image);

extern void (*orig_evgl_api_glEGLImageTargetRenderbufferStorageOES)(GLenum target, void *image);
EAPI void glEGLImageTargetRenderbufferStorageOES_evgl_api_thread_cmd(GLenum target, void *image);

extern void (*orig_evgl_api__evgl_glDiscardFramebufferEXT)(GLenum target, GLsizei numAttachments, const GLenum* attachments);
EAPI void _evgl_glDiscardFramebufferEXT_evgl_api_thread_cmd(GLenum target, GLsizei numAttachments, const GLenum* attachments);

extern void (*orig_evgl_api__evgl_glEvasGLImageTargetTexture2D)(GLenum target, EvasGLImage image);
EAPI void _evgl_glEvasGLImageTargetTexture2D_evgl_api_thread_cmd(GLenum target, EvasGLImage image);

extern void (*orig_evgl_api__evgl_glEvasGLImageTargetRenderbufferStorage)(GLenum target, EvasGLImage image);
EAPI void _evgl_glEvasGLImageTargetRenderbufferStorage_evgl_api_thread_cmd(GLenum target, EvasGLImage image);

extern EvasGLImage (*orig_evgl_api__evgl_evasglCreateImage)(int target, void* buffer, const int *attrib_list);
EAPI EvasGLImage _evgl_evasglCreateImage_evgl_api_thread_cmd(int target, void* buffer, const int *attrib_list);

extern void (*orig_evgl_api__evgl_evasglDestroyImage)(EvasGLImage image);
EAPI void _evgl_evasglDestroyImage_evgl_api_thread_cmd(EvasGLImage image);

extern EvasGLImage (*orig_evgl_api__evgl_evasglCreateImageForContext)(Evas_GL *evas_gl, Evas_GL_Context *ctx, int target, void* buffer, const int *attrib_list);
EAPI EvasGLImage _evgl_evasglCreateImageForContext_evgl_api_thread_cmd(Evas_GL *evas_gl, Evas_GL_Context *ctx, int target, void* buffer, const int *attrib_list);

extern EvasGLSync (*orig_evgl_api__evgl_evasglCreateSync)(Evas_GL *evas_gl, unsigned int type, const int *attrib_list);
EAPI EvasGLSync _evgl_evasglCreateSync_evgl_api_thread_cmd(Evas_GL *evas_gl, unsigned int type, const int *attrib_list);

extern Eina_Bool (*orig_evgl_api__evgl_evasglDestroySync)(Evas_GL *evas_gl, EvasGLSync sync);
EAPI Eina_Bool _evgl_evasglDestroySync_evgl_api_thread_cmd(Evas_GL *evas_gl, EvasGLSync sync);

extern int (*orig_evgl_api__evgl_evasglClientWaitSync)(Evas_GL *evas_gl, EvasGLSync sync);
EAPI int _evgl_evasglClientWaitSync_evgl_api_thread_cmd(Evas_GL *evas_gl, EvasGLSync sync);

extern Eina_Bool (*orig_evgl_api__evgl_evasglGetSyncAttrib)(Evas_GL *evas_gl, EvasGLSync sync, int attribute, int *value);
EAPI Eina_Bool _evgl_evasglGetSyncAttrib_evgl_api_thread_cmd(Evas_GL *evas_gl, EvasGLSync sync, int attribute, int *value);

extern Eina_Bool (*orig_evgl_api__evgl_evasglSignalSync)(Evas_GL *evas_gl, EvasGLSync sync, unsigned mode);
EAPI Eina_Bool _evgl_evasglSignalSync_evgl_api_thread_cmd(Evas_GL *evas_gl, EvasGLSync sync, unsigned mode);

extern int (*orig_evgl_api__evgl_evasglWaitSync)(Evas_GL *evas_gl, EvasGLSync sync, int flags);
EAPI int _evgl_evasglWaitSync_evgl_api_thread_cmd(Evas_GL *evas_gl, EvasGLSync sync, int flags);

extern Eina_Bool (*orig_evgl_api__evgl_evasglBindWaylandDisplay)(Evas_GL *evas_gl, void *wl_display);
EAPI Eina_Bool _evgl_evasglBindWaylandDisplay_evgl_api_thread_cmd(Evas_GL *evas_gl, void *wl_display);

extern Eina_Bool (*orig_evgl_api__evgl_evasglUnbindWaylandDisplay)(Evas_GL *evas_gl, void *wl_display);
EAPI Eina_Bool _evgl_evasglUnbindWaylandDisplay_evgl_api_thread_cmd(Evas_GL *evas_gl, void *wl_display);

extern Eina_Bool (*orig_evgl_api__evgl_evasglQueryWaylandBuffer)(Evas_GL *evas_gl, void *buffer, int attribute, int *value);
EAPI Eina_Bool _evgl_evasglQueryWaylandBuffer_evgl_api_thread_cmd(Evas_GL *evas_gl, void *buffer, int attribute, int *value);
