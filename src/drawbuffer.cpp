
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
#include "abyss.h"
#include <algorithm>
#include <functional>
#include <cmath>

void enable_arrays()

{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void disable_arrays()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

int32 get_patch(float32 lon, float32 lat, float32* u, float32* v, float32 width, float32 height)
{
	float32 dlon = lon;
	float32 dlat = lat;
	dlon = fmodf((dlon)+180.0f, 360.0f);
	dlat = fmodf(-(dlat)+90.0f, 180.0f);

	float32 x=1;
	float32 y=1;
	if (std::abs(width) > 0 && std::abs(height) > 0)
	{
		x = lon / (360.0f / width);
		y = lat / (180.0f / height);
	}
	if (u != NULL && v != NULL)
	{
		*u = y;
		*v = x;
	}
	return static_cast<int32>(std::floor(x) + width * std::floor(y));
}

drawbuffer::drawbuffer(bool triangles, bool dynamic) 
{
    if(triangles)
        primitive=GL_TRIANGLES;
    else 
        primitive=GL_LINES;

    if(dynamic)
        usage=GL_DYNAMIC_DRAW;
    else
        usage=GL_STATIC_DRAW;
	current_vertex = vec3{ 0.0f,0.0f,0.0f };
	current_normal = vec3{ 0.0f,1.0f,0.0f };
	current_colour = vec4{ 1.0f,1.0f,1.0f,1.0f };
	current_texcoord0 = vec2{ 0.0f,0.0f };
	current_texcoord1 = vec2{ 0.0f,0.0f };
	current_texcoord2 = vec2{ 0.0f,0.0f };
	current_texcoord3 = vec2{ 0.0f,0.0f };
	vbo_id = 0;
	vert_offset = 0;
	norm_offset = 0;
	col_offset = 0;
	tex0_offset = 0;
	tex1_offset = 0;
	tex2_offset = 0;
	tex3_offset = 0;
	offset2d = vec2(0.5f, 0.5f);
	offset3d=vec3(0.0f,0.0f,0.0f);
	vertices.reserve(64000);
	normals.reserve(64000);
	colours.reserve(64000);
	vtexcoord0.reserve(64000);
	vtexcoord1.reserve(64000);
	vtexcoord2.reserve(64000);
	vtexcoord3.reserve(64000);
	glGenBuffers(1, &vbo_id);
}
drawbuffer::~drawbuffer()
{
	if(!vbo_deleted)
		glDeleteBuffers(1, &vbo_id);
}
void drawbuffer::vertex(vec3 v)
{
	current_vertex = vec3{ v.x + offset3d.x ,v.y + offset3d.y, v.z + offset3d.z };
	vertices.emplace_back(current_vertex);
	colours.emplace_back(current_colour);
	normals.emplace_back(current_normal);
	vtexcoord0.emplace_back(current_texcoord0);
	vtexcoord1.emplace_back(current_texcoord1);
	vtexcoord2.emplace_back(current_texcoord2);
	vtexcoord3.emplace_back(current_texcoord3);
}
void drawbuffer::vertex(vec2 v)
{
	current_vertex = vec3{ v.x + offset2d.x, v.y + offset2d.y, 0.0f };
	vertices.emplace_back(current_vertex);
	colours.emplace_back(current_colour);
	normals.emplace_back(current_normal);
	vtexcoord0.emplace_back(current_texcoord0);
	vtexcoord1.emplace_back(current_texcoord1);
	vtexcoord2.emplace_back(current_texcoord2);
	vtexcoord3.emplace_back(current_texcoord3);
}
void drawbuffer::vertex(float32 x, float32 y, float32 z)
{
	vertex(vec3(x + offset3d.x,y + offset3d.y, z + offset3d.z));
}
void drawbuffer::normal(vec3 n)
{
	current_normal = vec3{ n.x,n.y,n.z };
}
void drawbuffer::normal(float32 nx, float32 ny, float32 nz)
{
	normal(vec3(nx,ny,nz));
}
void drawbuffer::set_active_texture_unit(int32 unit)
{
	active_texture_unit = clamp(unit, 0, 3);
}
int32_t drawbuffer::get_active_texture_unit()
{
	return active_texture_unit;
}
void drawbuffer::texcoord(vec2 t)
{
	switch (active_texture_unit)
	{
	case 0:
		texcoord0(t);
		break;
	case 1:
		texcoord1(t);
		break;
	case 2:
		texcoord2(t);
		break;
	case 3:
		texcoord3(t);
		break;
	default:
		texcoord0(t);
		break;
	}
}
void drawbuffer::texcoord(float32 u, float32 v)
{
	switch (active_texture_unit)
	{
	case 0:
		current_texcoord0 = vec2{ u,v };
		break;
	case 1:
		current_texcoord1 = vec2{ u,v };
		break;
	case 2:
		current_texcoord2 = vec2{ u,v };
		break;
	case 3:
		current_texcoord3 = vec2{ u,v };
		break;
	default:
		current_texcoord0 = vec2{ u,v };
		break;
	}
}
void drawbuffer::texcoord0(vec2 t)
{
	current_texcoord0 = vec2{ t.s,t.t };
}
void drawbuffer::texcoord1(vec2 t)
{
	current_texcoord1 = vec2{ t.s,t.t };
}
void drawbuffer::texcoord2(vec2 t)
{
	current_texcoord2 = vec2{ t.s,t.t };
}
void drawbuffer::texcoord3(vec2 t)
{
	current_texcoord3 = vec2{ t.s,t.t };
}
void drawbuffer::texcoord0(float32 u, float32 v)
{
	texcoord0(vec2(u, v));
}
void drawbuffer::texcoord1(float32 u, float32 v)
{
	texcoord1(vec2(u, v));
}
void drawbuffer::texcoord2(float32 u, float32 v)
{
	texcoord2(vec2(u, v));
}
void drawbuffer::texcoord3(float32 u, float32 v)
{
	texcoord3(vec2(u, v));
}
const vec3 drawbuffer::get_vertex(int32 index)
{
	return vec3(vertices[index].x, vertices[index].y, vertices[index].y);
}
vec2 drawbuffer::get_vertex2(int32 index)
{
	return vec2(vertices[index].x, vertices[index].y);
}
vec3 drawbuffer::get_normal(int32 index)
{
	return vec3(normals[index].x, normals[index].y, normals[index].z);
}
vec2 drawbuffer::get_texcoord(int32 index, int32 texture_index)
{
	vec2 result{ 0,0 };
	switch (texture_index)
	{
	case 0:
		result.x = vtexcoord0[index].x;
		result.y = vtexcoord0[index].y;
		break;
	case 1:
		result.x = vtexcoord1[index].x;
		result.y = vtexcoord1[index].y;
		break;
	case 2:
		result.x = vtexcoord2[index].x;
		result.y = vtexcoord2[index].y;
		break;
	case 3:
		result.x = vtexcoord3[index].x;
		result.y = vtexcoord3[index].y;
		break;
	}
	return result;
}
//void PrimitiveBuffer::colour(RGBAf c)
//{
//	current_colour.d[0] = c.d[0];
//	current_colour.d[1] = c.d[1];
//	current_colour.d[2] = c.d[2];
//	current_colour.d[3] = c.d[3];

//	current_colour.a = c.a;
//	current_colour.r = c.r;
//	current_colour.g = c.g;
//	current_colour.b = c.b;

//}
void drawbuffer::colour(pixel32 c)
{
	const float32 r = getr_nf(c);
	const float32 g = getg_nf(c);
	const float32 b = getb_nf(c);
	const float32 a = geta_nf(c);
	current_colour = vec4{ r,g,b,a };
	//current_colour.r = r;
	//current_colour.g = g;
	//current_colour.b = b;
	//current_colour.a = a;
}
void drawbuffer::colour(float32 r, float32 g, float32 b, float32 a)
{
	current_colour = vec4{ r,g,b,a };
	//current_colour.r = r;
	//current_colour.g = g;
	//current_colour.b = b;
	//current_colour.a = a;
}

void drawbuffer::point(float32 x1, float32 y1, pixel32 c)
{
	colour(c);
	vertex(vec2(x1 , y1 ));
}
void drawbuffer::line(float32 x1, float32 y1, float32 x2, float32 y2, pixel32 c)
{
	pixel32 colours[2]{ c,c };
	line(x1 , y1 , x2 , y2 , colours);
}
void drawbuffer::line(float32 x1, float32 y1, float32 x2, float32 y2, pixel32 colours[2])
{
	colour(colours[0]);
	vertex(vec2(x1 , y1 ));
	colour(colours[1]);
	vertex(vec2(x2 , y2 ));
}
void drawbuffer::rect(float32 x1, float32 y1, float32 width, float32 height, pixel32 c)
{
	pixel32 colours[4]{ c,c,c,c };
	rect(x1 , y1 , width, height, colours);
}

void drawbuffer::rect(float32 x1, float32 y1, float32 width, float32 height, pixel32 colours[4])
{
	float32 x2 = x1 + width;
	float32 y2 = y1 + height;

	texcoord(vec2(0, 0));
	colour(colours[0]);
	normal(vec3(0, 0, 1));
	vertex(vec2(x1 , y1 ));

	texcoord(vec2(1, 0));
	colour(colours[1]);
	normal(vec3(0, 0, 1));
	vertex(vec2(x2 , y1 ));


	texcoord(vec2(1, 1));
	colour(colours[2]);
	normal(vec3(0, 0, 1));
	vertex(vec2(x2 , y2 ));

	texcoord(vec2(0, 1));
	colour(colours[3]);
	normal(vec3(0, 0, 1));
	vertex(vec2(x1 , y2 ));

}
 
void drawbuffer::circle(float32 x, float32 y, float32 radius, pixel32 ccolour, int32 numsegments)
{
	colour(ccolour);
	const float32 dth = PIf / numsegments;
	if (primitive == GL_LINES)
	{
		for (float32 theta = 0; theta <= 2.0f * PIf; theta += dth)
		{
			const float32 x1 = (x + radius * cosf(theta));
			const float32 y1 = (y + radius * sinf(theta));
			theta += dth;
			const float32 x2 = (x + radius * cosf(theta));
			const float32 y2 = (y + radius * sinf(theta));
			vertex(vec2(x1 , y1 ));
			vertex(vec2(x2 , y2 ));
			theta -= dth;
		}
	}
	else if (primitive == GL_TRIANGLES)
	{
		colour(ccolour);
		for (float32 theta = 0; theta <= 2.0f * PIf; theta += dth)
		{
			const float32 x1 = (x + radius * cosf(theta));
			const float32 y1 = (y + radius * sinf(theta));
			const float32 last_theta = theta;
			theta += dth;
			const float32 x2 = (x + radius * cosf(theta));
			const float32 y2 = (y + radius * sinf(theta));
			vertex(vec2(x2 , y2 ));
			vertex(vec2(x1 , y1 ));
			vertex(vec2(x , y ));
			theta = last_theta;
		}
	}
}
void drawbuffer::ellipse(float32 x, float32 y, float32 width, float32 height, uint32 ccolour, int32 numsegments)
{
	colour(ccolour);
	if (primitive == GL_LINES)
	{
		for (float32 theta = 0.0f; theta < (2.0f * PIf); theta += (PIf / (float32)numsegments))
		{
			const float32 x1 = width * cosf(theta) + x;
			const float32 y1 = height * sinf(theta) + y;
			const float32 x2 = width * cosf(theta + (PIf / (float32)numsegments)) + x;
			const float32 y2 = height * sinf(theta + (PIf / (float32)numsegments)) + y;
			vertex(vec2(x1 , y1 ));
			vertex(vec2(x2 , y2 ));
		}
	}
	else if (primitive == GL_TRIANGLES)
	{
		for (float32 theta = 0.0f; theta < (2.0f * PIf); theta += (PIf / (float32)numsegments))
		{
			const float32 x1 = width * cosf(theta) + x;
			const float32 y1 = height * sinf(theta) + y;
			const float32 x2 = width * cosf(theta + (PIf / (float32)numsegments)) + x;
			const float32 y2 = height * sinf(theta + (PIf / (float32)numsegments)) + y;
			vertex(vec2(x2 , y2 ));
			vertex(vec2(x1 , y1 ));
			vertex(vec2(x , y ));
		}
	}
}
void drawbuffer::triangle(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, uint32 colours[3])
{
	colour(colours[0]);
	vertex(vec2(x1 , y1 ));
	colour(colours[1]);
	vertex(vec2(x2 , y2 ));
	colour(colours[2]);
	vertex(vec2(x3 , y3 ));
}
void drawbuffer::quad(vec3 p1, vec3 p2, vec3 p3, vec3 p4)
{
	texcoord0(vec2(0.0f, 0.0f));
	vertex(p1);
	texcoord0(vec2(1.0f, 0.0f));
	vertex(p2);
	texcoord0(vec2(1.0f, 1.0f));
	vertex(p3);
	texcoord0(vec2(0.0f, 1.0f));
	vertex(p4);
}
void drawbuffer::quadratic_bezier(vec2 startpos, vec2 controlpos, vec2 endpos, int32_t numseg)
{
	std::vector<vec2> plotresult;
	quadratic_plot(startpos, controlpos, endpos, plotresult, numseg);
	for (size_t i = 1; i < numseg; i++)
	{
		const vec2 v1 = plotresult[i - 1];
		vertex(v1);
		const vec2 v2 = plotresult[i];
		vertex(v2);
	}
}
 
void drawbuffer::draw_sphere_patch(float32 slon, float32 slat,
	float32 elon, float32 elat,
	int32 subdivide, int32 ysubdivide,
	float32 radius, float32 texture_width, float32 texture_height)
{
	float32 lon, lat;
	vec3 vert;
	vec3 norm;
	float32 u, v;

	if (!ysubdivide)
		ysubdivide = subdivide;

	for (int32_t patch_y = 0; patch_y < ysubdivide; patch_y++)
	{
		for (int32_t x = 0; x <= subdivide; x++)
		{
			for (int32_t y = patch_y; y <= patch_y + 1; y++)
			{
				lon = slon + ((elon - slon) / subdivide) * x;
				lat = slat + ((elat - slat) / ysubdivide) * y;
				u = (float32)x / (float32)subdivide;
				v = (float32)y / (float32)ysubdivide;
				float32 tu, tv;
				get_patch((float32)lon, (float32)lat, &tu, &tv, texture_width, texture_height);
				polar pol(degtorad(lon),degtorad(lat), radius);
				vert = pol.to_cartesian();
				norm = vert;
				//norm.normalise();
				glm::normalize(norm);
				vertex(vert);
				normal(norm);
				texcoord(vec2(u, v));

			}
		}
	}
	draw();
    
}
void drawbuffer::sphere(float32 radius, float32 texture_width, float32 texture_height)
{
	int32_t nslice = 128;
	draw_sphere_patch(-179.995, -89.995, 179.995, 89.995, nslice, nslice / 2, radius, texture_width, texture_height);
}
 
/*std::vector<Line3<float32_t> > GLPrimitiveBuffer::get_lines()
{
	std::vector<Line3<float32_t>> lines;
	size_t numvert = vertices.size();
	for (size_t i1 = 0; i1 < numvert; i1++)
	{
		size_t i2 = (i1 + 1) % numvert;
		Line3<float32_t> l(get_vertex((int32_t)i1), get_vertex((int32_t)i2));
		lines.emplace_back(l);
	}
	return lines;
}*/
void drawbuffer::alphasort()
{//
	//std::sort(std::begin(colours), std::end(colours), [](vec4 a, vec4 b) {return a.a > b.a; });
}


void drawbuffer::build()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	const GLsizei vert_size = (GLsizei)(sizeof(vec3) * vertices.size());
	const GLsizei norm_size = (GLsizei)(sizeof(vec3) * normals.size());
	const GLsizei col_size = (GLsizei)(sizeof(vec4) * colours.size());
	const GLsizei tex0_coord_size = (GLsizei)(sizeof(vec2) * vtexcoord0.size());
	const GLsizei tex1_coord_size =(GLsizei)(sizeof(vec2) * vtexcoord1.size());
	const GLsizei tex2_coord_size = 0;// (GLsizei)(sizeof(vec2) * vtexcoord2.size());
	const GLsizei tex3_coord_size = 0;// (GLsizei)(sizeof(vec2) * vtexcoord3.size());

	vert_offset = 0;
	norm_offset = vert_offset + vert_size;
	col_offset = norm_offset + norm_size;
	tex0_offset = col_offset + col_size;
	tex1_offset = tex0_offset + tex0_coord_size;
	tex2_offset = tex1_offset + tex1_coord_size;
	tex3_offset = tex2_offset + tex2_coord_size;

	glBufferData(GL_ARRAY_BUFFER, vert_size + norm_size + col_size + tex0_coord_size
		+ tex1_coord_size + tex2_coord_size + tex3_coord_size, 0, usage);
	if (vert_size)
		glBufferSubData(GL_ARRAY_BUFFER, (GLint)vert_offset, vert_size, &vertices[0]);
	if (norm_size)
		glBufferSubData(GL_ARRAY_BUFFER, (GLint)norm_offset, norm_size, &normals[0]);
	if (col_size)
		glBufferSubData(GL_ARRAY_BUFFER, (GLint)col_offset, col_size, &colours[0]);

	//gl_bind_texture_to_unit(tex0_glref,0);
	if (tex0_glref > 0)
	{
		//glBindTexture(GL_TEXTURE_2D,tex0_glref);
		glActiveTexture(GL_TEXTURE0);
		glClientActiveTexture(GL_TEXTURE0);
		
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if (tex0_coord_size)
			glBufferSubData(GL_ARRAY_BUFFER, (GLint)tex0_offset, tex0_coord_size, &vtexcoord0[0]);
	}


	//gl_bind_texture_to_unit(tex1_glref,1);
	if (tex1_glref > 0)
	{
		//glBindTexture(GL_TEXTURE_2D,tex1_glref);
		glActiveTexture(GL_TEXTURE0 + 1);
		glClientActiveTexture(GL_TEXTURE0 + 1);
		
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if (tex1_coord_size)
			glBufferSubData(GL_ARRAY_BUFFER, (GLint)tex1_offset, tex1_coord_size, &vtexcoord1[0]);
	}
	//gl_bind_texture_to_unit(tex2_glref,2);


}
void drawbuffer::draw()
{
	if (usage != GL_STATIC_DRAW)
		build();
	const GLsizei vert_size = (GLsizei)(sizeof(vec3) * vertices.size());
	const GLsizei norm_size = (GLsizei)(sizeof(vec3) * normals.size());
	const GLsizei col_size = (GLsizei)(sizeof(vec4) * colours.size());
	const GLsizei tex0_coord_size = (GLsizei)(sizeof(vec2) * vtexcoord0.size());
	const GLsizei tex1_coord_size = (GLsizei)(sizeof(vec2) * vtexcoord1.size());
	const GLsizei tex2_coord_size = 0;// (GLsizei)(sizeof(vec2) * vtexcoord2.size());
	const GLsizei tex3_coord_size = 0;// (GLsizei)(sizeof(vec2) * vtexcoord3.size());

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	if (vert_size)
		glVertexPointer(3, GL_FLOAT, 0, (void*)vert_offset);
	if (norm_size)
		glNormalPointer(GL_FLOAT, 0, (void*)norm_offset);
	if (col_size)
		glColorPointer(4, GL_FLOAT, 0, (void*)col_offset);


	bool textured = false;
	if (tex0_glref > 0)
	{
		enable_texturing();
		//glBindTexture(GL_TEXTURE_2D,tex0_glref);
		//gl_bind_texture_to_unit(tex0_glref, 0);
		glActiveTexture(GL_TEXTURE0);
		
		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if (tex0_coord_size)
			glTexCoordPointer(2, GL_FLOAT, 0, (void*)tex0_offset);
		textured = true;
	}

	if (tex1_glref > 0)
	{
		enable_texturing();
		//gl_bind_texture_to_unit(tex1_glref, 1);
		//glBindTexture(GL_TEXTURE_2D,tex1_glref);
		glActiveTexture(GL_TEXTURE0 + 1);
		glClientActiveTexture(GL_TEXTURE0 + 1);
		
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if (tex1_coord_size)
			glTexCoordPointer(2, GL_FLOAT, 0, (void*)tex1_offset);
		textured = true;
	}


	if (!textured)
	{
		disable_texturing();
	}
	else
	{
		enable_texturing();
	}


	enable_arrays();
	glDrawArrays(primitive, 0, (GLsizei)vertices.size());
	disable_arrays();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (usage != GL_STATIC_DRAW)
		reset();

	//gl_bind_texture_to_unit(0, 0);
	//gl_bind_texture_to_unit(0, 1);
	glBindTexture(GL_TEXTURE_2D,0);
	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);

}
void drawbuffer::reset()
{
	vertices.clear();
	normals.clear();
	vtexcoord0.clear();
	vtexcoord1.clear();
	vtexcoord2.clear();
	vtexcoord3.clear();
	colours.clear();
}
void drawbuffer::begin(GLenum type)
{
	primitive = type;
}
void drawbuffer::begin_triangles()
{
    primitive=GL_TRIANGLES;
}
void drawbuffer::begin_lines()
{
    primitive=GL_LINES;
}

void drawbuffer::end()
{
	if (usage != GL_STATIC_DRAW)
		draw();
}

vec3* drawbuffer::vertex_pointer(int32 index)
{
	if (index < vertices.size())
		return &vertices[index];
	return NULL;
}
vec3* drawbuffer::normal_pointer(int32 index)
{
	if (index < normals.size())
		return &normals[index];
	return NULL;
}
vec4* drawbuffer::colour_pointer(int32 index)
{
	if (index < colours.size())
		return &colours[index];
	return NULL;
}
vec2* drawbuffer::texcoord0_pointer(int32 index)
{
	if (index < vtexcoord0.size())
		return &vtexcoord0[index];
	return NULL;
}
vec2* drawbuffer::texcoord1_pointer(int32 index)
{
	if (index < vtexcoord1.size())
		return &vtexcoord1[index];
	return NULL;
}
vec2* drawbuffer::texcoord2_pointer(int32 index)
{
	if (index < vtexcoord2.size())
		return &vtexcoord2[index];
	return NULL;
}
vec2* drawbuffer::texcoord3_pointer(int32 index)
{
	if (index < vtexcoord3.size())
		return &vtexcoord3[index];
	return NULL;
}