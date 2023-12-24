
#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "sl_glext.h"
#else
#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#endif
#include "stb_image.h"
#include "sl.h"
#include <algorithm>
#include <functional>
#include <cmath>

int32 create_framebuffer(framebuffer **fb, int32 width, int32 height)
{
	(*fb) = (framebuffer *)global_alloc(sizeof(framebuffer), "framebuffer object"); // new framebuffer;
	(*fb)->width = width;
	(*fb)->height = height;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &(*fb)->glref);
	glBindTexture(GL_TEXTURE_2D, (*fb)->glref);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*fb)->width, (*fb)->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenRenderbuffers(1, &(*fb)->render_id);
	glBindRenderbuffer(GL_RENDERBUFFER, (*fb)->render_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, (*fb)->width, (*fb)->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glGenFramebuffers(1, &(*fb)->frame_id);
	glBindFramebuffer(GL_FRAMEBUFFER, (*fb)->frame_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (*fb)->glref, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, (*fb)->frame_id);
	(*fb)->valid = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return 0;
}
void destroy_framebuffer(framebuffer **fb)
{
	glDeleteFramebuffers(1, &(*fb)->frame_id);
	glDeleteFramebuffers(1, &(*fb)->render_id);
	// delete (*fb);
	global_free(*fb);
	(*fb) = nullptr;
}
void bind_framebuffer(framebuffer *fb)
{
	glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, fb->frame_id);
	glBindRenderbuffer(GL_RENDERBUFFER, fb->render_id);
	glViewport(0, 0, fb->width, fb->height);
}
void unbind_framebuffer(framebuffer *fb)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glPopAttrib();
}