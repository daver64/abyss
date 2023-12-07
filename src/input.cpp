#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "abyss.h"

uint32 keys[3] { 0,0,0};

uint32 getkey(uint32 k)
{
    return keys[k];
}
void keyboard_callback(GLFWwindow *window, int32 key, int32 scancode, int32 action, int32 mods)
{
    keys[0]=key;
}

void keyboard_install(context *ctx)
{
    glfwSetKeyCallback(ctx->window, keyboard_callback);
}