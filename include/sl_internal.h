#pragma once


using glm::dmat4x4;
using glm::dvec2;
using glm::dvec3;
using glm::dvec4;
using glm::ivec2;
using glm::ivec3;
using glm::ivec4;
using glm::mat4x4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;
 

#ifndef GLuint
typedef unsigned int GLuint;
#endif

#ifndef GLRef
typedef unsigned int GLRef;
#endif

#ifndef GLenum
	typedef unsigned int GLenum;
#endif
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef uint32_t pixel32;
typedef float float32;
typedef double float64;
typedef unsigned char ubyte;

#define ERROR_CREATE_DRAWBUFFER_DRAWBUFFER (-1)
#define ERROR_CREATE_DRAWBUFFER_VERTICES (-2)
#define ERROR_CREATE_DRAWBUFFER_NORMALS (-3)
#define ERROR_CREATE_DRAWBUFFER_COLOURS (-4)
#define ERROR_CREATE_DRAWBUFFER_TEXTURECOORDS (-5)
#define ERROR_CREATE_DRAWBUFFER_GLREF (-6)
#define CREATE_DRAWBUFFER_NO_ERROR (0)
#define DESTROY_DRAWBUFFER_NO_ERROR (0)


#define sgn(x) ((x<0)?-1:((x>0)?1:0))
#define argb(a,r,g,b) ( ((uint32)(a)<<24) |		\
			((((uint32)r)<<24)>>8) |	\
			((((uint32)g)<<24)>>16) |	\
			((((uint32)b)<<24)>>24))

#define rgb(r,g,b) argb(255,(r),(g),(b))
#define argb_f(a,r,g,b) argb((uint8)((a)*255.0f),(uint8)((r)*255.0f),(uint8)((g)*255.0f),(uint8)((b)*255.0f))
#define rgb_f(r,g,b) argb_f(1.0f,(r),(g),(b))
#define geta(p) ((uint32)((p))>>24)
#define getr(p) (((uint32)((p))<<8)>>24)
#define getg(p) (((uint32)((p))<<16)>>24)
#define getb(p) (((uint32)((p))<<24)>>24)

#define geta_nf(p) (geta(p)/255.0f)
#define getr_nf(p) (getr(p)/255.0f)
#define getg_nf(p) (getg(p)/255.0f)
#define getb_nf(p) (getb(p)/255.0f)
template <typename T, typename U, typename V>
inline T clamp(T a, U low, V high)
{
	T val = a < low ? low : a;
	return val > high ? high : val;
}
void cubic_plot(vec2 startpos, vec2 controlpos1, vec2 endpos, vec2 controlpos2, std::vector<vec2> &plotresult, int32 numsegments);
void quadratic_plot(vec2 startpos, vec2 controlpos, vec2 endpos, std::vector<vec2> &plotresult, int32 numsegments);


namespace x11colours {
	static constexpr pixel32 antiquewhite = rgb(0xfa, 0xeb, 0xd7);
	static constexpr pixel32 darkslategrey = rgb(0x2f, 0x4f, 0x4f);
	static constexpr pixel32 black = rgb(0, 0, 0);
	static constexpr pixel32 orange = rgb(255, 127, 0);
	static constexpr pixel32 magenta = rgb(255, 0, 255);
	static constexpr pixel32 grey = rgb(150, 150, 150);
	static constexpr pixel32 brightred = rgb(255, 128, 128);
	static constexpr pixel32 brightgreen = rgb(128, 255, 128);
	static constexpr pixel32 brightblue = rgb(128, 128, 255);
	static constexpr pixel32 darkgrey = rgb(64, 64, 64);
	static constexpr pixel32 mediumgrey = rgb(128, 128, 128);
	static constexpr pixel32 brightgrey = rgb(192, 192, 192);
	static constexpr pixel32 navy = rgb(0, 0, 128);
	static constexpr pixel32 royalblue = rgb(65, 105, 225);
	static constexpr pixel32 tomato = rgb(255, 99, 71);
	static constexpr pixel32 navyblue = rgb(0, 0, 0x80);
	static constexpr pixel32 maroon = rgb(0xb0,0x30,0x60);
	static constexpr pixel32 silver = rgb(192, 192, 192);
	static constexpr pixel32 green = rgb(0, 255, 0);
	static constexpr pixel32 white = rgb(255, 255, 255);
	static constexpr pixel32 red = rgb(255, 0, 0);
	static constexpr pixel32 blue = rgb(0, 0, 255);
	static constexpr pixel32 yellow = rgb(255, 255, 0);
	static constexpr pixel32 cyan = rgb(0, 255, 255);
	static constexpr pixel32 darkkhaki = rgb(189, 183, 107);
	static constexpr pixel32 khaki = rgb(240, 230, 140);
	static constexpr pixel32 palegoldenrod = rgb(238, 232, 170);
	static constexpr pixel32 peachpuff = rgb(255, 218, 185);
	static constexpr pixel32 moccasin = rgb(255, 228, 181);
	static constexpr pixel32 papayawhip = rgb(255, 239, 213);
	static constexpr pixel32 lightgoldenrodyellow = rgb(250, 250, 210);
	static constexpr pixel32 lemonchiffon = rgb(255, 250, 205);
	static constexpr pixel32 lightyellow = rgb(255, 255, 224);
	static constexpr pixel32 gold = rgb(255, 215, 0);
	static constexpr pixel32 darkorange = rgb(255, 140, 0);
	static constexpr pixel32 orangered = rgb(255, 69, 0);
	static constexpr pixel32 coral = rgb(255, 127, 80);
	static constexpr pixel32 lightsalmon = rgb(255, 160, 122);
	static constexpr pixel32 palevioletred = rgb(219, 112, 147);
	static constexpr pixel32 mediumvioletred = rgb(199, 21, 133);
	static constexpr pixel32 deeppink = rgb(255, 20, 147);
	static constexpr pixel32 hotpink = rgb(255, 105, 180);
	static constexpr pixel32 lightpink = rgb(255, 182, 193);
	static constexpr pixel32 pink = rgb(255, 192, 203);
	static constexpr pixel32 darkred = rgb(139, 0, 0);
	static constexpr pixel32 firebrick = rgb(178, 34, 34);
	static constexpr pixel32 crimson = rgb(220, 20, 60);
	static constexpr pixel32 lightcoral = rgb(240, 128, 128);
	static constexpr pixel32 indianred = rgb(205, 92, 92);
	static constexpr pixel32 thistle = rgb(219, 191, 216);
	static constexpr pixel32 plum = rgb(221, 160, 221);
	static constexpr pixel32 violet = rgb(238, 130, 238);
	static constexpr pixel32 orchid = rgb(218, 112, 214);
	static constexpr pixel32 fuchsia = rgb(255, 0, 255);
	static constexpr pixel32 mediumorchid = rgb(186, 85, 211);
	static constexpr pixel32 blueviolet = rgb(138, 43, 226);
	static constexpr pixel32 darkviolet = rgb(148, 0, 211);
	static constexpr pixel32 darkorchid = rgb(153, 50, 204);
	static constexpr pixel32 darkmagenta = rgb(139, 0, 139);
	static constexpr pixel32 purple = rgb(128, 0, 128);
	static constexpr pixel32 indigo = rgb(75, 0, 130);
	static constexpr pixel32 slateblue = rgb(106, 90, 205);
	static constexpr pixel32 darkslateblue = rgb(72, 61, 139);
	static constexpr pixel32 mediumslateblue = rgb(123, 104, 238);
	static constexpr pixel32 slategrey = rgb(112, 128, 144);
	static constexpr pixel32 lightslategrey = rgb(119, 136, 153);
	static constexpr pixel32 dimgrey = rgb(105, 105, 05);
	static constexpr pixel32 lightgrey = rgb(211, 211, 211);
	static constexpr pixel32 gainsboro = rgb(220, 220, 220);
	static constexpr pixel32 mistyrose = rgb(255, 228, 225);
	static constexpr pixel32 lavenderbush = rgb(255, 240, 245);
	static constexpr pixel32 linen = rgb(250, 240, 230);
	static constexpr pixel32 ivory = rgb(255, 255, 240);
	static constexpr pixel32 floralwhite = rgb(255, 250, 240);
	static constexpr pixel32 oldlace = rgb(253, 245, 230);
	static constexpr pixel32 beige = rgb(245, 245, 220);
	static constexpr pixel32 seashell = rgb(255, 245, 238);
	static constexpr pixel32 whitesmoke = rgb(245, 245, 245);
	static constexpr pixel32 aliceblue = rgb(240, 248, 255);
	static constexpr pixel32 azure = rgb(240, 255, 255);
	static constexpr pixel32 mintcream = rgb(245, 255, 250);
	static constexpr pixel32 honeydew = rgb(240, 255, 250);
	static constexpr pixel32 snow = rgb(255, 250, 250);
	static constexpr pixel32 brown = rgb(265, 42, 42);
	static constexpr pixel32 sienna = rgb(160, 82, 45);
	static constexpr pixel32 saddlebrown = rgb(139, 69, 19);
	static constexpr pixel32 chocolate = rgb(210, 105, 30);
	static constexpr pixel32 peru = rgb(205, 133, 63);
	static constexpr pixel32 darkgoldenrod = rgb(184, 134, 11);
	static constexpr pixel32 goldenrod = rgb(218, 165, 32);
	static constexpr pixel32 sandybrown = rgb(244, 164, 96);
	static constexpr pixel32 rosybrown = rgb(188, 143, 143);
	static constexpr pixel32 tan = rgb(210, 180, 140);
	static constexpr pixel32 burlywood = rgb(222, 184, 135);
	static constexpr pixel32 wheat = rgb(245, 222, 179);
	static constexpr pixel32 navajowhite = rgb(255, 222, 173);
	static constexpr pixel32 bisque = rgb(255, 228, 196);
	static constexpr pixel32 blanchedalmond = rgb(255, 235, 205);
	static constexpr pixel32 cornsilk = rgb(255, 248, 220);
	static constexpr pixel32 greenyellow = rgb(173, 255, 47);
	static constexpr pixel32 chartreuse = rgb(127, 255, 0);
	static constexpr pixel32 lawngreen = rgb(124, 252, 0);
	static constexpr pixel32 limegreen = rgb(50, 205, 50);
	static constexpr pixel32 palegreen = rgb(152, 251, 152);
	static constexpr pixel32 lightgreen = rgb(144, 238, 144);
	static constexpr pixel32 mediumspringgreen = rgb(0, 250, 154);
	static constexpr pixel32 springgreen = rgb(0, 255, 127);
	static constexpr pixel32 mediumseagreen = rgb(60, 179, 113);
	static constexpr pixel32 seagreen = rgb(46, 139, 87);
	static constexpr pixel32 forestgreen = rgb(34, 139, 24);
	static constexpr pixel32 darkgreen = rgb(0, 100, 0);
	static constexpr pixel32 yellowgreen = rgb(154, 205, 50);
	static constexpr pixel32 olivedrab = rgb(107, 142, 35);
	static constexpr pixel32 darkolivegreen = rgb(85, 107, 47);
	static constexpr pixel32 mediumaquamarine = rgb(102, 205, 170);
	static constexpr pixel32 darkseagreen = rgb(143, 188, 143);
	static constexpr pixel32 lightseagreen = rgb(32, 178, 170);
	static constexpr pixel32 darkcyan = rgb(0, 139, 139);
	static constexpr pixel32 lightcyan = rgb(224, 255, 255);
	static constexpr pixel32 paleturquoise = rgb(175, 238, 238);
	static constexpr pixel32 aquamarine = rgb(127, 255, 212);
	static constexpr pixel32 turquoise = rgb(64, 224, 208);
	static constexpr pixel32 mediumturquoise = rgb(72, 209, 204);
	static constexpr pixel32 darkturquoise = rgb(0, 206, 209);
	static constexpr pixel32 cadetblue = rgb(95, 158, 160);
	static constexpr pixel32 steelblue = rgb(70, 130, 180);
	static constexpr pixel32 lightsteelblue = rgb(176, 196, 222);
	static constexpr pixel32 powderblue = rgb(176, 224, 230);
	static constexpr pixel32 lightblue = rgb(173, 216, 230);
	static constexpr pixel32 skyblue = rgb(135, 206, 235);
	static constexpr pixel32 lightskyblue = rgb(135, 206, 250);
	static constexpr pixel32 deepskyblue = rgb(0, 191, 255);
	static constexpr pixel32 dodgerblue = rgb(30, 144, 255);
	static constexpr pixel32 cornflowerblue = rgb(100, 149, 237);
	static constexpr pixel32 mediumblue = rgb(0, 0, 205);
	static constexpr pixel32 darkblue = rgb(0, 0, 139);
	static constexpr pixel32 midnightblue = rgb(25, 25, 112);
	static constexpr pixel32 teal = rgb(0, 128, 128);
}


