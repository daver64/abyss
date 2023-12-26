/***
 * Copyright 2004-2024, Dave Rowbotham and Toni Ylisirnio
 * All rights reserved.
 */
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <cstring>
#include <cstdarg>
#include "stb_image.h"
#include "sl.h"

#include <atomic>
std::atomic<float64> mouse_x;
std::atomic<float64> mouse_y;

#define CREATE_WINDOWED_CONTEXT_OK (0)
#define CREATE_FULLSCREEN_CONTEXT_OK (0)
#define ERROR_CREATE_WINDOWED_CONTEXT_ERROR (-1)
#define ERROR_CREATE_FULLSCREEN_CONTEXT_ERROR (-2)

static void cursor_position_callback(GLFWwindow *window, float64 xpos, float64 ypos)
{
    mouse_x = xpos;
    mouse_y = ypos;
}

static void glfw_error_callback(int32 error, const char *description)
{
    fprintf(stderr, "Error %d : %s\n", error, description);
}

static void framebuffer_size_callback(GLFWwindow *window, int32 width, int32 height)
{
    glViewport(0, 0, width, height);
}

vec2 get_mouse_position()
{
    vec2 mpos{(float32)mouse_x, (float32)mouse_y};
    return mpos;
}
void make_current(context *ctx)
{
    glfwMakeContextCurrent(ctx->window);
}

int32 create_fullscreen_context(context **ctx, const std::string &titletext)
{

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    *ctx = (context *)global_alloc(sizeof(context), "simlib context object");
    (*ctx)->fullscreen = true;
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    (*ctx)->width = mode->width;
    (*ctx)->height = mode->height;
    (*ctx)->quit_requested = false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, titletext.c_str(), monitor, nullptr);
    (*ctx)->window = window;
    if (!window)
    {
        fprintf(stderr, "failed to create window\n");
        glfwTerminate();
        return ERROR_CREATE_FULLSCREEN_CONTEXT_ERROR;
    }

    glfwMakeContextCurrent(window);
#ifdef _WIN32
    load_gl_extensions();
#endif
    glViewport(0, 0, mode->width, mode->height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSwapInterval(1);

    GLFWimage images[2];
    images[0].pixels = stbi_load("../data/textures/bud.png", &images[0].width, &images[0].height, 0, 4);
    images[1].pixels = stbi_load("../data/textures/bud.png", &images[1].width, &images[1].height, 0, 4);
    if (images[0].pixels && images[1].pixels)
    {
        glfwSetWindowIcon(window, 2, images);
        stbi_image_free(images[0].pixels);
    }
    init_sound();
    return CREATE_FULLSCREEN_CONTEXT_OK;
}
int32 create_windowed_context(context **ctx, const std::string &titletext, const int32 width, const int32 height)
{
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    int32 actual_width = width;
    int32 actual_height = height;
    if (width == 0 || height == 0)
    {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        actual_width = mode->width / 2;
        actual_height = mode->height / 2;
    }
    *ctx = (context *)global_alloc(sizeof(context), "simlib windowed context object");

    (*ctx)->fullscreen = false;
    (*ctx)->width = actual_width;
    (*ctx)->height = actual_height;
    (*ctx)->quit_requested = false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow *window = glfwCreateWindow(actual_width, actual_height, titletext.c_str(), nullptr, nullptr);

    (*ctx)->window = window;
    if (!window)
    {
        global_free((*ctx));
        (*ctx)=nullptr;
        fprintf(stderr, "failed to create window\n");
        glfwTerminate();
        return ERROR_CREATE_WINDOWED_CONTEXT_ERROR;
    }

    glfwMakeContextCurrent(window);
#ifdef _WIN32
    load_gl_extensions();
#endif
    glViewport(0, 0, actual_width, actual_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSwapInterval(1);

    GLFWimage images[2];
    images[0].pixels = stbi_load("../data/textures/bud.png", &images[0].width, &images[0].height, 0, 4);
    images[1].pixels = stbi_load("../data/textures/bud.png", &images[1].width, &images[1].height, 0, 4);
    if (images[0].pixels && images[1].pixels)
    {
        glfwSetWindowIcon(window, 2, images);
        stbi_image_free(images[0].pixels);
    }
    init_sound();
    return CREATE_WINDOWED_CONTEXT_OK;
}

void destroy_context(context **ctx)
{
    assert(*ctx);
    global_free(*ctx);
    *ctx = nullptr;
    glfwTerminate();
    deinit_sound();
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
