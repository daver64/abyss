/***
* Copyright 2004-2024, Dave Rowbotham and Toni Ylisirnio
* All rights reserved.
*/

#pragma once




#include "sl_internal.h"
#include "sl_maths.h"

struct ma_sound;
struct GLFWwindow;
struct GLFWvidmode;

//
// Textures
//
struct texture
{
	pixel32 *pixels{nullptr};
	uint32 ref{0};
	int32 width{0};
	int32 height{0};
};

struct textureatlas
{
	texture *tex{nullptr};
	int32 tile_width{0};
	int32 tile_height{0};
	int32 numtiles_x{0};
	int32 numtiles_y{0};
};


//
// Drawing Primitives
//
struct vertexdata 
{
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

struct gpu_shader 
{
	uint32 program;
	uint32 geometry;
	uint32 vertex;
	uint32 fragment;
};

struct soundobject
{
	ma_sound *sound;
};

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
int32 stop_sound(soundobject *sobj);
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
int32 create_windowed_context(context** ctx, const std::string& titletext,const int32 width=0,const int32 height=0);
void destroy_context(context **ctx);

// texture
int32 create_texture(texture **tex, const int32 width, const int32 height, const bool mipmapped = true);
int32 create_texture(texture **tex, const std::string &filename, const bool mipmapped = true);
void destroy_texture(texture **tex);
int32 save_texture(texture *tex, const std::string &filename);

// shader
void set_uniform2v(gpu_shader* shader, const char* name, int32 num_elements, float32* value);
void set_uniform3v(gpu_shader* shader, const char* name, int32 num_elements, float32* value);
void set_uniform4v(gpu_shader* shader, const char* name, int32 num_elements, float32* value);
void set_uniform_vec2(gpu_shader* shader, const char* name, vec2& value);
void set_uniform_vec3(gpu_shader* shader, const char* name, vec3& value);
void set_uniform1i(gpu_shader* shader, const char* name, int32 value);
void set_uniform_matrix4v(gpu_shader* shader, const char* name, int32 num_elements, mat4x4 value);
int32 create_shader(gpu_shader **shader,std::string &vertex_glsl,
	std::string &fragment_glsl, std::string &geometry_glsl);
void destroy_shader(gpu_shader **shader);
void bind(gpu_shader *shader);


// Software Drawing
void putpixel(texture *pixelbuffer, int32 x,int32 y, pixel32 colour);
pixel32 getpixel(texture *pixelbuffer,int32 x,int32 y);
void line(texture *pixelbuffer,int32 x1,int32 y1,int32 x2,int32 y2,pixel32 colour1,pixel32 colour2);
void rectangle(texture *pixelbuffer, int32 x,int32 y, int32 width,int32 height, pixel32 colour);
void clear(texture *pixelbuffer,pixel32 colour);
int32 pixelcopy(texture *destination, texture *source, ivec2 destination_origin,ivec2 source_origin, int32 width, int32 height);
rgba from_pixel32(pixel32 pixel);
pixel32 from_rgba(rgba pixel);

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
int32 create_atlas(textureatlas **atlas, const std::string &filename, int32 numx, int32 numy);
int32 create_atlas(textureatlas **atlas, int32 width, int32 height, int32 numx, int32 numy);
void destroy_atlas(textureatlas **atlas);
void draw_atlas_tile(array_buffer *target,textureatlas *atlas, float_t x1, float_t y1, float_t width, float_t height, int32 index, pixel32 colour = x11colours::white);
void draw_atlas_tile(array_buffer *target,textureatlas *atlas, int32 x1, int32 y1, int32 width, int32 height, int32 index, pixel32 colour = x11colours::white);
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

void ortho2d(int32 width, int32 height, bool flip = false, float32 near_z = -1.0f, float32 far_z = 1.0f);
void ortho2d(context *app_context, bool flip = false, float32 near_z = -1.0f, float32 far_z = 1.0f);



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
void textout(array_buffer *target,textureatlas *atlas, const char *text, int32 x, int32 y, pixel32 colour = x11colours::white);
void gprintf(array_buffer *target,textureatlas *atlas, float32 x, float32 y, pixel32 colour, const char* fmt, ...);

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