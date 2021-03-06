/*
    Banshee and all code, documentation, and other materials contained
    therein are:

        Copyright 2013 Dmitry Kozlov
        All Rights Reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the software's owners nor the names of its
        contributors may be used to endorse or promote products derived from
        this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    (This is the Modified BSD License)
*/

#ifndef MATHUTILS_H
#define MATHUTILS_H


#include "float3.h"
#include "float2.h"
#include "quaternion.h"
#include "matrix.h"
#include "ray.h"

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

#define PI 3.14159265358979323846f

#ifndef WIN32
#define FLT_EPSILON 0.00000000001f
#endif

#define OFFSETOF(struc,member) (&(((struc*)0)->member))

/// Initialize RNG
inline void rand_init() { std::srand((unsigned)std::time(0)); }

/// Generate random float value within [0,1] range
inline float rand_float() { return (float)std::rand()/RAND_MAX; }

/// Genarate random uint value
inline unsigned	rand_uint() { return (unsigned)std::rand(); }

/// Convert cartesian coordinates to spherical
inline void	cartesian_to_spherical ( float3 const& cart, float& r, float& phi, float& theta )
{
    float temp = std::atan2(cart.z, cart.x);
    r = std::sqrt(cart.x*cart.x + cart.y*cart.y + cart.z*cart.z);
    phi = (float)((temp >= 0)?temp:(temp + 2*PI));
    theta = std::acos(cart.y/r);
}

/// Convert cartesian coordinates to spherical
inline void	cartesian_to_spherical ( float3 const& cart, float3& sph ) 
{
	cartesian_to_spherical(cart, sph.x, sph.y, sph.z);
}


/// Clamp int value to [a, b] range
inline int clamp(int x, int a, int b)
{
    return x < a ? a : (x > b ? b : x);
}

/// Clamp float value to [a, b) range
inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}

/// Clamp each component of the vector to [a, b) range
inline float3 clamp(float3 const& v, float3 const& v1, float3 const& v2)
{
    float3 res;
    res.x = clamp(v.x, v1.x, v2.x);
    res.y = clamp(v.y, v1.y, v2.y);
    res.z = clamp(v.z, v1.z, v2.z);
    return res;
}

/// Clamp each component of the vector to [a, b) range
inline float2 clamp(float2 const& v, float2 const& v1, float2 const& v2)
{
    float2 res;
    res.x = clamp(v.x, v1.x, v2.x);
    res.y = clamp(v.y, v1.y, v2.y);
    return res;
}

/// Convert spherical coordinates to cartesian 
inline void	spherical_to_cartesian ( float r, float phi, float theta, float3& cart )
{
	cart.y = r * std::cos(theta);
	cart.z = r * std::sin(theta) * std::cos(phi);
	cart.x = r * std::sin(theta) * std::sin(phi);
}

/// Convert spherical coordinates to cartesian 
inline void	spherical_to_cartesian ( float3 const& sph, float3& cart )
{
	spherical_to_cartesian(sph.x, sph.y, sph.z, cart); 
}

/// Transform a point using a matrix
inline float3 transform_point(float3 const& p, matrix const& m)
{
    float3 res = m * p;
    res.x += m.m03;
    res.y += m.m13;
    res.z += m.m23;
    return res;
}

/// Transform a vector using a matrix
inline float3 transform_vector(float3 const& v, matrix const& m)
{
    return m * v;
}

/// Transform a normal using a matrix.
/// Use this function carefully as the normal can be 
/// transformed much faster using transform matrix in many cases
/// NOTE: You need to pass inverted matrix for the transform
inline float3 transform_normal(float3 const& n, matrix const& minv)
{
    return minv.transpose() * n;
}

/// Transform a ray using a matrix
inline ray transform_ray(ray const& r, matrix const& m)
{
    return ray(transform_point(r.o, m), transform_vector(r.d, m), r.t);
}




/// Solve quadratic equation
/// Returns false in case of no real roots exist
/// true otherwise
bool	solve_quadratic( float a, float b, float c, float& x1, float& x2 );

/// Matrix transforms
matrix translation(float3 const& v);
matrix rotation_x(float ang);
matrix rotation_y(float ang);
matrix rotation_z(float ang);
matrix rotation(float3 const& axis, float ang);
matrix scale(float3 const& v);

/// This perspective projection matrix effectively maps view frustum to [-1,1]x[-1,1]x[0,1] clip space, i.e. DirectX depth
matrix perspective_proj_lh_dx(float l, float r, float b, float t, float n, float f);
matrix perspective_proj_lh_gl(float l, float r, float b, float t, float n, float f);

/// This perspective projection matrix effectively maps view frustum to [-1,1]x[-1,1]x[-1,1] clip space, i.e. OpenGL depth
matrix perspective_proj_rh_gl(float l, float r, float b, float t, float n, float f);

matrix perspective_proj_fovy(float fovy, float aspect, float n, float f);
matrix perspective_proj_fovy_lh_dx(float fovy, float aspect, float n, float f);
matrix perspective_proj_fovy_lh_gl(float fovy, float aspect, float n, float f);
matrix perspective_proj_fovy_rh_gl(float fovy, float& aspect, float n, float f);

matrix lookat_lh_dx( float3 const& pos, float3 const& at, float3 const& up);

/// Quaternion transforms
quaternion  rotation_quaternion(float3 const& axis, float angle);
float3      rotate_vector( float3 const& v, quaternion const& q );
quaternion  rotate_quaternion( quaternion const& v, quaternion const& q );

inline quaternion matrix_to_quaternion(matrix const& m)
{
    quaternion q;
    q.w = 0.5f*sqrt(m.trace());
    q.x = (m.m[2][1] - m.m[1][2])/(4*q.w);
    q.y = (m.m[0][2] - m.m[2][0])/(4*q.w);
    q.z = (m.m[1][0] - m.m[0][1])/(4*q.w);
    return q;
}

inline matrix quaternion_to_matrix(quaternion const& q)
{
    matrix m;
    float s = 2/q.norm();
    m.m[0][0] = 1 - s*(q.y*q.y + q.z*q.z); m.m[0][1] = s * (q.x*q.y - q.w*q.z);      m.m[0][2] = s * (q.x*q.z + q.w*q.y);      m.m[0][3] = 0;
    m.m[1][0] = s * (q.x*q.y + q.w*q.z);   m.m[1][1] = 1 - s * (q.x*q.x + q.z*q.z);  m.m[1][2] = s * (q.y*q.z - q.w*q.x);      m.m[1][3] = 0;
    m.m[2][0] = s * (q.x*q.z - q.w*q.y);   m.m[2][1] = s * (q.y*q.z + q.w*q.x);      m.m[2][2] = 1 - s * (q.x*q.x + q.y*q.y);  m.m[2][3] = 0;
    m.m[3][0] = 0;                         m.m[3][1] = 0;                            m.m[3][2] = 0;                            m.m[3][3] = 1;
    return m;
}

// Calculate vector orthogonal to a given one
inline float3 orthovector(float3 const& n)
{
    float3 p;
    if (fabs(n.z) > 0.707106781186547524401f) {
        float k = sqrt(n.y*n.y + n.z*n.z);
        p.x = 0; p.y = -n.z/k; p.z = n.y/k;
    }
    else {
        float k = sqrt(n.x*n.x + n.y*n.y);
        p.x = -n.y/k; p.y = n.x/k; p.z = 0;
    }
    return p;
}

// Map [0..1]x[0..1] value to unit hemisphere with pow e cos weighted pdf
inline float3 map_to_hemisphere(float3 const& n, float2 const& s, float e)
{
    float3 u = orthovector(n);

    float3 v = cross(u, n);
    u = cross(n, v);

    float sinpsi = sinf(2*PI*s.x);
    float cospsi = cosf(2*PI*s.x);
    float costheta = powf(1.f - s.y, 1.f/(e + 1.f));
    float sintheta = sqrt(1.f - costheta * costheta);

    return normalize(u * sintheta * cospsi + v * sintheta * sinpsi + n * costheta);
}

// Map [0..1]x[0..1] value to unit hemisphere with pow e cos weighted pdf
inline float3 map_to_hemisphere(float3 const& n, float2 const& s)
{
    float3 u = orthovector(n);
    
    float3 v = cross(u, n);
    u = cross(n, v);
    
    float sinpsi = sinf(2*PI*s.x);
    float cospsi = cosf(2*PI*s.x);
    
    float costheta = cosf(PI*s.y);
    float sintheta = sinf(PI*s.y);
    
    return normalize(u * sintheta * cospsi + v * sintheta * sinpsi + n * costheta);
}

// Map [0..1]x[0..1] value to triangle and return barycentric coords
inline float3 map_to_triangle(float2 const& s)
{
    return float3(1.f - sqrtf(s.x), sqrtf(s.x) * (1.f - s.y), sqrtf(s.x) * s.y);
}

// Checks if the float value is IEEE FP NaN
inline bool is_nan(float val)
{
    return val != val;
}

// Checks if one of arguments components is IEEE FP NaN
inline bool has_nans(float3 const& val)
{
    return is_nan(val.x) || is_nan(val.y) || is_nan(val.z); 
}

// Linearly interpolate two float3 values
inline float3 lerp(float3 const& v1, float3 const& v2, float s)
{
    return (1.f - s) * v1 + s * v2;
}

// Linearly interpolate two float3 values
inline void lerp(float3 const& v1, float3 const& v2, float s, float3& res)
{
    res.x = (1.f - s) * v1.x + s * v2.x;
    res.y = (1.f - s) * v1.y + s * v2.y;
    res.z = (1.f - s) * v1.z + s * v2.z;
}

// Linearly interpolate two float values
inline float lerp(float v1, float v2, float s)
{
    return (1.f - s) * v1 + s * v2;
}

// Multiple importance sampling power heuristic
inline float PowerHeuristic(int nf, float fpdf, int ng, float gpdf)
{
    float f = nf * fpdf;
    float g = ng * gpdf;
    return (f*f) / (f*f + g*g);
}


// Power of 2 rounding
inline unsigned long upper_power_of_two(unsigned long v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

#endif // MATHUTILS_H