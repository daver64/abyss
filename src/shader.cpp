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

#include "sl.h"

int32 create_shader(gpu_shader **shader, std::string &vertex_glsl,
                    std::string &fragment_glsl, std::string &geometry_glsl)
{
    (*shader) = (gpu_shader *)global_alloc(sizeof(gpu_shader), "shader object");

    std::string vertex_source = load_text_file(vertex_glsl);
    std::string fragment_source = load_text_file(fragment_glsl);

    char *infobuffer{nullptr};
    int32 success = 0;
    int32 buflen = 0;
    int32 infolen = 0;

    (*shader)->vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource((*shader)->vertex, 1, (const char **)vertex_source.c_str(), 0);
    glCompileShader((*shader)->vertex);
    glGetShaderiv((*shader)->vertex, GL_COMPILE_STATUS, &success);
    glGetShaderiv((*shader)->vertex, GL_INFO_LOG_LENGTH, &buflen);
    infobuffer = (char *)global_alloc(buflen + 1, "vertex shader source");
    glGetShaderInfoLog((*shader)->vertex, buflen, &infolen, infobuffer);
    fprintf(stderr, "Vertex Shader:%s\n%s\n", vertex_glsl.c_str(), infobuffer);

    success = 0;
    buflen = 0;
    infolen = 0;
    global_free(infobuffer);

    (*shader)->fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource((*shader)->fragment, 1, (const char **)fragment_source.c_str(), 0);
    glCompileShader((*shader)->fragment);
    glGetShaderiv((*shader)->fragment, GL_COMPILE_STATUS, &success);
    glGetShaderiv((*shader)->fragment, GL_INFO_LOG_LENGTH, &buflen);
    infobuffer = (char *)global_alloc(buflen + 1, "vertex shader source");
    glGetShaderInfoLog((*shader)->vertex, buflen, &infolen, infobuffer);
    fprintf(stderr, "Fragment Shader:%s\n%s\n", fragment_glsl.c_str(), infobuffer);

    success = 0;
    buflen = 0;
    infolen = 0;
    global_free(infobuffer);

    (*shader)->geometry = 0;

    (*shader)->program = glCreateProgram();
    glAttachShader((*shader)->program, (*shader)->fragment);
    glAttachShader((*shader)->program, (*shader)->vertex);
    glLinkProgram((*shader)->program);

    return 0;
}
void destroy_shader(gpu_shader **shader)
{
    glUseProgram(0);
    glDetachShader((*shader)->program, (*shader)->vertex);
    glDetachShader((*shader)->program, (*shader)->fragment);
    glDetachShader((*shader)->program, (*shader)->geometry);
    glDeleteShader((*shader)->vertex);
    glDeleteShader((*shader)->fragment);
    glDeleteShader((*shader)->geometry);
    global_free((*shader));
    (*shader) = nullptr;
}
void bind(gpu_shader *shader)
{
    glUseProgram(shader->program);
}
void unbind(gpu_shader *shader)
{
    glUseProgram(0);
}
int32 gl_set_uniform_float(gpu_shader *shader, const char *uniform, float32 var)
{
    int32 loc = glGetUniformLocation(shader->program, uniform);
    if (loc != -1)
    {
        glUniform1f(loc, var);
    }
    return loc;
}

void gl_set_uniform2v(gpu_shader *shader, const char *name, int32 num_elements, float32 *value)
{
    GLint uniform_loc = glGetUniformLocation(shader->program, name);
    glUniform2fv(uniform_loc, num_elements, value);
}
void gl_set_uniform3v(gpu_shader *shader, const char *name, int32 num_elements, float32 *value)
{
    GLint uniform_loc = glGetUniformLocation(shader->program, name);
    glUniform3fv(uniform_loc, num_elements, value);
}
void gl_set_uniform4v(gpu_shader *shader, const char *name, int32 num_elements, float32 *value)
{
    GLint uniform_loc = glGetUniformLocation(shader->program, name);
    glUniform4fv(uniform_loc, num_elements, value);
}
void gl_set_uniform_vec2(gpu_shader *shader, const char *name, vec2 &value)
{
    float32 v[2]{value.x, value.y};
    gl_set_uniform2v(shader, name, 1, &v[0]);
}
void gl_set_uniform_vec3(gpu_shader *shader, const char *name, vec3 &value)
{
    float32 v[3]{value.x, value.y, value.z};
    gl_set_uniform3v(shader, name, 1, &v[0]);
}
void gl_set_uniform1i(gpu_shader *shader, const char *name, int32 value)
{
    GLint uniform_loc = glGetUniformLocation(shader->program, name);
    glUniform1i(uniform_loc, value);
}
void gl_set_uniform_matrix4v(gpu_shader *shader, const char *name, int32 num_elements, mat4x4 value)
{
    GLint uniform_loc = glGetUniformLocation(shader->program, name);
    glUniformMatrix4fv(uniform_loc, num_elements, GL_FALSE, (const GLfloat *)&value);
}
