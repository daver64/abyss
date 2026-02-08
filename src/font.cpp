#include "../include/abyss.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdarg>
#include <cstring>

extern int g_window_width;
extern int g_window_height;

namespace {
    TTF_Font *g_font = nullptr;
    GLuint g_font_texture = 0;
    GLuint g_font_vao = 0;
    GLuint g_font_vbo = 0;
    GLuint g_font_ebo = 0;
    GLuint g_font_shader = 0;
    int g_font_width = 0;
    int g_font_height = 0;

    // Find a monospace font on the system
    std::string find_monospace_font()
    {
        // Try common Linux monospace font paths
        const char *font_paths[] = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
            "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
            "/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf",
            "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf",
        };
        
        for (const auto &path : font_paths) {
            FILE *f = fopen(path, "r");
            if (f) {
                fclose(f);
                return path;
            }
        }
        
        return "";
    }

    const char *font_vs_source = R"(
        #version 410 core
        layout (location = 0) in vec2 position;
        layout (location = 1) in vec2 texCoord;
        
        uniform mat4 projection;
        uniform mat4 model;
        
        out VS_OUT {
            vec2 tex_coord;
        } vs_out;
        
        void main()
        {
            gl_Position = projection * model * vec4(position, 0.0, 1.0);
            vs_out.tex_coord = texCoord;
        }
    )";
    
    const char *font_fs_source = R"(
        #version 410 core
        layout (location = 0) out vec4 FragColor;
        
        in VS_OUT {
            vec2 tex_coord;
        } fs_in;
        
        uniform sampler2D tex_sampler;
        uniform vec4 text_colour;
        
        void main()
        {
            vec4 tex = texture(tex_sampler, fs_in.tex_coord);
            FragColor = vec4(text_colour.rgb, tex.a);
        }
    )";

    bool compile_font_shader(GLuint shader, const char *source)
    {
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        
        int success;
        char info_log[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, info_log);
            SDL_Log("Font shader compilation failed: %s", info_log);
            return false;
        }
        return true;
    }

    bool init_font_shader()
    {
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        if (!compile_font_shader(vertex_shader, font_vs_source)) {
            glDeleteShader(vertex_shader);
            return false;
        }
        
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        if (!compile_font_shader(fragment_shader, font_fs_source)) {
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            return false;
        }
        
        g_font_shader = glCreateProgram();
        glAttachShader(g_font_shader, vertex_shader);
        glAttachShader(g_font_shader, fragment_shader);
        glLinkProgram(g_font_shader);
        
        int success;
        char info_log[512];
        glGetProgramiv(g_font_shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(g_font_shader, 512, nullptr, info_log);
            SDL_Log("Font shader linking failed: %s", info_log);
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            return false;
        }
        
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        

        return true;
    }

    bool init_font_geometry()
    {
        // Vertices for a unit quad (0,0 to 1,1)
        float vertices[] = {
            // positions      // texture coords
            0.0f, 0.0f,      0.0f, 1.0f,
            1.0f, 0.0f,      1.0f, 1.0f,
            1.0f, 1.0f,      1.0f, 0.0f,
            0.0f, 1.0f,      0.0f, 0.0f
        };
        
        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
        };
        
        glGenVertexArrays(1, &g_font_vao);
        glGenBuffers(1, &g_font_vbo);
        glGenBuffers(1, &g_font_ebo);
        
        glBindVertexArray(g_font_vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, g_font_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_font_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        

        return true;
    }
}

bool load_font(const std::string &font_path, int font_size)
{
    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        return false;
    }
    
    std::string path = font_path;
    if (path.empty()) {
        path = find_monospace_font();
        if (path.empty()) {
            SDL_Log("Could not find a monospace font on the system");
            return false;
        }
    }
    
    g_font = TTF_OpenFont(path.c_str(), font_size);
    if (!g_font) {
        SDL_Log("Failed to load font from '%s': %s", path.c_str(), TTF_GetError());
        return false;
    }
    
    // Get font dimensions from a sample character
    int advance;
    TTF_GlyphMetrics(g_font, 'M', nullptr, nullptr, nullptr, nullptr, &advance);
    g_font_width = advance;
    g_font_height = TTF_FontHeight(g_font);
    
    if (!init_font_shader() || !init_font_geometry()) {
        TTF_CloseFont(g_font);
        g_font = nullptr;
        return false;
    }
    

    return true;
}

bool unload_font()
{
    if (g_font) {
        TTF_CloseFont(g_font);
        g_font = nullptr;
    }
    
    if (g_font_texture) {
        glDeleteTextures(1, &g_font_texture);
        g_font_texture = 0;
    }
    
    if (g_font_shader) {
        glDeleteProgram(g_font_shader);
        g_font_shader = 0;
    }
    
    if (g_font_vao) {
        glDeleteVertexArrays(1, &g_font_vao);
        g_font_vao = 0;
    }
    
    if (g_font_vbo) {
        glDeleteBuffers(1, &g_font_vbo);
        g_font_vbo = 0;
    }
    
    if (g_font_ebo) {
        glDeleteBuffers(1, &g_font_ebo);
        g_font_ebo = 0;
    }
    
    TTF_Quit();
    return true;
}

int gprintf(int x, int y, RGBA colour, const char *fmt, ...)
{
    if (!g_font || !g_font_shader || !g_font_vao) {
        return 0;
    }
    
    // Format the text
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    

    
    // Render text to surface - use Blended for proper RGBA with antialiasing
    SDL_Color text_color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(g_font, buffer, text_color);
    if (!surface) {
        SDL_Log("Failed to render text: %s", TTF_GetError());
        return len;
    }
    

    
    // Convert to a standard RGBA format to avoid any byte-order confusion
    SDL_PixelFormat *target_fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    SDL_Surface *rgba_surface = SDL_ConvertSurface(surface, target_fmt, 0);
    SDL_FreeFormat(target_fmt);
    SDL_FreeSurface(surface);
    
    if (!rgba_surface) {
        SDL_Log("gprintf: Failed to convert to standard RGBA32: %s", SDL_GetError());
        return len;
    }
    
    
    // Determine the correct GL format based on SDL surface format
    GLenum gl_format = GL_RGBA;
    
    // ABGR8888 in memory is [B, G, R, A], so we need GL_BGRA to upload it correctly
    if (rgba_surface->format->format == SDL_PIXELFORMAT_ABGR8888) {
        gl_format = GL_BGRA;  // Byte layout in memory is B,G,R,A
    } else if (rgba_surface->format->format == SDL_PIXELFORMAT_RGBA32) {
        gl_format = GL_RGBA;
    } else {
        gl_format = GL_RGBA;  // Default
    }
    

    
    // Create OpenGL texture from surface
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Set pixel store parameters to handle SDL surface pitch (row alignment)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Minimal alignment to handle any pitch
    
    // Copy the surface data to a tightly-packed buffer to avoid pitch issues
    int pixel_count = rgba_surface->w * rgba_surface->h;
    unsigned char *packed_data = new unsigned char[pixel_count * 4];
    unsigned char *src = (unsigned char*)rgba_surface->pixels;
    unsigned char *dst = packed_data;
    
    for (int y = 0; y < rgba_surface->h; y++) {
        memcpy(dst, src, rgba_surface->w * 4);
        dst += rgba_surface->w * 4;
        src += rgba_surface->pitch;
    }
    

    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rgba_surface->w, rgba_surface->h, 0, gl_format, GL_UNSIGNED_BYTE, packed_data);
    
    delete[] packed_data;  // Clean up temporary buffer
    
    // Reset pixel store to defaults
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Draw the text
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(g_window_width), 
                                      static_cast<float>(g_window_height), 0.0f, 
                                      -1.0f, 1.0f);
    
    // Clear any existing errors
    while (glGetError() != GL_NO_ERROR) {}
    
    // Use shader program
    glUseProgram(g_font_shader);
    
    // Set uniforms BEFORE binding VAO
    GLint proj_loc = glGetUniformLocation(g_font_shader, "projection");
    GLint model_loc = glGetUniformLocation(g_font_shader, "model");
    GLint colour_loc = glGetUniformLocation(g_font_shader, "text_colour");
    GLint tex_loc = glGetUniformLocation(g_font_shader, "tex_sampler");
    
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1i(tex_loc, 0);
    glUniform4f(colour_loc, colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, colour.a / 255.0f);
    
    // Create temporary VAO and VBO for this text
    GLuint temp_vao, temp_vbo;
    glGenVertexArrays(1, &temp_vao);
    glGenBuffers(1, &temp_vbo);
    
    glBindVertexArray(temp_vao);
    
    // IMPORTANT: Unbind any element buffer so glDrawArrays works
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, temp_vbo);
    
    // Vertex data for a quad as 2 triangles with correct CCW winding
    // Triangle 1: v0, v1, v2 (top-left, top-right, bottom-right)
    // Triangle 2: v0, v2, v3 (top-left, bottom-right, bottom-left)
    float vertices[] = {
        // Triangle 1
        0.0f, 0.0f,    0.0f, 0.0f,     // v0: top-left
        1.0f, 0.0f,    1.0f, 0.0f,     // v1: top-right
        1.0f, 1.0f,    1.0f, 1.0f,     // v2: bottom-right
        // Triangle 2
        0.0f, 0.0f,    0.0f, 0.0f,     // v0: top-left
        1.0f, 1.0f,    1.0f, 1.0f,     // v2: bottom-right
        0.0f, 1.0f,    0.0f, 1.0f      // v3: bottom-left
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(rgba_surface->w, rgba_surface->h, 1.0f));
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisable(GL_BLEND);
    glBindVertexArray(0);
    glUseProgram(0);
    
    // Clean up temporary resources
    glDeleteBuffers(1, &temp_vbo);
    glDeleteVertexArrays(1, &temp_vao);
    glDeleteTextures(1, &texture);
    SDL_FreeSurface(rgba_surface);
    
    return len;
}
