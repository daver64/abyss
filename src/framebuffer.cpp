
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
#include "abyss.h"

#include <algorithm>
#include <functional>
#include <cmath>

int32 create_framebuffer(framebuffer **fb, int32 width, int32 height)
{

    return 0;
}
void destroy_framebuffer(framebuffer **fb)
{

}
void bind_framebuffer(framebuffer *fb)
{

}
void unbind_framebuffer(framebuffer *fb)
{

}