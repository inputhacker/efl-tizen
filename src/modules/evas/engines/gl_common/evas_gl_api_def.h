_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glBindFramebuffer, GLenum, target, GLuint, framebuffer )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glClearDepthf, GLclampf, depth )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glDepthRangef, GLclampf, zNear, GLclampf, zFar )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glGetShaderPrecisionFormat, GLenum, shadertype, GLenum, precisiontype, GLint*, range, GLint*, precision )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glShaderBinary, GLsizei, n, const GLuint*, shaders, GLenum, binaryformat, const void*, binary, GLsizei, length )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glReleaseShaderCompiler)
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glClearColor, GLclampf, red, GLclampf, green, GLclampf, blue, GLclampf, alpha )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glClear, GLbitfield, mask )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glEnable, GLenum, cap )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glDisable, GLenum, cap )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glGetIntegerv, GLenum, pname, GLint*, params )
_EVASGL_FUNCTION_PRIVATE_BEGIN(const GLubyte *, glGetString, GLenum, name )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glReadPixels, GLint, x, GLint, y, GLsizei, width, GLsizei, height, GLenum, format, GLenum, type, void*, pixels )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glScissor, GLint, xoffset, GLint, y, GLsizei, width, GLsizei, height )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glViewport, GLint, x, GLint, y, GLsizei, width, GLsizei, height )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glDeleteFramebuffers, GLsizei, n, const GLuint*, framebuffers )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glFramebufferRenderbuffer, GLenum, target, GLenum, attachment, GLenum, renderbuffertarget, GLuint, renderbuffer )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glFramebufferTexture2D, GLenum, target, GLenum, attachment, GLenum, textarget, GLuint, texture, GLint, level )
_EVASGL_FUNCTION_PRIVATE_BEGIN(GLenum, glGetError )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glGetFloatv, GLenum, pname, GLfloat*, params )
_EVASGL_FUNCTION_PRIVATE_BEGIN_VOID( glGetFramebufferAttachmentParameteriv, GLenum, target, GLenum, attachment, GLenum, pname, GLint*, params )

_EVASGL_FUNCTION_BEGIN_VOID( glActiveTexture, GLenum, texture )
_EVASGL_FUNCTION_BEGIN_VOID( glAttachShader, GLuint, program, GLuint, shader )
_EVASGL_FUNCTION_BEGIN_VOID( glBindAttribLocation, GLuint, program, GLuint, index, const char*, name )
_EVASGL_FUNCTION_BEGIN_VOID( glBindBuffer, GLenum, target, GLuint, buffer )
_EVASGL_FUNCTION_BEGIN_VOID( glBindRenderbuffer, GLenum, target, GLuint, renderbuffer )
_EVASGL_FUNCTION_BEGIN_VOID( glBindTexture, GLenum, target, GLuint, texture )
_EVASGL_FUNCTION_BEGIN_VOID( glBlendColor, GLclampf, red, GLclampf, green, GLclampf, blue, GLclampf, alpha )
_EVASGL_FUNCTION_BEGIN_VOID( glBlendEquation,  GLenum, mode  )
_EVASGL_FUNCTION_BEGIN_VOID( glBlendEquationSeparate, GLenum, modeRGB, GLenum, modeAlpha )
_EVASGL_FUNCTION_BEGIN_VOID( glBlendFunc, GLenum, sfactor, GLenum, dfactor )
_EVASGL_FUNCTION_BEGIN_VOID( glBlendFuncSeparate, GLenum, srcRGB, GLenum, dstRGB, GLenum, srcAlpha, GLenum, dstAlpha )
_EVASGL_FUNCTION_BEGIN_VOID( glBufferData, GLenum, target, GLsizeiptr, size, const void*, data, GLenum, usage )
_EVASGL_FUNCTION_BEGIN_VOID( glBufferSubData, GLenum, target, GLintptr, offset, GLsizeiptr, size, const void*, data )
_EVASGL_FUNCTION_BEGIN(GLenum, glCheckFramebufferStatus, GLenum, target )
_EVASGL_FUNCTION_BEGIN_VOID( glClearStencil, GLint, s )
_EVASGL_FUNCTION_BEGIN_VOID( glColorMask, GLboolean, red, GLboolean, green, GLboolean, blue, GLboolean, alpha )
_EVASGL_FUNCTION_BEGIN_VOID( glCompileShader, GLuint, shader )
_EVASGL_FUNCTION_BEGIN_VOID( glCompressedTexImage2D, GLenum, target, GLint, level, GLenum, internalformat, GLsizei, width, GLsizei, height, GLint, border, GLsizei, imageSize, const void*, data )
_EVASGL_FUNCTION_BEGIN_VOID( glCompressedTexSubImage2D, GLenum, target, GLint, level, GLint, xoffset, GLint, yoffset, GLsizei, width, GLsizei, height, GLenum, format, GLsizei, imageSize, const void*, data )
_EVASGL_FUNCTION_BEGIN_VOID( glCopyTexImage2D, GLenum, target, GLint, level, GLenum, internalformat, GLint, x, GLint, y, GLsizei, width, GLsizei, height, GLint, border )
_EVASGL_FUNCTION_BEGIN_VOID( glCopyTexSubImage2D, GLenum, target, GLint, level, GLint, xoffset, GLint, yoffset, GLint, x, GLint, y, GLsizei, width, GLsizei, height )
_EVASGL_FUNCTION_BEGIN(GLuint, glCreateProgram )
_EVASGL_FUNCTION_BEGIN(GLuint, glCreateShader, GLenum, type )
_EVASGL_FUNCTION_BEGIN_VOID( glCullFace, GLenum, mode )
_EVASGL_FUNCTION_BEGIN_VOID( glDeleteBuffers, GLsizei, n, const GLuint*, buffers )
_EVASGL_FUNCTION_BEGIN_VOID( glDeleteProgram, GLuint, program )
_EVASGL_FUNCTION_BEGIN_VOID( glDeleteRenderbuffers, GLsizei, n, const GLuint*, renderbuffers )
_EVASGL_FUNCTION_BEGIN_VOID( glDeleteShader, GLuint, shader )
_EVASGL_FUNCTION_BEGIN_VOID( glDeleteTextures, GLsizei, n, const GLuint*, textures )
_EVASGL_FUNCTION_BEGIN_VOID( glDepthFunc, GLenum, func )
_EVASGL_FUNCTION_BEGIN_VOID( glDepthMask, GLboolean, flag )
_EVASGL_FUNCTION_BEGIN_VOID( glDetachShader, GLuint, program, GLuint, shader )
_EVASGL_FUNCTION_BEGIN_VOID( glDisableVertexAttribArray, GLuint, index )
_EVASGL_FUNCTION_BEGIN_VOID( glDrawArrays, GLenum, mode, GLint, first, GLsizei, count )
_EVASGL_FUNCTION_BEGIN_VOID( glDrawElements, GLenum, mode, GLsizei, count, GLenum, type, const void*, indices )
_EVASGL_FUNCTION_BEGIN_VOID( glEnableVertexAttribArray, GLuint, index )
_EVASGL_FUNCTION_BEGIN_VOID( glFinish )
_EVASGL_FUNCTION_BEGIN_VOID( glFlush )
_EVASGL_FUNCTION_BEGIN_VOID( glFrontFace, GLenum, mode )
_EVASGL_FUNCTION_BEGIN_VOID( glGenBuffers, GLsizei, n, GLuint*, buffers )
_EVASGL_FUNCTION_BEGIN_VOID( glGenerateMipmap, GLenum, target )
_EVASGL_FUNCTION_BEGIN_VOID( glGenFramebuffers, GLsizei, n, GLuint*, framebuffers )
_EVASGL_FUNCTION_BEGIN_VOID( glGenRenderbuffers, GLsizei, n, GLuint*, renderbuffers )
_EVASGL_FUNCTION_BEGIN_VOID( glGenTextures, GLsizei, n, GLuint*, textures )
_EVASGL_FUNCTION_BEGIN_VOID( glGetActiveAttrib, GLuint, program, GLuint, index, GLsizei, bufsize, GLsizei*, length, GLint*, size, GLenum*, type, char*, name )
_EVASGL_FUNCTION_BEGIN_VOID( glGetActiveUniform, GLuint, program, GLuint, index, GLsizei, bufsize, GLsizei*, length, GLint*, size, GLenum*, type, char*, name )
_EVASGL_FUNCTION_BEGIN_VOID( glGetAttachedShaders, GLuint, program, GLsizei, maxcount, GLsizei*, count, GLuint*, shaders )
_EVASGL_FUNCTION_BEGIN(int, glGetAttribLocation, GLuint, program, const char*, name )
_EVASGL_FUNCTION_BEGIN_VOID( glGetBooleanv, GLenum, pname, GLboolean*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glGetBufferParameteriv, GLenum, target, GLenum, pname, GLint*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glGetProgramiv, GLuint, program, GLenum, pname, GLint*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glGetProgramInfoLog, GLuint, program, GLsizei, bufsize, GLsizei*, length, char*, infolog )
_EVASGL_FUNCTION_BEGIN_VOID( glGetRenderbufferParameteriv, GLenum, target, GLenum, pname, GLint*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glGetShaderiv, GLuint, shader, GLenum, pname, GLint*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glGetShaderInfoLog, GLuint, shader, GLsizei, bufsize, GLsizei*, length, char*, infolog )
_EVASGL_FUNCTION_BEGIN_VOID( glGetShaderSource, GLuint, shader, GLsizei, bufsize, GLsizei*, length, char*, source )
_EVASGL_FUNCTION_BEGIN_VOID( glGetTexParameterfv, GLenum, target, GLenum, pname, GLfloat*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glGetTexParameteriv, GLenum, target, GLenum, pname, GLint*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glGetUniformfv, GLuint, program, GLint, location, GLfloat*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glGetUniformiv, GLuint, program, GLint, location, GLint*, params )
_EVASGL_FUNCTION_BEGIN(int, glGetUniformLocation, GLuint, program, const char*, name )
_EVASGL_FUNCTION_BEGIN_VOID( glGetVertexAttribfv, GLuint, index, GLenum, pname, GLfloat*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glGetVertexAttribiv, GLuint, index, GLenum, pname, GLint*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glGetVertexAttribPointerv, GLuint, index, GLenum, pname, void**, pointer )
_EVASGL_FUNCTION_BEGIN_VOID( glHint, GLenum, target, GLenum, mode )
_EVASGL_FUNCTION_BEGIN(GLboolean, glIsBuffer, GLuint, buffer )
_EVASGL_FUNCTION_BEGIN(GLboolean, glIsEnabled, GLenum, cap )
_EVASGL_FUNCTION_BEGIN(GLboolean, glIsFramebuffer, GLuint, framebuffer )
_EVASGL_FUNCTION_BEGIN(GLboolean, glIsProgram, GLuint, program )
_EVASGL_FUNCTION_BEGIN(GLboolean, glIsRenderbuffer, GLuint, renderbuffer )
_EVASGL_FUNCTION_BEGIN(GLboolean, glIsShader, GLuint, shader )
_EVASGL_FUNCTION_BEGIN(GLboolean, glIsTexture, GLuint, texture )
_EVASGL_FUNCTION_BEGIN_VOID( glLineWidth, GLfloat, width )
_EVASGL_FUNCTION_BEGIN_VOID( glLinkProgram, GLuint, program )
_EVASGL_FUNCTION_BEGIN_VOID( glPixelStorei, GLenum, pname, GLint, param )
_EVASGL_FUNCTION_BEGIN_VOID( glPolygonOffset, GLfloat, factor, GLfloat, units )
_EVASGL_FUNCTION_BEGIN_VOID( glRenderbufferStorage, GLenum, target, GLenum, internalformat, GLsizei, width, GLsizei, height )
_EVASGL_FUNCTION_BEGIN_VOID( glSampleCoverage, GLclampf, value, GLboolean, invert )
_EVASGL_FUNCTION_BEGIN_VOID( glShaderSource, GLuint, shader, GLsizei, count, const GLchar* const*, string, const GLint*, length )
_EVASGL_FUNCTION_BEGIN_VOID( glStencilFunc, GLenum, func, GLint, ref, GLuint, mask )
_EVASGL_FUNCTION_BEGIN_VOID( glStencilFuncSeparate, GLenum, face, GLenum, func, GLint, ref, GLuint, mask )
_EVASGL_FUNCTION_BEGIN_VOID( glStencilMask, GLuint, mask )
_EVASGL_FUNCTION_BEGIN_VOID( glStencilMaskSeparate, GLenum, face, GLuint, mask )
_EVASGL_FUNCTION_BEGIN_VOID( glStencilOp, GLenum, fail, GLenum, zfail, GLenum, zpass )
_EVASGL_FUNCTION_BEGIN_VOID( glStencilOpSeparate, GLenum, face, GLenum, fail, GLenum, zfail, GLenum, zpass )
_EVASGL_FUNCTION_BEGIN_VOID( glTexImage2D, GLenum, target, GLint, level, GLint, internalformat, GLsizei, width, GLsizei, height, GLint, border, GLenum, format, GLenum, type, const void*, pixels )
_EVASGL_FUNCTION_BEGIN_VOID( glTexParameterf, GLenum, target, GLenum, pname, GLfloat, param )
_EVASGL_FUNCTION_BEGIN_VOID( glTexParameterfv, GLenum, target, GLenum, pname, const GLfloat*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glTexParameteri, GLenum, target, GLenum, pname, GLint, param )
_EVASGL_FUNCTION_BEGIN_VOID( glTexParameteriv, GLenum, target, GLenum, pname, const GLint*, params )
_EVASGL_FUNCTION_BEGIN_VOID( glTexSubImage2D, GLenum, target, GLint, level, GLint, xoffset, GLint, yoffset, GLsizei, width, GLsizei, height, GLenum, format, GLenum, type, const void*, pixels )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform1f, GLint, location, GLfloat, x )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform1fv, GLint, location, GLsizei, count, const GLfloat*, v )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform1i, GLint, location, GLint, x )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform1iv, GLint, location, GLsizei, count, const GLint*, v )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform2f, GLint, location, GLfloat, x, GLfloat, y )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform2fv, GLint, location, GLsizei, count, const GLfloat*, v )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform2i, GLint, location, GLint, x, GLint, y )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform2iv, GLint, location, GLsizei, count, const GLint*, v )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform3f, GLint, location, GLfloat, x, GLfloat, y, GLfloat, z )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform3fv, GLint, location, GLsizei, count, const GLfloat*, v )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform3i, GLint, location, GLint, x, GLint, y, GLint, z )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform3iv, GLint, location, GLsizei, count, const GLint*, v )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform4f, GLint, location, GLfloat, x, GLfloat, y, GLfloat, z, GLfloat, w )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform4fv, GLint, location, GLsizei, count, const GLfloat*, v )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform4i, GLint, location, GLint, x, GLint, y, GLint, z, GLint, w )
_EVASGL_FUNCTION_BEGIN_VOID( glUniform4iv, GLint, location, GLsizei, count, const GLint*, v )
_EVASGL_FUNCTION_BEGIN_VOID( glUniformMatrix2fv, GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat*, value )
_EVASGL_FUNCTION_BEGIN_VOID( glUniformMatrix3fv, GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat*, value )
_EVASGL_FUNCTION_BEGIN_VOID( glUniformMatrix4fv, GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat*, value )
_EVASGL_FUNCTION_BEGIN_VOID( glUseProgram, GLuint, program )
_EVASGL_FUNCTION_BEGIN_VOID( glValidateProgram, GLuint, program )
_EVASGL_FUNCTION_BEGIN_VOID( glVertexAttrib1f, GLuint, indx, GLfloat, x )
_EVASGL_FUNCTION_BEGIN_VOID( glVertexAttrib1fv, GLuint, indx, const GLfloat*, values )
_EVASGL_FUNCTION_BEGIN_VOID( glVertexAttrib2f, GLuint, indx, GLfloat, x, GLfloat, y )
_EVASGL_FUNCTION_BEGIN_VOID( glVertexAttrib2fv, GLuint, indx, const GLfloat*, values )
_EVASGL_FUNCTION_BEGIN_VOID( glVertexAttrib3f, GLuint, indx, GLfloat, x, GLfloat, y, GLfloat, z )
_EVASGL_FUNCTION_BEGIN_VOID( glVertexAttrib3fv, GLuint, indx, const GLfloat*, values )
_EVASGL_FUNCTION_BEGIN_VOID( glVertexAttrib4f, GLuint, indx, GLfloat, x, GLfloat, y, GLfloat, z, GLfloat, w )
_EVASGL_FUNCTION_BEGIN_VOID( glVertexAttrib4fv, GLuint, indx, const GLfloat*, values )
_EVASGL_FUNCTION_BEGIN_VOID( glVertexAttribPointer, GLuint, indx, GLint, size, GLenum, type, GLboolean, normalized, GLsizei, stride, const void*, ptr )
