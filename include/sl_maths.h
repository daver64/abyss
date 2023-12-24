/***
* Copyright 2004-2024, Dave Rowbotham and Toni Ylisirnio
* All rights reserved.
*/
#pragma once


extern const float64 PI;
extern const float64 TAU;
extern const float64 PIDIV2;
extern const float64 ROUNDING_ERROR;

extern const float32 PIf;
extern const float32 TAUf;
extern const float32 PIDIV2f;
extern const float32 ROUNDING_ERRORf;

//inline float32 lerp(float32 v1, float32 v2, float32 a) { return (v1 * (1.0f - a) + v2 * a); }
//inline float64 lerp(float64 v1, float64 v2, float64 a) { return (v1 * (1.0 - a) + v2 * a); }
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
		const vec3 &point) const;
	void set_plane(
		const vec3 &point1, const vec3 &point2, const vec3 &point3);
	void recalculate(const vec3 &point);
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