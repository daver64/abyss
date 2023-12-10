/**
 * (c) 2023 David Rowbotham thedaver64@gmail.com
*/
#include <windows.h>
#include "sl.h"
//#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

#include "sl_glext.h"

PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = NULL;
PFNGLGENERATEMIPMAP glGenerateMipmap = NULL;
PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLDETACHSHADERPROC glDetachShader = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLUNIFORM1FVPROC glUniform1fv = NULL;
PFNGLUNIFORM2FVPROC glUniform2fv = NULL;
PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
PFNGLUNIFORM4FVPROC glUniform4fv = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLPROGRAMPARAMETEREIPROC glProgramParameteri = NULL;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = NULL;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = NULL;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = NULL;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = NULL;
PFNGLWGLSWAPINTERVALPROC wglSwapInterval = NULL;
PFNGLGLBINDATTRIBLOCATIONPROC glBindAttribLocation = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
PFNGLCOMPRESSEDTEXIMAGE1D glCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2D glCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE3D glCompressedTexImage3D = NULL;
PFNGLTEXIMAGE3D glTexImage3D = NULL;
PFNGLCREATETEXTURES glCreateTextures = NULL;
PFNGLTEXTURESTORAGE2D glTextureStorage2D = NULL;
PFNGLTEXTURESUBIMAGE2D glTextureSubImage2D = NULL;

PFNGLGENQUERIES glGenQueries = NULL;
PFNGLQUERYCOUNTER glQueryCounter = NULL;
PFNGLGETQUERYOBJECTIV glGetQueryObjectiv = NULL;
PFNGLGETQUERYOBJECTUI64V glGetQueryObjectui64v = NULL;
void load_gl_extensions()
{
	// manually load OpenGL features windows doesn't provide by default.
	wglSwapInterval = (PFNGLWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
	glGenerateMipmap = (PFNGLGENERATEMIPMAP)wglGetProcAddress("glGenerateMipmap");
	glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");
	glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)wglGetProcAddress("glBlendFuncSeparate");
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)wglGetProcAddress("glClientActiveTexture");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glProgramParameteri = (PFNGLPROGRAMPARAMETEREIPROC)wglGetProcAddress("glProgramParameteri");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
	glBindAttribLocation = (PFNGLGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
	glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1D)wglGetProcAddress("glCompressedTexImage1D");
	glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2D)wglGetProcAddress("glCompressedTexImage2D");
	glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3D)wglGetProcAddress("glCompressedTexImage3D");
	glTexImage3D = (PFNGLTEXIMAGE3D)wglGetProcAddress("glTexImage3D");
	glCreateTextures = (PFNGLCREATETEXTURES)wglGetProcAddress("glCreateTextures");
	glTextureStorage2D = (PFNGLTEXTURESTORAGE2D)wglGetProcAddress("glTextureStorage2D");
	glTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2D)wglGetProcAddress("glTextureSubImage2D");
	glGenQueries = (PFNGLGENQUERIES)wglGetProcAddress("glGenQueries");
	glQueryCounter = (PFNGLQUERYCOUNTER)wglGetProcAddress("glQueryCounter");
	glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIV)wglGetProcAddress("glGetQueryObjectiv");
	glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64V)wglGetProcAddress("glGetQueryObjectui64v");
}
