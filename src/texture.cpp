
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "abyss.h"

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
int32 create_texture(texture **tex, int32 width, int32 height, const bool mipmapped)
{
    (*tex) = new texture;
    if ((*tex))
    {
        (*tex)->pixels = new pixel32[width * height];
        (*tex)->ref = 0;
        (*tex)->width = width;
        (*tex)->height = height;
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &(*tex)->ref);
        glBindTexture(GL_TEXTURE_2D, (*tex)->ref);
        if (mipmapped)
        {
            enable_mipmapping();
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*tex)->width, (*tex)->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*tex)->pixels);
    }
    return 0;
}
int32 create_texture(texture **tex, const std::string &filename, const bool mipmapped)
{
    (*tex) = new texture;
    pixel32 *tmp_pix = (pixel32 *)stbi_load(filename.c_str(), &(*tex)->width, &(*tex)->height, 0, 4);
    if (tmp_pix)
    {
        (*tex)->pixels = new pixel32[(*tex)->width * (*tex)->height];
        memcpy((*tex)->pixels, tmp_pix, (*tex)->width * (*tex)->height * sizeof(pixel32));
        stbi_image_free((void *)tmp_pix);
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &(*tex)->ref);
        glBindTexture(GL_TEXTURE_2D, (*tex)->ref);
        if (mipmapped)
        {
            enable_mipmapping();
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*tex)->width, (*tex)->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*tex)->pixels);
    }
    return 0;
}
void destroy_texture(texture **tex)
{
    delete[] (*tex)->pixels;
    glDeleteTextures(1, &(*tex)->ref);
    *tex = nullptr;
}

void texture_gpu_write(texture *tex)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex->ref);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);
}
void texture_gpu_read(texture *tex)
{
    GLenum type = GL_UNSIGNED_BYTE;
    glBindTexture(GL_TEXTURE_2D, tex->ref);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, type, tex->pixels);
}
void bind_texture(texture *tex)
{
    glBindTexture(GL_TEXTURE_2D, tex->ref);
}
void bind_texture(drawbuffer *db, texture *tex)
{
    db->bind_tex0(tex->ref);
    glBindTexture(GL_TEXTURE_2D, tex->ref);
}
void texture_unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

int32 create_atlas(textureatlas **atlas,drawbuffer *target,const std::string& filename,int32 numx,int32 numy)
{
    (*atlas) = new textureatlas;
    (*atlas)->numtiles_x=numx;
    (*atlas)->numtiles_y=numy;
    (*atlas)->target = target;
    int32 result = create_texture(&(*atlas)->tex,filename,true);
    (*atlas)->tile_height=(*atlas)->tex->height/numy;
    (*atlas)->tile_width=(*atlas)->tex->width/numx;
    return result;
}
int32 destroy_atlas(textureatlas **atlas)
{
    destroy_texture(&(*atlas)->tex);
    delete (*atlas);
    (*atlas)=nullptr;
    return 0;
}
void bind_atlas(textureatlas *atlas)
{
    bind_texture(atlas->target,atlas->tex);
}
void begin_atlas(textureatlas *atlas)
{
    atlas->target->begin(GL_QUADS);
}
void draw_atlas_tile(textureatlas *atlas, float_t x1, float_t y1, float_t width,
                    float_t height, int32 index, pixel32 colour)
{
    drawbuffer *target=atlas->target;
    texture *tex=atlas->tex;
    int32 numtiles_x = atlas->numtiles_x;
    int32 numtiles_y = atlas->numtiles_y;
    int32 tile_width = atlas->tile_width;
    int32 tile_height = atlas->tile_height;
	const float32 x2 = x1 + width;
	const float32 y2 = y1 + height;

	const int32 tile_x = index % (numtiles_x);
	const int32 tile_y = index / (numtiles_x);

	const float32 u1 = (float32)tile_width * (float32)tile_x / (float32)tex->width;
	const float32 v1 = (float32)tile_height * (float32)tile_y / (float32)tex->height;
	const float32 u_span = (float32)(tile_width) / (float32)tex->width;
	const float32 v_span = (float32)(tile_height) / (float32)tex->height;
	const float32 u2 = u1 + u_span;
	const float32 v2 = v1 + v_span;

	target->colour(colour);

	target->texcoord0(vec2(u1, v1));
	target->vertex(vec2(x1, y1));

	target->texcoord0(vec2(u2, v1));
	target->vertex(vec2(x2, y1)); 

	target->texcoord0(vec2(u2, v2));
	target->vertex(vec2(x2, y2));

	target->texcoord0(vec2(u1, v2));
	target->vertex(vec2(x1, y2));
}
void draw_atlas_tile(textureatlas *atlas, int32 x1, int32 y1, int32 width, int32 height, int32 index, pixel32 colour)
{
    draw_atlas_tile(atlas,(float32)x1,(float32)y1,(float32)width,(float32)height,index,colour);
}
void end_atlas(textureatlas *atlas)
{
    atlas->target->end();
}
