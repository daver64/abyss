#include "../include/abyss.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TextureAtlas tile_atlas;

namespace {
    // Shader program for rendering textured quads
    GLuint g_shader_program = 0;
    GLuint g_quad_vao = 0;
    GLuint g_quad_vbo = 0;
    
    // Window dimensions (updated externally)
    int g_window_width = 800;
    int g_window_height = 600;
    
    const char *vs_source = R"(
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
    
    const char *fs_source = R"(
        #version 410 core
        layout (location = 0) out vec4 FragColor;
        
        in VS_OUT {
            vec2 tex_coord;
        } fs_in;
        
        uniform sampler2D tex_sampler;
        
        void main()
        {
            FragColor = texture(tex_sampler, fs_in.tex_coord);
        }
    )";
    
    bool init_quad_geometry()
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
        
        glGenVertexArrays(1, &g_quad_vao);
        glGenBuffers(1, &g_quad_vbo);
        
        GLuint ebo;
        glGenBuffers(1, &ebo);
        
        glBindVertexArray(g_quad_vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, g_quad_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // Position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Texture coordinate attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        return true;
    }
    
    bool compile_shader(GLuint shader, const char *source)
    {
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        
        int success;
        char info_log[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, info_log);
            SDL_Log("Shader compilation failed: %s", info_log);
            return false;
        }
        return true;
    }
    
    bool init_shaders()
    {
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        if (!compile_shader(vertex_shader, vs_source)) {
            glDeleteShader(vertex_shader);
            return false;
        }
        
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        if (!compile_shader(fragment_shader, fs_source)) {
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            return false;
        }
        
        g_shader_program = glCreateProgram();
        glAttachShader(g_shader_program, vertex_shader);
        glAttachShader(g_shader_program, fragment_shader);
        glLinkProgram(g_shader_program);
        
        int success;
        char info_log[512];
        glGetProgramiv(g_shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(g_shader_program, 512, nullptr, info_log);
            SDL_Log("Shader linking failed: %s", info_log);
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            return false;
        }
        
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        
        return true;
    }
    
    void setup_rendering_state()
    {
        static bool initialized = false;
        if (!initialized) {
            init_shaders();
            init_quad_geometry();
            initialized = true;
        }
    }
}

void draw_atlas_tile(const TextureAtlas &atlas, int tile_index, int tile_width, int tile_height, int screen_x, int screen_y, float z, float scale)
{
    setup_rendering_state();
    
    if (g_shader_program == 0 || g_quad_vao == 0 || atlas.gl_texture_id == 0) {
        return;
    }
    
    // Calculate texture coordinates based on tile_index
    int tiles_per_row = atlas.width / tile_width;
    int tile_row = tile_index / tiles_per_row;
    int tile_col = tile_index % tiles_per_row;
    
    float tex_x = (tile_col * tile_width) / static_cast<float>(atlas.width);
    float tex_y = (tile_row * tile_height) / static_cast<float>(atlas.height);
    float tex_width = tile_width / static_cast<float>(atlas.width);
    float tex_height = tile_height / static_cast<float>(atlas.height);
    
    // Set up projection matrix (2D orthographic)
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(g_window_width), 
                                      static_cast<float>(g_window_height), 0.0f, 
                                      -1.0f, 1.0f);
    
    // Set up model matrix (position and scale)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(screen_x, screen_y, z));
    model = glm::scale(model, glm::vec3(tile_width * scale, tile_height * scale, 1.0f));

    // Reset color to white to avoid texture tinting
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Use shader program
    glUseProgram(g_shader_program);
    
    // Set uniforms
    GLint proj_loc = glGetUniformLocation(g_shader_program, "projection");
    GLint model_loc = glGetUniformLocation(g_shader_program, "model");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    
    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas.gl_texture_id);
    GLint tex_loc = glGetUniformLocation(g_shader_program, "tex_sampler");
    glUniform1i(tex_loc, 0);
    
    // Bind VAO and draw
    glBindVertexArray(g_quad_vao);
    
    // Update texture coordinates in the VBO
    // We need to modify the quad vertices to use the correct texture coordinates
    float quad_vertices[] = {
        // positions      // texture coords
        0.0f, 0.0f,      tex_x, tex_y + tex_height,
        1.0f, 0.0f,      tex_x + tex_width, tex_y + tex_height,
        1.0f, 1.0f,      tex_x + tex_width, tex_y,
        0.0f, 1.0f,      tex_x, tex_y
    };
    
    glBindBuffer(GL_COPY_WRITE_BUFFER, g_quad_vbo);
    glBufferSubData(GL_COPY_WRITE_BUFFER, 0, sizeof(quad_vertices), quad_vertices);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    glUseProgram(0);
}


bool draw_level(const Level &level, float scale)
{
    bool success = true;

    // Draw tiles
    for(int y = 0; y < level.height; ++y)
    {
        for(int x = 0; x < level.width; ++x)
        {
            Tile &tile = get_tile(const_cast<Level&>(level), x, y);
            int tile_index = static_cast<int>(tile.type) * 8 + static_cast<int>(tile.feature);
            draw_atlas_tile(tile_atlas, tile_index, 16, 16, static_cast<int>(x * 16 * scale), static_cast<int>(y * 16 * scale), 0.0f, scale);
        }
    }

    // Draw entities
    for(const auto &entity : level.entities)
    {
        if(entity.type == EntityType::PLAYER)
        {
            draw_atlas_tile(tile_atlas, 39, 16, 16, static_cast<int>(entity.x * 16 * scale), static_cast<int>(entity.y * 16 * scale), -0.5f, scale);
        }
    }

    return success;
}
