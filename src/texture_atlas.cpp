#include "../include/abyss.h"

#include <GL/glew.h>
#include <SDL2/SDL_image.h>

#include <cstdio>

bool load_texture_atlas(const std::string &filename, TextureAtlas &atlas) {
    enum { INIT_FLAGS = IMG_INIT_PNG };
    if ((IMG_Init(INIT_FLAGS) & INIT_FLAGS) != INIT_FLAGS) {
        SDL_Log("IMG_Init failed: %s", IMG_GetError());
        return false;
    }

    SDL_Surface *surface = IMG_Load(filename.c_str());
    if (!surface) {
        SDL_Log("IMG_Load failed for %s: %s", filename.c_str(), IMG_GetError());
        return false;
    }

    // Determine texture format based on surface format
    GLenum format = GL_RGBA;
    if (surface->format->BytesPerPixel == 3) {
        format = GL_RGB;
    } else if (surface->format->BytesPerPixel == 4) {
        format = GL_RGBA;
    }

    // Create OpenGL texture
    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Upload pixel data
    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format,
                 GL_UNSIGNED_BYTE, surface->pixels);

    // Set texture filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Store atlas info
    atlas.gl_texture_id = texture_id;
    atlas.width = surface->w;
    atlas.height = surface->h;
    atlas.filepath = filename;

    SDL_FreeSurface(surface);

    std::printf("Loaded texture atlas '%s': %dx%d (GL texture ID: %u)\n",
                filename.c_str(), atlas.width, atlas.height, atlas.gl_texture_id);

    return true;
}

bool unload_texture_atlas(TextureAtlas &atlas) {
    if (atlas.gl_texture_id != 0) {
        glDeleteTextures(1, &atlas.gl_texture_id);
        atlas.gl_texture_id = 0;
    }
    atlas.width = 0;
    atlas.height = 0;
    atlas.filepath.clear();
    IMG_Quit();
    return true;
}

