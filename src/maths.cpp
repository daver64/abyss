#include "abyss.h"

const float64 PI = 3.14159265358979323846264338327950288;
const float64 TAU = 2.0 * PI;
const float64 PIDIV2 = PI / 2.0;
float64 ROUNDING_ERROR = std::numeric_limits<float64>::epsilon();

const float32 PIf = 3.14159265358979323846;
const float32 TAUf = 2.0f * PIf;
const float32 PIDIV2f = PIf / 2.0f;
float32 ROUNDING_ERRORf = std::numeric_limits<float32>::epsilon();

vec3 camera::forward_vector() const
{
    return glm::rotate(rotation, (vec3(0.0f, 0.0f, -1.0f)));
}

vec3 camera::back_vector() const
{
    return -glm::rotate(rotation, (vec3(0.0f, 0.0f, -1.0f)));
}

vec3 camera::up_vector() const
{
    return glm::rotate(rotation, (vec3(0.0f, 1.0f, 0.0f)));
}

vec3 camera::down_vector() const
{
    return glm::rotate(rotation, (-vec3(0.0f, 1.0f, 0.0f)));
}

vec3 camera::left_vector() const
{
    return glm::rotate(rotation, (vec3(1.0f, 0.0f, 0.0f)));
}

vec3 camera::right_vector() const
{
    return glm::rotate(rotation, (-vec3(1.0f, 0.0f, 0.0f)));
}

polar::polar(const float32 &lon,
             const float32 &lat,
             const float32 &alt) : lon(lon), lat(lat), alt(alt) {}

polar::polar(const float32 p[3]) : lon(p[0]), lat(p[1]), alt(p[0]) {}

polar::polar(const vec3 &pos)
{
    rad = std::sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
    lon = std::atan(pos.y / pos.x);
    lat = std::atan(std::sqrt(pos.x * pos.x + pos.y * pos.y) / pos.z);
}
float32 &polar::operator[](size_t index)
{
    return d[index];
}

vec3 polar::polar_to_cartesian(const polar &pos)
{
    return vec3(std::cos(pos.lon) * std::cos(pos.lat) * pos.rad,
                std::sin(pos.lon) * std::cos(pos.lat) * pos.rad,
                std::sin(pos.lat) * pos.rad);
}

polar polar::cartesian_to_polar(const vec3 &pos)
{
    return polar(pos);
}

vec3 polar::to_cartesian()
{
    return polar_to_cartesian(*this);
}

void polar::from_cartesian(const vec3 &pos)
{
    *this = cartesian_to_polar(pos);
}

plane::plane()
{
    normal = vec3(0, 1, 0);
}

plane::~plane()
{
}

void quadratic_plot(vec2 startpos, vec2 controlpos, vec2 endpos, std::vector<vec2> &plotresult, int32 numsegments)
{
    for (int32 i = 0; i <= numsegments; ++i)
    {
        const float32 t = (float32)i / (float32)numsegments;
        const float32 a = std::pow((1.0 - t), 2.0);
        const float32 b = 2.0 * t * (1.0 - t);
        const float32 c = std::pow((float32)t, 2.0);
        const float32 x = a * startpos.x + b * controlpos.x + c * endpos.x;
        const float32 y = a * startpos.y + b * controlpos.y + c * endpos.y;
        plotresult.emplace_back(vec2(x,y));
    }
}

void cubic_plot(vec2 startpos, vec2 controlpos1, vec2 endpos, vec2 controlpos2, std::vector<vec2> &plotresult, int32 numsegments)
{
    for (int32 i = 0; i <= numsegments; ++i)
    {
        const float32 t = (float_t)i / (float_t)numsegments;
        const float32 a = powf((1.0f - t), 3.0f);
        const float32 b = 3.0f * t * powf((1.0f - t), 2.0f);
        const float32 c = 3.0f * powf(t, 2.0f) * (1.0f - t);
        const float32 d = powf(t, 3.0f);
        const float32 x = a * startpos.x + b * controlpos1.x + d * endpos.x + c * controlpos2.x;
        const float32 y = a * startpos.y + b * controlpos1.y + d * endpos.y + c * controlpos2.y;
        plotresult.emplace_back(vec2(x, y));
    }
}

vec2 get_line_intersection(vec2 v0, vec2 v1, vec2 v2, vec2 v3, bool &success)
{
    vec2 vr;
    success = get_line_intersection(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, vr.x, vr.y);
    return vr;
}

bool get_line_intersection(float32 p0_x, float32 p0_y,
                           float32 p1_x, float32 p1_y,
                           float32 p2_x, float32 p2_y,
                           float32 p3_x, float32 p3_y,
                           float32 &i_x, float32 &i_y)
{
    float32 s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;
    float32 s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);
    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        i_x = p0_x + (t * s1_x);
        i_y = p0_y + (t * s1_y);
        return true;
    }
    i_x = NAN;
    i_y = NAN;
    return false;
}

float64 gcircle_distance(float64 lon1, float64 lat1, float64 lon2, float64 lat2, float64 R)
{
    const float64 theta1 = degtorad(lat1);
    const float64 theta2 = degtorad(lat2);
    const float64 delta_theta = degtorad(lat2 - lat1);
    const float64 delta_tau = degtorad(lon2 - lon1);
    const float64 a = sin(delta_theta / 2.0) * sin(delta_theta / 2.0) +
                      cos(theta1) * cos(theta2) * sin(delta_tau / 2) * sin(delta_tau / 2.0);
    const float64 c = 2 * atan2(sqrt(a), sqrt(1 - a));
    float64 d = R * c;
    return d;
}

float64 gcircle_bearing(float64 from_lon, float64 from_lat, float64 to_lon, float64 to_lat)
{
    const float64 y = sin(degtorad(to_lon - from_lon)) * cos(degtorad(to_lat));
    const float64 x = cos(degtorad(from_lat)) * sin(degtorad(to_lat)) - sin(degtorad(from_lat)) * cos(degtorad(to_lat)) * cos(degtorad(to_lon - from_lon));
    float64 b = radtodeg(atan2(y, x));
    if (b < 0)
        b = 360 + b;
    return b;
}

void gcircle_destination(float64 from_lon, float64 from_lat, float64 bearing, float64 distance, float64 &to_lon, float64 &to_lat, float64 R)
{
    const float64 to_lat1 = asin(sin(degtorad(from_lat)) * cos(distance / R) +
                                 cos(degtorad(from_lat)) * sin(distance / R) * cos(degtorad(bearing)));
    const float64 lon = degtorad(from_lon) + atan2(sin(degtorad(bearing)) *
                                                       sin(distance / R) * cos(degtorad(from_lat)),
                                                   cos(distance / R) - sin(degtorad(from_lat)) * sin(degtorad(to_lat1)));
    to_lat = radtodeg(to_lat1);
    to_lon = radtodeg(lon);
}