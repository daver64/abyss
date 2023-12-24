
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
#define MAX_BUFFER_VERTICES (300000)

int32 create_array_buffer(array_buffer **db)
{
    (*db) = (array_buffer *)global_alloc(sizeof(array_buffer));
    if (!(*db))
        return ERROR_CREATE_DRAWBUFFER_DRAWBUFFER;

    (*db)->vertices = (vec3 *)global_alloc(MAX_BUFFER_VERTICES * sizeof(vec3));
    if (!(*db)->vertices)
    {
        global_free((*db));
        (*db) = nullptr;
        return ERROR_CREATE_DRAWBUFFER_VERTICES;
    }
    (*db)->normals = (vec3 *)global_alloc(MAX_BUFFER_VERTICES * sizeof(vec3));
    if (!(*db)->normals)
    {
        global_free((*db)->vertices);
        global_free((*db));
        (*db) = nullptr;
        return ERROR_CREATE_DRAWBUFFER_NORMALS;
    }
    (*db)->colours = (rgba *)global_alloc(MAX_BUFFER_VERTICES * sizeof(rgba));
    if (!(*db)->colours)
    {
        global_free((*db)->vertices);
        global_free((*db)->normals);
        global_free((*db));
        (*db) = nullptr;
        return ERROR_CREATE_DRAWBUFFER_COLOURS;
    }
    (*db)->texture_coordinates = (vec2 *)global_alloc(MAX_BUFFER_VERTICES * sizeof(vec2));
    if (!(*db)->texture_coordinates)
    {
        global_free((*db)->vertices);
        global_free((*db)->normals);
        global_free((*db)->colours);
        global_free((*db));
        (*db) = nullptr;
        return ERROR_CREATE_DRAWBUFFER_TEXTURECOORDS;
    }
    (*db)->array_capacity = MAX_BUFFER_VERTICES;
    (*db)->array_index = 0;
    (*db)->current_colour = rgba{1, 1, 1, 1};
    (*db)->current_normal = vec3{0, 1, 0};
    (*db)->current_vertex = vec3{0, 0, 0};
    (*db)->current_texture_coordinate = vec2{0, 0};
    (*db)->tex0_glref = 0;
    (*db)->tex1_glref = 0;
    (*db)->tex2_glref = 0;
    (*db)->tex3_glref = 0;
    glGenBuffers(1, &(*db)->glref);
    if ((*db)->glref < 1)
    {
        global_free((*db)->vertices);
        global_free((*db)->normals);
        global_free((*db)->colours);
        global_free((*db)->texture_coordinates);
        global_free((*db));
        (*db) = nullptr;
        return ERROR_CREATE_DRAWBUFFER_GLREF;
    }
    return CREATE_DRAWBUFFER_NO_ERROR;
}
int32 destroy_array_buffer(array_buffer **db)
{
    glDeleteBuffers(1, &(*db)->glref);
    global_free((*db)->vertices);
    global_free((*db)->normals);
    global_free((*db)->colours);
    global_free((*db)->texture_coordinates);
    global_free((*db));
    (*db) = nullptr;
    return DESTROY_DRAWBUFFER_NO_ERROR;
}
void begin_triangles(array_buffer *db)
{
    db->primitive = GL_TRIANGLES;
}
void begin_quads(array_buffer *db)
{
    db->primitive = GL_QUADS;
}
void begin_lines(array_buffer *db)
{
    db->primitive = GL_LINES;
}
void end_triangles(array_buffer *db)
{
    draw(db);
}
void end_quads(array_buffer *db)
{
    draw(db);
}
void end_lines(array_buffer *db)
{
    draw(db);
}
void vertex(array_buffer *db, vec3 vtx)
{
    db->current_vertex = vtx;
    db->vertices[db->array_index] = db->current_vertex;
    db->normals[db->array_index] = db->current_normal;
    db->colours[db->array_index] = db->current_colour;
    db->texture_coordinates[db->array_index] = db->current_texture_coordinate;
    db->array_index++;
    assert(db->array_index < db->array_capacity);
}
void rectangle(array_buffer *db, vec2 position, float32 width, float32 height, pixel32 pixel)
{
    vec3 v1 = vec3{position.x, position.y, 0};
    vec3 v2 = vec3{position.x + width, position.y, 0};
    vec3 v3 = vec3{position.x + width, position.y + height, 0};
    vec3 v4 = vec3{position.x, position.y + height, 0};
    quad(db, v1, v2, v3, v4, pixel, pixel, pixel, pixel);
}
void rectangle(array_buffer *db, float32 x, float32 y, float32 width, float32 height, pixel32 pixel)
{
    rectangle(db, vec2(x, y), width, height, pixel);
}
void triangle(array_buffer *db, vec3 v1, vec3 v2, vec3 v3, pixel32 pixel)
{
    colour(db, pixel);
    vertex(db, v1);
    vertex(db, v2);
    vertex(db, v3);
}
void quad(array_buffer *db, vec3 v1, vec3 v2, vec3 v3, vec3 v4, pixel32 pixel1, pixel32 pixel2, pixel32 pixel3, pixel32 pixel4)
{

    colour(db, pixel1);
    texture_coordinate(db, vec2{0, 0});
    vertex(db, v1);

    colour(db, pixel2);
    texture_coordinate(db, vec2{1, 0});
    vertex(db, v2);

    colour(db, pixel3);
    texture_coordinate(db, vec2{1, 1});
    vertex(db, v3);

    colour(db, pixel4);
    texture_coordinate(db, vec2{0, 1});
    vertex(db, v4);
}
void quad(array_buffer *db, vertexdata v1, vertexdata v2, vertexdata v3, vertexdata v4)
{
    colour(db, v1.colour);
    texture_coordinate(db, v1.texture_coordinates);
    vertex(db, v1.position);

    colour(db, v2.colour);
    texture_coordinate(db, v2.texture_coordinates);
    vertex(db, v2.position);

    colour(db, v3.colour);
    texture_coordinate(db, v3.texture_coordinates);
    vertex(db, v3.position);

    colour(db, v4.colour);
    texture_coordinate(db, v4.texture_coordinates);
    vertex(db, v4.position);
}
void triangle(array_buffer *db, vertexdata v1, vertexdata v2, vertexdata v3)
{
    colour(db, v1.colour);
    texture_coordinate(db, v1.texture_coordinates);
    vertex(db, v1.position);

    colour(db, v2.colour);
    texture_coordinate(db, v2.texture_coordinates);
    vertex(db, v2.position);

    colour(db, v3.colour);
    texture_coordinate(db, v3.texture_coordinates);
    vertex(db, v3.position);
}
void normal(array_buffer *db, vec3 nml)
{
    db->current_normal = nml;
}
void texture_coordinate(array_buffer *db, vec2 tc)
{
    db->current_texture_coordinate = tc;
}
rgba from_pixel32(pixel32 pixel)
{
    rgba cc;
    cc.r = getr_nf(pixel);
    cc.g = getg_nf(pixel);
    cc.b = getb_nf(pixel);
    cc.a = geta_nf(pixel);
    return cc;
}
pixel32 from_rgba(rgba pixel)
{
    assert(pixel.r >= 0 && pixel.r <= 1.0f);
    assert(pixel.g >= 0 && pixel.g <= 1.0f);
    assert(pixel.b >= 0 && pixel.b <= 1.0f);
    assert(pixel.a >= 0 && pixel.a <= 1.0f);

    uint8 r = pixel.r * 255;
    uint8 g = pixel.g * 255;
    uint8 b = pixel.b * 255;
    uint8 a = pixel.a * 255;
    return argb(a, r, g, b);
}
void colour(array_buffer *db, pixel32 pixel)
{
    rgba cc;
    cc.r = getr_nf(pixel);
    cc.g = getg_nf(pixel);
    cc.b = getb_nf(pixel);
    cc.a = geta_nf(pixel);
    db->current_colour = cc;
}
void colour(array_buffer *db, rgba pixel)
{
    db->current_colour = pixel;
}
void draw(array_buffer *db)
{
    glBindBuffer(GL_ARRAY_BUFFER, db->glref);
    const uint32 vert_size = (uint32)(sizeof(vec3) * db->array_index);
    const uint32 norm_size = (uint32)(sizeof(vec3) * db->array_index);
    const uint32 col_size = (uint32)(sizeof(vec4) * db->array_index);
    const uint32 tex0_coord_size = (uint32)(sizeof(vec2) * db->array_index);
    const uint32 tex1_coord_size = (uint32)(sizeof(vec2) * db->array_index);
    db->vert_offset = 0;
    db->norm_offset = db->vert_offset + vert_size;
    db->col_offset = db->norm_offset + norm_size;
    db->tex0_offset = db->col_offset + col_size;
    db->tex1_offset = db->tex0_offset + tex0_coord_size;

    glBufferData(GL_ARRAY_BUFFER, vert_size + norm_size + col_size + tex0_coord_size + tex1_coord_size, 0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, (int32)db->vert_offset, vert_size, db->vertices);
    glBufferSubData(GL_ARRAY_BUFFER, (int32)db->norm_offset, norm_size, db->normals);
    glBufferSubData(GL_ARRAY_BUFFER, (int32)db->col_offset, col_size, db->colours);

    if (db->tex0_glref)
    {
        glActiveTexture(GL_TEXTURE0);
        glClientActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, db->tex0_glref);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBufferSubData(GL_ARRAY_BUFFER, (GLint)db->tex0_offset, tex0_coord_size, db->texture_coordinates);
        glTexCoordPointer(2, GL_FLOAT, 0, (void *)db->tex0_offset);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glVertexPointer(3, GL_FLOAT, 0, (void *)db->vert_offset);
    glNormalPointer(GL_FLOAT, 0, (void *)db->norm_offset);
    glColorPointer(4, GL_FLOAT, 0, (void *)db->col_offset);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glDrawArrays(db->primitive, 0, (GLsizei)db->array_index);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    if (db->tex0_glref)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glClientActiveTexture(GL_TEXTURE0);
    }
    db->array_index = 0;
}

void reset(array_buffer *db)
{
    db->array_index = 0;
}