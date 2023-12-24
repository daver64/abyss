#pragma once


#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cassert>
#include <vector>

#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "glm/gtx/quaternion.hpp"
#include <xmmintrin.h>

#ifdef _WIN32
void load_gl_extensions();
#include "resource.h"
#endif

#define FN_USE_DOUBLES
#include "fastnoise.h"
#include "sl_internal.h"
#include "sl_maths.h"

//
// Colour Primitive
//
struct rgba {
	float32 r{1.0f};
	float32 g{1.0f};
	float32 b{1.0f};
	float32 a{1.0f};
};

rgba from_pixel32(pixel32 pixel);
pixel32 from_rgba(rgba pixel);
//
// Texture Handling
//
struct texture
{
	pixel32 *pixels{nullptr};
	uint32 ref{0};
	int32 width{0};
	int32 height{0};
};


class array_buffer;
struct textureatlas
{
	texture *tex{nullptr};
	array_buffer *target{nullptr};
	int32 tile_width{0};
	int32 tile_height{0};
	int32 numtiles_x{0};
	int32 numtiles_y{0};
};


//
// Drawing Primitives
//

struct vertexdata {
	vec3 position;
	vec3 normal;
	rgba colour;
	vec2 texture_coordinates;
};

struct array_buffer
{
    vec3 *vertices{nullptr};
    vec3 *normals{nullptr};
    rgba *colours{nullptr};
    vec2 *texture_coordinates{nullptr};

    vec3 current_vertex;
    vec3 current_normal;
    rgba current_colour;
    vec2 current_texture_coordinate;
    uint32 array_capacity{0};
    uint32 array_index{0};
    uint32 glref{0};
	int32 primitive{0};
	int32 tex0_glref{0};
	int32 tex1_glref{0};
	int32 tex2_glref{0};
	int32 tex3_glref{0};
    uint64 vert_offset{0};
    uint64 norm_offset{0};
    uint64 col_offset{0};
    uint64 tex0_offset{0};
    uint64 tex1_offset{0};
    uint64 tex2_offset{0};
    uint64 tex3_offset{0};
};


struct framebuffer
{
	int32 width{0};
	int32 height{0};
	uint32 glref{0};
	uint32 frame_id{0};
	uint32 render_id{0};
	uint32 depth_id{0};
	bool valid{false};
};

struct ma_sound;
struct soundobject
{
	ma_sound *sound;
};

struct GLFWwindow;
struct GLFWvidmode;
struct context
{
	GLFWwindow *window{nullptr};
	int32 width;
	int32 height;
	bool fullscreen;
	bool quit_requested;
};



//
// audio handling
//
int32 create_soundobject(soundobject **sobj,const std::string &filename);
void destroy_soundobject(soundobject **sobj);
int32 play_sound(soundobject *sobj);
int32 init_sound();
void deinit_sound();


//
// Memory
//
void *global_alloc(const uint32 numbytes,const char* description=nullptr);
void global_free(void *pointer);
void dump_global_allocs();

//
// Screen and IO Handling
//


// context
int32 create_fullscreen_context(context **ctx, const std::string &titletext);
int32 create_windowed_context(context** ctx, int32 width, int32 height, const std::string& titletext);

void destroy_context(context **ctx);

// texture
int32 create_texture(texture **tex, int32 width, int32 height, const bool mipmapped = true);
int32 create_texture(texture **tex, const std::string &filename, const bool mipmapped = true);
void destroy_texture(texture **tex);
int32 save_texture(texture *tex, const std::string &filename);

// Software Drawing
int32 putpixel(texture *pixelbuffer, ivec2 p, pixel32 colour);
int32 getpixel(texture *pixelbuffer, ivec2 p, pixel32 &colour);
int32 line(texture *pixelbuffer, ivec2 p1, ivec2 p2, pixel32 colour1, pixel32 colour2);
int32 rectangle(texture *pixelbuffer, ivec2 p, int32 width, int32 height,
	pixel32 colour1,pixel32 colour2, pixel32 colour3, pixel32 colour4);
int32 triangle(texture *pixelbuffer, ivec2 p1,ivec2 p2,ivec2 p3,
    pixel32 colour1,pixel32 colour2, pixel32 colour3);
int32 clear(texture *pixelbuffer, pixel32 colour);
int32 pixelcopy(texture *destination, texture *source, ivec2 destination_origin,ivec2 source_origin, int32 width, int32 height);


//
// GPU Drawing
//

int32 create_array_buffer(array_buffer **db);
int32 destroy_array_buffer(array_buffer **db);
void begin_triangles(array_buffer *db);
void begin_quads(array_buffer *db);
void begin_lines(array_buffer *db);
void end_triangles(array_buffer *db);
void end_quads(array_buffer *db);
void end_lines(array_buffer *db);
void vertex(array_buffer *db, vec3 vtx);
void normal(array_buffer *db, vec3 nml);
void texture_coordinate(array_buffer *db, vec2 tc);
void colour(array_buffer *db, pixel32 colour);
void colour(array_buffer *db, rgba colour);
void draw(array_buffer *db);


void rectangle(array_buffer *db, vec2 position, float32 width, float32 height, pixel32 colour);
void rectangle(array_buffer *db, float32 x,float32 y,float32 width,float32 height,pixel32 colour);
void triangle(array_buffer *db, vec3 v1,vec3 v2, vec3 v4, pixel32 colour);
void quad(array_buffer *db, vec3 v1, vec3 v2, vec3 v3, vec3 v4, pixel32 colour1,pixel32 colour2, pixel32 colour3, pixel32 colour4);


void quad(array_buffer *db, vertexdata v1, vertexdata v2,vertexdata v3, vertexdata v4);
void triangle(array_buffer *db, vertexdata v1,vertexdata v2,vertexdata v3);




//
// Framebuffer
//
int32 create_framebuffer(framebuffer **fb, int32 width, int32 height);
void destroy_framebuffer(framebuffer **fb);
void bind_framebuffer(framebuffer *fb);
void unbind_framebuffer(framebuffer *fb);

//
// Texture Atlas
//
int32 create_atlas(textureatlas **atlas, array_buffer *target, const std::string &filename, int32 numx, int32 numy);
int32 create_atlas(textureatlas **atlas, array_buffer *target, int32 width, int32 height, int32 numx, int32 numy);
void destroy_atlas(textureatlas **atlas);
void bind_atlas(textureatlas *atlas);
void begin_atlas(textureatlas *atlas);
void draw_atlas_tile(textureatlas *atlas, float_t x1, float_t y1, float_t width, float_t height, int32 index, pixel32 colour = x11colours::white);
void draw_atlas_tile(textureatlas *atlas, int32 x1, int32 y1, int32 width, int32 height, int32 index, pixel32 colour = x11colours::white);
void end_atlas(textureatlas *atlas);
int32 putpixel(textureatlas *atlas, ivec2 p, int32 index, pixel32 colour);
int32 getpixel(textureatlas *atlas, ivec2 p, int32 index, pixel32 &colour);
int32 line(textureatlas *atlas, ivec2 p1, ivec2 p2, int32 index, pixel32 colour1, pixel32 colour2);
int32 rectangle(textureatlas *atlas, ivec2 p, int32 width, int32 height, int32 index,
	pixel32 colour1,pixel32 colour2, pixel32 colour3, pixel32 colour4);
int32 triangle(textureatlas *atlas, ivec2 p1,ivec2 p2,ivec2 p3, int32 index,
    pixel32 colour1,pixel32 colour2, pixel32 colour3);
int32 clear(textureatlas *atlas, int32 index, pixel32 colour);

//
// Graphics Pipeline State
//
void clear_screen();
void clear_screen(pixel32 colour);
void set_clear_colour(pixel32 colour);
void enable_multisampling();
void disable_multisampling();
void enable_texturing();
void disable_texturing();
void enable_arrays();
void disable_arrays();
void enable_texture_clamping();
void disable_texture_clamping();
void enable_mipmapping();
void disable_mipmapping();
void enable_texture_filtering();
void disable_texture_filtering();
void enable_blending();
void disable_blending();
void texture_gpu_write(texture *tex);
void texture_gpu_read(texture *tex);
void texture_bind(texture *tex);
void texture_unbind();
void disable_depthtest();
void enable_depthtest();
void swap(context *ctx);

void ortho2d(int32 width, int32 height, bool flip, float32 near_z, float32 far_z);



//
// input
//
void poll_input(context *ctx);
void process_input(context *ctx);

//
// Timing
//

void sleep_in_ms(uint32 ms);
void sleep_in_us(uint32 us);

//
// RNG
//

int32 random_int(int32 start=INT32_MIN, int32 end=INT32_MAX);
float32 random_float(float32 start=FLT_MIN, float32 end=FLT_MAX);

//
// texture drawing. software
//
void end_quads(array_buffer *target);
void begin_quads(array_buffer *target);
void draw_rectangle(array_buffer *target,
					float32 x, float32 y,
					float32 width, float32 height,
					pixel32 colour);
void bind_texture(array_buffer *db, texture *tex);
void textout(textureatlas *atlas, const char *text, int32 x, int32 y, pixel32 colour = x11colours::white);
void gprintf(textureatlas *atlas, float32 x, float32 y, pixel32 colour, const char* fmt, ...);

//
// keyboard  -- experimental api atm
//
void keyboard_install(context *ctx);
uint32 getkey(uint32 k); // very very temporary...
vec2 get_mouse_position();

//
// Utility
//
bool want_to_quit(context *ctx);
void app_quit(context *ctx);
const float64 get_frame_delta_t_ms();

std::string load_text_file(const std::string &filename);
std::vector<std::string> load_text_file_lines(const std::string &filename);
ubyte *load_binary_file(const std::string &filename);
std::vector<std::string> split_string(const std::string &str,const std::string &delims,
	std::vector<std::string> &elems,bool skip_empty=true);


//
// GUI
//
int32 init_gui(context *app_context);
void deinit_gui();
void draw_gui();