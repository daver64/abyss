
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <cstring>
#include <cstdarg>
#include "stb_image.h"
#include "sl.h"

#include <atomic>
std::atomic<float64> mouse_x;
std::atomic<float64> mouse_y;

static void cursor_position_callback(GLFWwindow *window,float64 xpos,float64 ypos)
{
    mouse_x=xpos;
    mouse_y=ypos;
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
    vec2 mpos{(float32)mouse_x,(float32)mouse_y};
    return mpos;
}
void make_current(context *ctx)
{
    glfwMakeContextCurrent(ctx->window);
}


int32 create_fullscreen_context(context** ctx, const std::string& titletext)
{

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    *ctx = (context*)global_alloc(sizeof(context));
    (*ctx)->fullscreen = true;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    (*ctx)->width = mode->width;
    (*ctx)->height = mode->height;
    (*ctx)->quit_requested = false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, titletext.c_str(), monitor, nullptr);
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
    glfwSetCursorPosCallback(window,cursor_position_callback);
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
    init_sound();
    return 0;
}
int32 create_windowed_context(context** ctx, int32 width, int32 height, const std::string& titletext)
{

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    *ctx = (context*)global_alloc(sizeof(context));
    (*ctx)->fullscreen = false;

    (*ctx)->width = width;
    (*ctx)->height = height;
    (*ctx)->quit_requested = false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, titletext.c_str(), nullptr, nullptr);
    
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
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window,cursor_position_callback);
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
    init_sound();
    return 0;
}

void destroy_context(context **ctx)
{
    assert(*ctx);
   // delete (*ctx);
    global_free(*ctx);
    *ctx = nullptr;
    glfwTerminate();
    deinit_sound();

}