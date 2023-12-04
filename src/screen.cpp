
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <cstring>
#include "stb_image.h"
#include "abyss.h"

void ortho2d(int32 width, int32 height, bool flip, float32 near_z, float32 far_z)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (!flip)
        glOrtho(0, width, height, 0, near_z, far_z);
    else
        glOrtho(0, width, 0, height, near_z, far_z);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void glfw_error_callback(int32 error, const char *description)
{
    fprintf(stderr, "Error %d : %s\n", error, description);
}

static void framebuffer_size_callback(GLFWwindow *window, int32 width, int32 height)
{
    glViewport(0, 0, width, height);
}

int32 create_context(context **ctx, const std::string &titletext, int32 width, int32 height, bool fullscreen)
{
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    GLFWwindow *window = glfwCreateWindow(width, height, titletext.c_str(), nullptr, nullptr);
    if (!window)
    {
        fprintf(stderr, "failed to create window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    #ifdef _WIN32
    load_gl_extensions();
    #endif
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    GLFWimage images[2];
    images[0].pixels = stbi_load("../data/textures/bud.png", &images[0].width, &images[0].height, 0, 4);
    images[1].pixels = stbi_load("../data/textures/bud.png", &images[1].width, &images[1].height, 0, 4);
    if (images[0].pixels && images[1].pixels)
    {
        glfwSetWindowIcon(window, 2, images);
        stbi_image_free(images[0].pixels);
    }
    *ctx = new context;
    (*ctx)->width = width;
    (*ctx)->height = height;
    (*ctx)->fullscreen = fullscreen;
    (*ctx)->window = window;
    (*ctx)->quit_requested = false;
    return 0;
}

int32 destroy_context(context **ctx)
{
    assert(*ctx);
    delete (*ctx);
    *ctx = nullptr;
    glfwTerminate();
    return 0;
}

bool want_to_quit(context *ctx)
{
    assert(ctx);
    return ctx->quit_requested;
}

void app_quit(context *ctx)
{
    assert(ctx);
    ctx->quit_requested = true;
}

void swap(context *ctx)
{
    glfwSwapBuffers(ctx->window);
}

void poll_input(context *ctx)
{
    glfwPollEvents();
    if (glfwWindowShouldClose(ctx->window))
    {
        ctx->quit_requested = true;
    }
}

void process_input(context *ctx)
{
    poll_input(ctx);
    if (glfwGetKey(ctx->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        app_quit(ctx);
        glfwSetWindowShouldClose(ctx->window, true);
    }
}

void clear_screen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clear_screen(pixel32 colour)
{
    const float32 r = getr_nf(colour);
    const float32 g = getg_nf(colour);
    const float32 b = getb_nf(colour);
    const float32 a = geta_nf(colour);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void textout(textureatlas *atlas, const char *text, int32 x, int32 y)
{
	int32 len = (int32)strlen(text);
	int32 endx = x + len * atlas->tile_width;
	const char *ptr = text;
	int32 xpos=x;//*atlas->tile_width;
	bind_atlas(atlas);
	begin_atlas(atlas);
	for (int32 i = 0; i < len; i++)
	{
		int32 index = (int32)text[i];
		draw_atlas_tile(atlas,xpos,y,atlas->tile_width,atlas->tile_height,index,x11colours::white);
		xpos+=atlas->tile_width;
	}
	end_atlas(atlas);
}