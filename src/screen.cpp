
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <cstring>
#include <cstdarg>
#include "stb_image.h"
#include "sl.h"

float64 frame_delta_t_ms = 16.0;
float64 frame_start_t_ms = 0.0;

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
void ortho2d(context *app_context,bool flip, float32 near_z,float32 far_z)
{
    ortho2d(app_context->width,app_context->height,flip,near_z,far_z);  
}

#define NUM_TIME_SAMPLES 8
float64 average_frame_delta_t_ms{0.0};
float64 average_frame_delta_t_ms_samples[NUM_TIME_SAMPLES]{0.0};
int32 average_frame_delta_t_index = 0;
void swap(context *ctx)
{
    glfwSwapBuffers(ctx->window);
    float64 f = glfwGetTime() * 1000.0;
    frame_delta_t_ms = f - frame_start_t_ms;
    average_frame_delta_t_ms_samples[average_frame_delta_t_index] = frame_delta_t_ms;
    average_frame_delta_t_index++;
    average_frame_delta_t_index = average_frame_delta_t_index % NUM_TIME_SAMPLES;
}

const float64 get_frame_delta_t_ms()
{
    float64 result = 0.0f;
    for (int32 i = 0; i < NUM_TIME_SAMPLES; i++)
    {
        result += average_frame_delta_t_ms_samples[i];
    }
    if (result > 0.0)
    {
        result /= (float64)NUM_TIME_SAMPLES;
    }

    return result;
}


void clear_screen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clear_screen(pixel32 clearcolour)
{
    const float32 r = getr_nf(clearcolour);
    const float32 g = getg_nf(clearcolour);
    const float32 b = getb_nf(clearcolour);
    const float32 a = geta_nf(clearcolour);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void clear_screen(rgba clearcolour)
{
    glClearColor(clearcolour.r, clearcolour.g, clearcolour.b, clearcolour.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void textout(textureatlas *atlas, const char *text, int32 x, int32 y, pixel32 textcolour)
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
            draw_atlas_tile(atlas, xpos, ypos, atlas->tile_width, atlas->tile_height, index, textcolour);
            xpos += atlas->tile_width;
        }
    }
    end_atlas(atlas);
}

void gprintf(textureatlas *atlas, float32 x, float32 y, pixel32 textcolour, const char *fmt, ...)
{
    char buf[4096];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 4096, fmt, args);
    va_end(args);
    textout(atlas, buf, x, y, textcolour);
}