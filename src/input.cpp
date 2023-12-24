#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "sl.h"

uint32 keys[4] { 0,0,0,0};
extern float64 frame_start_t_ms;
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

uint32 getkey(uint32 k)
{
    uint32 rk=keys[k];
    return rk;
}
void keyboard_callback(GLFWwindow *window, int32 key, int32 scancode, int32 action, int32 mods)
{
    // key      : GLFW_KEY_A etc
    // scancode : platform specific scan code. VK_A??
    // action   : GLFW_PRESS, GLFW_REPEAT, GLFW_RELEASE etc
    // mods     : GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, GLFW_CAPS_LOCK etc
    keys[0]=key;
    keys[1]=scancode;
    keys[2]=action;
    keys[3]=mods;
}

void keyboard_install(context *ctx)
{
    glfwSetKeyCallback(ctx->window, keyboard_callback);
}