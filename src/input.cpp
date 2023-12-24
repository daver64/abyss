#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "sl.h"

uint32 keys[4] { 0,0,0,0};

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