#include "pointlight.h"

float3 PointLight::GetSample(ShapeBundle::Hit const& hit, float2 const& sample, float3& d, float& pdf) const
{
    // Light position is the only possible sample point for a point light
    d = p_ - hit.p;
    // It's probability density == 1
    pdf = 1.f;
    // Emissive power with squared fallof
    float d2inv = 1.f / d.sqnorm();
    //
    return e_ * d2inv;
}