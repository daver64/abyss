/***
* Copyright 2004-2024, Dave Rowbotham and Toni Ylisirnio
* All rights reserved.
*/
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

void enable_depthtest()
{
    glEnable(GL_DEPTH_TEST);
}

void disable_depthtest()
{
    glDisable(GL_DEPTH_TEST);
}
void enable_blending()
{
    glEnable(GL_BLEND);
    // glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}
void disable_blending()
{
    glDisable(GL_BLEND);
}
void set_clear_colour(pixel32 colour)
{
    const float32 r = getr_nf(colour);
    const float32 g = getg_nf(colour);
    const float32 b = getb_nf(colour);
    const float32 a = geta_nf(colour);
    glClearColor(r, g, b, a);
}

void enable_arrays()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void disable_arrays()
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void enable_multisampling()
{
    glEnable(GL_MULTISAMPLE);
}

void disable_multisampling()
{
    glDisable(GL_MULTISAMPLE);
}

void enable_texturing()
{
    glEnable(GL_TEXTURE_2D);
}

void disable_texturing()
{
    glDisable(GL_TEXTURE_2D);
}

void enable_texture_clamping()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void disable_texture_clamping()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void enable_mipmapping()
{
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void disable_mipmapping()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void disable_texture_filtering()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void enable_texture_filtering()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
