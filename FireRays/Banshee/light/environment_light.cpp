#include "environment_light.h"

#include "../texture/texturesystem.h"
#include "../math/mathutils.h"

#include <cassert>

float3 EnvironmentLight::GetSample(ShapeBundle::Hit const& hit, float2 const& sample, float3& d, float& pdf) const
{
    // Precompute invpi
    float invpi = 1.f / PI;

    // Generate random hemispherical direction with cosine distribution
    d = map_to_hemisphere(hit.n, sample, 1.f);

    // PDF is proportional to dot(n,d) since the distribution
    // and normalized
    pdf = dot(hit.n, d) * invpi;

    // Convert world d coordinates to spherical representation
    float r, phi, theta;
    cartesian_to_spherical(d, r, phi, theta);

    // Compose the textcoord to fetch
    float2 uv(phi / (2*PI), theta / PI);

    // Make it long
    d *= 10000000.f;

    // Fetch the value
    float3 val = texsys_.Sample(texture_, uv, float2(0,0));
    
    // Apply gamma correction
    val = float3(pow(val.x, invgamma_), pow(val.y, invgamma_), pow(val.z, invgamma_));
    
    // Fetch radiance value and scale it
    return scale_ * val;
}


float3 EnvironmentLight::GetLe(ray const& r) const
{
     // Convert world d coordinates to spherical representation
    float rr, phi, theta;
    cartesian_to_spherical(r.d, rr, phi, theta);

    // Compose the textcoord to fetch
    float2 uv(phi / (2*PI), theta / PI);

    // Fetch the value
    float3 val = texsys_.Sample(texture_, uv, float2(0,0));
    
    // Apply gamma correction
    val = float3(pow(val.x, invgamma_), pow(val.y, invgamma_), pow(val.z, invgamma_));
    
    // Fetch radiance value and scale it
    return scale_ * val;
}

// PDF of a given direction sampled from isect.p
float EnvironmentLight::GetPdf(ShapeBundle::Hit const& hit, float3 const& w) const
{
    // Precompute invpi
    float invpi = 1.f / PI;
    
    // PDF is proportional to dot(n,d)
    return dot(hit.n, normalize(w)) * invpi;
}