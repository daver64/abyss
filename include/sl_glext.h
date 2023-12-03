/**
 * (c) 2023 David Rowbotham thedaver64@gmail.com
*/
#pragma once
#include <cstdint>

#ifdef __cplusplus
extern "C"
{
#endif
#ifndef GLint64
	typedef int64_t GLint64;
#endif
#ifndef GLuint64
	typedef uint64_t GLuint64;
#endif

	// Only load the OpenGL extensions we are actually going to use. 
	typedef void (APIENTRY* PFNGLBLENDEQUATIONPROC) (GLenum mode);
	extern PFNGLBLENDEQUATIONPROC glBlendEquation;
	typedef void (APIENTRY* PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
	extern PFNGLGENBUFFERSPROC glGenBuffers;
	typedef void (APIENTRY* PFNGLDELETEBUFFERSPROC)(GLsizei n, GLuint* buffers);
	extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	typedef void (APIENTRY* PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
	extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	typedef void (APIENTRY* PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
	extern PFNGLBINDBUFFERPROC glBindBuffer;
	typedef void (APIENTRY* PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalised, GLsizei stride, const GLvoid* pointer);
	extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	typedef void (APIENTRY* PFNGLBUFFERDATAPROC)(GLenum target, GLsizei size, const GLvoid* data, GLenum usage);
	extern PFNGLBUFFERDATAPROC glBufferData;
	typedef void (APIENTRY* PFNGLACTIVETEXTUREPROC)(GLenum texture);
	extern PFNGLACTIVETEXTUREPROC glActiveTexture;
	typedef void (APIENTRY* PFNGLCLIENTACTIVETEXTUREPROC)(GLenum textureunit);
	extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
	typedef GLuint(APIENTRY* PFNGLCREATESHADERPROC)(GLenum type);
	extern PFNGLCREATESHADERPROC glCreateShader;
	typedef void (APIENTRY* PFNGLSHADERSOURCEPROC)(GLint shader, GLsizei count, const char** string, const GLint* length);
	extern PFNGLSHADERSOURCEPROC glShaderSource;
	typedef void (APIENTRY* PFNGLCOMPILESHADERPROC)(GLuint shader);
	extern PFNGLCOMPILESHADERPROC glCompileShader;
	typedef void (APIENTRY* PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
	extern PFNGLGETSHADERIVPROC glGetShaderiv;
	typedef void (APIENTRY* PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei maxlength, GLsizei* length, char* infolog);
	extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	typedef GLuint(APIENTRY* PFNGLCREATEPROGRAMPROC)();
	extern PFNGLCREATEPROGRAMPROC glCreateProgram;
	typedef void (APIENTRY* PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
	extern PFNGLATTACHSHADERPROC glAttachShader;
	typedef void (APIENTRY* PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
	extern PFNGLDETACHSHADERPROC glDetachShader;
	typedef void (APIENTRY* PFNGLDELETESHADERPROC)(GLuint shader);
	extern PFNGLDELETESHADERPROC glDeleteShader;
	typedef void (APIENTRY* PFNGLLINKPROGRAMPROC)(GLuint program);
	extern PFNGLLINKPROGRAMPROC glLinkProgram;
	typedef void (APIENTRY* PFNGLUSEPROGRAMPROC)(GLuint program);
	extern PFNGLUSEPROGRAMPROC glUseProgram;
	typedef GLint(APIENTRY* PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const char* name);
	extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	typedef void (APIENTRY* PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
	extern PFNGLUNIFORM1FPROC glUniform1f;
	typedef void(APIENTRY* PFNGLUNIFORM1IPROC)(GLint location, GLint v);
	extern PFNGLUNIFORM1IPROC glUniform1i;
	typedef void(APIENTRY* PFNGLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat* value);
	extern  PFNGLUNIFORM1FVPROC glUniform1fv;
	typedef void(APIENTRY* PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat* value);
	extern  PFNGLUNIFORM2FVPROC glUniform2fv;
	typedef void(APIENTRY* PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat* value);
	extern  PFNGLUNIFORM3FVPROC glUniform3fv;
	typedef void(APIENTRY* PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat* value);
	extern  PFNGLUNIFORM4FVPROC glUniform4fv;

	typedef void(APIENTRY* PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	extern  PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

	typedef void(APIENTRY* PFNGLGENERATEMIPMAP)(GLenum target);
	extern PFNGLGENERATEMIPMAP glGenerateMipmap;

	typedef void (APIENTRY* PFNGLPROGRAMPARAMETEREIPROC)(GLuint program, GLenum pname, GLint value);
	extern PFNGLPROGRAMPARAMETEREIPROC glProgramParameteri;
	typedef void (APIENTRY* PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
	extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
	typedef void (APIENTRY* PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
	extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
	typedef void (APIENTRY* PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint* framebuffers);
	extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
	typedef void (APIENTRY* PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint* ids);
	extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	typedef void (APIENTRY* PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint* renderbuffers);
	extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
	typedef void (APIENTRY* PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, GLuint* renderbuffers);
	extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
	typedef void (APIENTRY* PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
	typedef void (APIENTRY* PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
	typedef void (APIENTRY* PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
	typedef GLenum(APIENTRY* PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
	extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
	typedef BOOL(APIENTRY* PFNGLWGLSWAPINTERVALPROC)(int interval);
	extern PFNGLWGLSWAPINTERVALPROC wglSwapInterval;
	typedef void (APIENTRY* PFNGLGLBINDATTRIBLOCATIONPROC)(GLuint program, GLuint index, const char* name);
	extern PFNGLGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
	//typedef void (APIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
	//extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	typedef void (APIENTRY* PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
	extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
	typedef void (APIENTRY* PFNGLBUFFERSUBDATAPROC)(GLenum target, GLint offset, GLsizei size, const GLvoid* data);
	extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
	typedef void(APIENTRY* PFNGLCOMPRESSEDTEXIMAGE1D)(GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLint border, GLsizei imageSize, const void* data);
	extern PFNGLCOMPRESSEDTEXIMAGE1D glCompressedTexImage1D;
	typedef void(APIENTRY* PFNGLCOMPRESSEDTEXIMAGE2D)(GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
	extern PFNGLCOMPRESSEDTEXIMAGE2D glCompressedTexImage2D;
	typedef void(APIENTRY* PFNGLCOMPRESSEDTEXIMAGE3D)(GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
	extern PFNGLCOMPRESSEDTEXIMAGE3D glCompressedTexImage3D;
	typedef void(APIENTRY* PFNGLTEXIMAGE3D)(GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* data);
	extern PFNGLTEXIMAGE3D glTexImage3D;
	
	typedef void (APIENTRY* PFNGLCREATETEXTURES)(GLenum target, GLsizei n, GLuint* textures);
	extern PFNGLCREATETEXTURES glCreateTextures;
	typedef void (APIENTRY* PFNGLTEXTURESTORAGE2D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
	extern PFNGLTEXTURESTORAGE2D glTextureStorage2D;
	typedef void (APIENTRY* PFNGLTEXTURESUBIMAGE2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
		GLenum format, GLenum type, const void* pixels);
	extern PFNGLTEXTURESUBIMAGE2D glTextureSubImage2D;
	typedef void (APIENTRY* PFNGLGENQUERIES)(GLsizei n, GLuint* ids);
	extern PFNGLGENQUERIES glGenQueries;
	typedef void (APIENTRY* PFNGLQUERYCOUNTER)(GLuint id, GLenum target);
	extern PFNGLQUERYCOUNTER glQueryCounter;
	typedef void (APIENTRY* PFNGLGETQUERYOBJECTIV)(GLuint id, GLenum pname, GLint* params);
	extern PFNGLGETQUERYOBJECTIV glGetQueryObjectiv;
	typedef void (APIENTRY* PFNGLGETQUERYOBJECTUI64V)(GLuint id, GLenum pname, GLint64* params);
	extern PFNGLGETQUERYOBJECTUI64V glGetQueryObjectui64v;
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_MULTISAMPLE                    0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE            0x809F
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP         0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_COMPRESSED_RGB                 0x84ED
#define GL_COMPRESSED_RGBA                0x84EE
#define GL_TEXTURE_COMPRESSION_HINT       0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE  0x86A0
#define GL_TEXTURE_COMPRESSED             0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define GL_CLAMP_TO_BORDER                0x812D
#define GL_CLIENT_ACTIVE_TEXTURE          0x84E1
#define GL_MAX_TEXTURE_UNITS              0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX     0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX    0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX       0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX         0x84E6
#define GL_MULTISAMPLE_BIT                0x20000000
#define GL_NORMAL_MAP                     0x8511
#define GL_REFLECTION_MAP                 0x8512
#define GL_COMPRESSED_ALPHA               0x84E9
#define GL_COMPRESSED_LUMINANCE           0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA     0x84EB
#define GL_COMPRESSED_INTENSITY           0x84EC
#define GL_COMBINE                        0x8570
#define GL_COMBINE_RGB                    0x8571
#define GL_COMBINE_ALPHA                  0x8572
#define GL_SOURCE0_RGB                    0x8580
#define GL_SOURCE1_RGB                    0x8581
#define GL_SOURCE2_RGB                    0x8582
#define GL_SOURCE0_ALPHA                  0x8588
#define GL_SOURCE1_ALPHA                  0x8589
#define GL_SOURCE2_ALPHA                  0x858A
#define GL_OPERAND0_RGB                   0x8590
#define GL_OPERAND1_RGB                   0x8591
#define GL_OPERAND2_RGB                   0x8592
#define GL_OPERAND0_ALPHA                 0x8598
#define GL_OPERAND1_ALPHA                 0x8599
#define GL_OPERAND2_ALPHA                 0x859A
#define GL_RGB_SCALE                      0x8573
#define GL_ADD_SIGNED                     0x8574
#define GL_INTERPOLATE                    0x8575
#define GL_SUBTRACT                       0x84E7
#define GL_CONSTANT                       0x8576
#define GL_PRIMARY_COLOR                  0x8577
#define GL_PREVIOUS                       0x8578
#define GL_DOT3_RGB                       0x86AE
#define GL_DOT3_RGBA                      0x86AF

#define GL_FUNC_ADD 0x8006
#define GL_MIN 0x8007
#define GL_MAX 0x8008
#define GL_ARRAY_BUFFER 0x8892

#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA

#define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_LINES_ADJACENCY 0x000A
#define GL_LINE_STRIP_ADJACENCY 0x000B
#define GL_TRIANGLES_ADJACENCY 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D


#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D

#define GL_GENERATE_MIPMAP 0x8191
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7

#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_GEOMETRY_VERTICES_OUT 0x8916
#define GL_GEOMETRY_INPUT_TYPE 0x8917
#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125

#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ATTACHED_SHADERS 0x8B85

#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_ELEMENT_ARRAY_BUFFER 0x8893

#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

#define GL_RGBA32F                        0x8814
#define GL_BGRA                           0x80E1
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506

#define GL_TIME_ELAPSED                   0x88BF
#define GL_TIMESTAMP                      0x8E28
#define GL_QUERY_COUNTER_BITS             0x8864
#define GL_CURRENT_QUERY                  0x8865
#define GL_QUERY_RESULT                   0x8866
#define GL_QUERY_RESULT_AVAILABLE         0x8867

#ifndef GL_NV_texture_rectangle
#define GL_NV_texture_rectangle 1
#define GL_TEXTURE_RECTANGLE_NV           0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_NV   0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_NV     0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_NV  0x84F8
#endif /* GL_NV_texture_rectangle */

#define GL_TEXTURE_3D 0x806F

#ifdef __cplusplus
}
#endif
