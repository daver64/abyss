#pragma once


#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <xmmintrin.h>
#include <AL/al.h>
#include <AL/alc.h>
#ifdef _WIN32
#include "resource.h"
#endif

#define FN_USE_DOUBLES
#include "fastnoise.h"
#include "abyss_internal.h"


struct bitmap {
	pixel32 *pixels{nullptr};
	int32 width{0};
	int32 height{0};
};

struct texture {
	int32 ref{0};
	int32 width{0};
	int32 height{0};
};

bitmap *create_bitmap(int32 width,int32 height);
bitmap *create_bitmap(texture *tex);
void destroy_bitmap(bitmap *bmp);

texture *create_texture(int32 width,int32 height);
texture *create_texture(bitmap *bmp);
void destroy_texture(texture *tex);

int32 copy_texture(bitmap *targetbitmap, texture *sourcetexture);
int32 copy_bitmap(texture *targettexture, bitmap *sourcebitmap);

