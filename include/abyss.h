#pragma once

#include <cstdint>
#include <string>
#include <cassert>
#include <vector>

#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "glm/gtx/quaternion.hpp"
#include <xmmintrin.h>
#include <AL/al.h>
#include <AL/alc.h>
#ifdef _WIN32
void load_gl_extensions();
#include "resource.h"
#endif

#define FN_USE_DOUBLES
#include "fastnoise.h"
#include "abyss_internal.h"

extern const float64 PI;
extern const float64 TAU;
extern const float64 PIDIV2;
extern float64 ROUNDING_ERROR;

extern const float32 PIf;
extern const float32 TAUf;
extern const float32 PIDIV2f;
extern float32 ROUNDING_ERRORf;

inline float32 lerp(float32 v1, float32 v2, float32 a) { return (v1 * (1.0f - a) + v2 * a); }
inline float64 lerp(float64 v1, float64 v2, float64 a) { return (v1 * (1.0 - a) + v2 * a); }
inline float64 degtorad(float64 val) { return val * 0.0174532925; }
inline float32 degtorad(float32 val) { return val * 0.0174532925f; }
inline float64 radtodeg(float64 val) { return val * 57.2957795; }
inline float32 radtodeg(float32 val) { return val * 57.2957795f; }
bool get_line_intersection(float32 p0_x, float32 p0_y,
						   float32 p1_x, float32 p1_y,
						   float32 p2_x, float32 p2_y,
						   float32 p3_x, float32 p3_y,
						   float32 &i_x, float32 &i_y);
vec2 get_line_intersection(vec2 v0, vec2 v1, vec2 v2, vec2 v3, bool &success);
enum EIntersectionRelation3D
{
	ISREL3D_FRONT = 0,
	ISREL3D_BACK,
	ISREL3D_PLANAR,
	ISREL3D_SPANNING,
	ISREL3D_CLIPPED
};
class polar
{
public:
	union
	{
		float32 d[4] = {0, 0, 0, 0};
		struct
		{
			float32 ra, dec, rad, w;
		};
		struct
		{
			float32 lon, lat, alt, p;
		};
	};
	polar(const float32 &lon = 0,
		  const float32 &lat = 0,
		  const float32 &alt = 0);
	polar(const float32 p[3]);
	polar(const vec3 &pos);
	float32 &operator[](size_t index);
	vec3 polar_to_cartesian(const polar &pos);
	polar cartesian_to_polar(const vec3 &pos);
	vec3 to_cartesian();
	void from_cartesian(const vec3 &pos);
};

class plane
{
public:
	plane();
	~plane();
	EIntersectionRelation3D classify_point_relation(
		const vec3 &point) const
	{
		const float32 d = glm::dot(normal, point) + D;
		if (d < -ROUNDING_ERROR)
			return ISREL3D_BACK;
		if (d > ROUNDING_ERROR)
			return ISREL3D_FRONT;
		return ISREL3D_PLANAR;
	}
	void set_plane(
		const vec3 &point1, const vec3 &point2, const vec3 &point3)
	{
		// normal = (point2 - point1).cross(point3 - point1);
		normal = glm::cross((point2 - point1), (point3 - point1));
		// normal.normalise();
		glm::normalize(normal);
		recalculate(point1);
	}
	void recalculate(const vec3 &point)
	{
		// D = -point.dot(normal);
		D = glm::dot(point, normal);
	}
	vec3 normal;
	float32 D;
};

class camera
{
public:
	void set_clip(float32 near,float32 far);
	void zoom_out(float32 angle);
	void zoom_in(float32 angle);
	void roll(float32 angle);
	void yaw(float32 angle);
	void pitch(float32 angle);
	void move(const vec3& direction);
	void set_position(const vec3& newposition);
	mat4x4 view() const;
	mat4x4 projection(float32 aspectratio) const;
	vec3 down_vector() const;
	vec3 up_vector() const;
	vec3 right_vector() const;
	vec3 left_vector() const;
	vec3 forward_vector() const;
	vec3 back_vector() const;

	vec3 position{0.0f,0.0f,5.0f};
	quat rotation{1.0f,0.0f,0.0f,0.0f};
	float32 fov{glm::radians(60.0f)};
	float32 znear{0.1f};
	float32 zfar{100.0f};
};
//
// Platform API
//

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
class drawbuffer;
struct textureatlas
{
	texture *tex{nullptr};
	drawbuffer *target{nullptr};
	int32 tile_width{0};
	int32 tile_height{0};
	int32 numtiles_x{0};
	int32 numtiles_y{0};
};

int32 create_atlas(textureatlas **atlas, drawbuffer *target, const std::string &filename, int32 numx, int32 numy);
int32 destroy_atlas(textureatlas **atlas);
void bind_atlas(textureatlas *atlas);
void begin_atlas(textureatlas *atlas);
void draw_atlas_tile(textureatlas *atlas, float_t x1, float_t y1, float_t width, float_t height, int32 index, pixel32 colour = x11colours::white);
void draw_atlas_tile(textureatlas *atlas, int32 x1, int32 y1, int32 width, int32 height, int32 index, pixel32 colour = x11colours::white);
void end_atlas(textureatlas *atlas);

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


void texture_gpu_write(texture *tex);
void texture_gpu_read(texture *tex);
void texture_bind(texture *tex);

void texture_unbind();

//
// Drawing Primitives
//
class drawbuffer
{
public:
	// drawbuffer(GLenum primitive = GL_TRIANGLES, GLenum usage = GL_DYNAMIC_DRAW);
	drawbuffer(bool triangles = true, bool dynamic = true);
	~drawbuffer();
	void vertex(vec3 v);
	void vertex(vec2 v);
	void vertex(float32 x, float32 y, float32 z);
	void normal(vec3 n);
	void normal(float32 nx, float32 ny, float32 nz);
	void texcoord(vec2 t);
	void texcoord(float32 u, float32 v);
	void texcoord0(vec2 t);
	void texcoord1(vec2 t);
	void texcoord2(vec2 t);
	void texcoord3(vec2 t);
	void texcoord0(float32 u, float32 v);
	void texcoord1(float32 u, float32 v);
	void texcoord2(float32 u, float32 v);
	void texcoord3(float32 u, float32 v);
	const vec3 get_vertex(int32 index);
	vec2 get_vertex2(int32 index);
	vec3 get_normal(int32 index);
	vec2 get_texcoord(int32 index, int32 texture_index = 0);
	void colour(pixel32 c);
	void colour(float32 r, float32 g, float32 b, float32 a);
	// void colour(RGBAf c);
	void point(float32 x1, float32 y1, pixel32 colour);
	void line(float32 x1, float32 y1, float32 x2, float32 y2, pixel32 colour);
	void line(float32 x1, float32 y1, float32 x2, float32 y2, pixel32 colours[2]);
	void rect(float32 x1, float32 y1, float32 width, float32 height, pixel32 colour);
	void rect(float32 x1, float32 y1, float32 width, float32 height, pixel32 colours[4]);
	void circle(float32 x, float32 y, float32 radius, pixel32 colour, int32 numsegments);
	void ellipse(float32 x, float32 y, float32 width, float32 height, pixel32 ccolour, int32 numsegments);
	void triangle(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, pixel32 colours[3]);
	void quad(vec3 p1, vec3 p2, vec3 p3, vec3 p4);
	void quadratic_bezier(vec2 startpos, vec2 controlpos, vec2 endpos, int32_t numseg);
	void draw_sphere_patch(float32 slon, float32 slat,
						   float32 elon, float32 elat,
						   int subdivide, int ysubdivide,
						   float32 radius, float32 texture_width, float32 texture_height);
	void sphere(float32 radius, float32 texture_width, float32 texture_height);
	// std::vector<Line3<float32> > get_lines();
	void alphasort();
	void build();
	void draw();
	void reset();
	void begin(GLenum type);
	void end();
	void begin_quads();
	void begin_triangles();
	void begin_lines();
	vec3 *vertex_pointer(int32 index);
	vec3 *normal_pointer(int32 index);
	vec4 *colour_pointer(int32 index);
	vec2 *texcoord0_pointer(int32 index);
	vec2 *texcoord1_pointer(int32 index);
	vec2 *texcoord2_pointer(int32 index);
	vec2 *texcoord3_pointer(int32 index);
	void set_active_texture_unit(int32 unit);
	int32 get_active_texture_unit();
	void bind_tex0(int32 gl) { tex0_glref = gl; }
	void bind_tex1(int32 gl) { tex1_glref = gl; }
	void bind_tex2(int32 gl) { tex2_glref = gl; }
	void bind_tex3(int32 gl) { tex3_glref = gl; }

	GLenum primitive;
	GLenum usage;
	vec3 current_vertex;
	vec3 current_normal;
	vec4 current_colour;
	vec2 current_texcoord0;
	vec2 current_texcoord1;
	vec2 current_texcoord2;
	vec2 current_texcoord3;

	GLuint vbo_id = 0;
	uint64 vert_offset = 0;
	uint64 norm_offset = 0;
	uint64 col_offset = 0;
	uint64 tex0_offset = 0;
	uint64 tex1_offset = 0;
	uint64 tex2_offset = 0;
	uint64 tex3_offset = 0;
	int32 active_texture_unit = 0;
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec4> colours;
	std::vector<vec2> vtexcoord0;
	std::vector<vec2> vtexcoord1;
	std::vector<vec2> vtexcoord2;
	std::vector<vec2> vtexcoord3;

	int32 tex0_glref = 0;
	int32 tex1_glref = 0;
	int32 tex2_glref = 0;
	int32 tex3_glref = 0;
	vec2 offset2d{0.0f, 0.0f};
	vec3 offset3d{0.0f, 0.0f, 0.0f};
	bool vbo_deleted{false};
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
//
// Screen and IO Handling
//
struct GLFWwindow;
struct context
{
	GLFWwindow *window{nullptr};
	int32 width;
	int32 height;
	bool fullscreen;
	bool quit_requested;
};

// context
int32 create_context(context **ctx, const std::string &titletext, int32 width, int32 height, bool fullscreen);
int32 destroy_context(context **ctx);

// texture
int32 create_texture(texture **tex, int32 width, int32 height, const bool mipmapped = true);
int32 create_texture(texture **tex, const std::string &filename, const bool mipmapped = true);
void destroy_texture(texture **tex);

// drawbuffer
int32 create_drawbuffer(drawbuffer **db);
void destroy_drawbuffer(drawbuffer **db);

// framebuffer
int32 create_framebuffer(framebuffer **fb, int32 width, int32 height);
void destroy_framebuffer(framebuffer **fb);
void bind_framebuffer(framebuffer *fb);
void unbind_framebuffer(framebuffer *fb);



void disable_depthtest();
void enable_depthtest();
void ortho2d(int32 width, int32 height, bool flip, float32 near_z, float32 far_z);
bool want_to_quit(context *ctx);
void app_quit(context *ctx);
void swap(context *ctx);
void poll_input(context *ctx);
void process_input(context *ctx);
void clear_screen();
void clear_screen(pixel32 colour);
void set_clear_colour(pixel32 colour);

// free form drawing
void end_quads(drawbuffer *target);
void begin_quads(drawbuffer *target);
void draw_rectangle(drawbuffer *target,
					float32 x, float32 y,
					float32 width, float32 height,
					pixel32 colour);
void bind_texture(drawbuffer *db, texture *tex);
void textout(textureatlas *atlas, const char *text, int32 x, int32 y);