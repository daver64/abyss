
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <cstring>
#include <cstdarg>
#include "stb_image.h"
#include "sl.h"

static float64 frame_delta_t_ms = 16.0;
static float64 frame_start_t_ms = 0.0;

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
void make_current(context *ctx)
{
    glfwMakeContextCurrent(ctx->window);
}
int32 create_context(context **ctx, const std::string &titletext)
{

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    *ctx = new context;
    (*ctx)->fullscreen = true;
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    (*ctx)->width = mode->width;
    (*ctx)->height = mode->height;
    (*ctx)->quit_requested = false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, titletext.c_str(), monitor, nullptr);
    (*ctx)->window = window;
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
    glViewport(0, 0, mode->width, mode->height);
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
    float64 f = glfwGetTime() * 1000.0;
    frame_delta_t_ms = f - frame_start_t_ms;
}

const float64 get_frame_delta_t_ms()
{
    return frame_delta_t_ms;
}
void poll_input(context *ctx)
{
    frame_start_t_ms = glfwGetTime() * 1000.0;
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

void textout(textureatlas *atlas, const char *text, int32 x, int32 y, pixel32 colour)
{
    int32 len = (int32)strlen(text);
    int32 endx = x + len * atlas->tile_width;
    const char *ptr = text;
    int32 xpos = x;
    int32 ypos = y;
    bind_atlas(atlas);
    begin_atlas(atlas);
    for (int32 i = 0; i < len; i++)
    {
        int32 index = (int32)text[i];
        if (text[i] == '\n')
        {
            ypos += atlas->tile_height;
            xpos = x;
        }
        else if (text[i] == '\t')
        {
            int32 tabs = 2;
            xpos += atlas->tile_width * tabs;
        }
        else
        {
            draw_atlas_tile(atlas, xpos, ypos, atlas->tile_width, atlas->tile_height, index, colour);
            xpos += atlas->tile_width;
        }
    }
    end_atlas(atlas);
}

void gprintf(textureatlas *atlas, float32 x, float32 y, pixel32 colour, const char *fmt, ...)
{
    char buf[4096];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 4096, fmt, args);
    va_end(args);
    textout(atlas, buf, x, y, colour);
}