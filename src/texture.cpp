
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
int32 create_texture(texture **tex,int32 width, int32 height, const bool mipmapped)
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
        if(mipmapped)
        {
            enable_mipmapping();
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*tex)->width, (*tex)->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*tex)->pixels);
    }
    return 0;
}
int32 create_texture(texture **tex,const std::string &filename, const bool mipmapped)
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    return 0;
}
void destroy_texture(texture **tex)
{
    delete[] (*tex)->pixels;
    glDeleteTextures(1, &(*tex)->ref);
    *tex=nullptr;
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
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA,type,  tex->pixels);
}
void bind_texture(texture *tex)
{
    glBindTexture(GL_TEXTURE_2D, tex->ref);
}
void bind_texture(drawbuffer* db, texture* tex)
{
    db->bind_tex0(tex->ref);
    glBindTexture(GL_TEXTURE_2D, tex->ref);
}
void texture_unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
