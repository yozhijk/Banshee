#ifndef PHONG_H
#define PHONG_H

#include "material.h"
#include "../bsdf/lambert.h"
#include "../bsdf/perfect_reflect.h"
#include "../bsdf/fresnel.h"


///< Phong material provides the combination of Lambert + specular BRDFs
///< and is supposed to be used in simple renders and imported material translations
///<
class Phong : public Material
{
public:
    // If diffuse map is specified it is used as a diffuse color, otherwise diffuse color is used
    Phong (TextureSystem const& texturesys,
        float eta,
        float3 const& diffuse, 
        float3 const& specular, 
        std::string const& diffusemap = "",
        std::string const& normalmap = "")
        : Material(texturesys)
        , diffuse_(diffuse)
        , specular_(specular)
        , diffusemap_(diffusemap)
        , normalmap_(normalmap)
        , fresnel_(new FresnelDielectric())
        , diffusebsdf_(new Lambert())
        , specularbsdf_(new PerfectReflect())
        , eta_(eta)
    {
    }

    // Sample material and return outgoing ray direction along with combined BSDF value
    float3 Sample(Primitive::Intersection const& isect, float2 const& sample, float3 const& wi, float3& wo, float& pdf) const
    {
        // Copy to be able to alter normal
        Primitive::Intersection isectlocal = isect;

        float ndotwi = dot(wi, isectlocal.n);

        if (!normalmap_.empty())
        {
            MapNormal(normalmap_, isectlocal);
        }

        float3 kd = diffusemap_.empty() ? diffuse_ : texturesys_.Sample(diffusemap_, isect.uv, float2(0,0));
        float3 ks = specular_;
        
        float r = fresnel_->Evaluate(1.f, eta_, ndotwi);

        float rnd = rand_float();
        
        if (rnd < r)
        {
            return ks*specularbsdf_->Sample(isectlocal, sample, wi, wo, pdf);
        }
        else
        {
            return kd*diffusebsdf_->Sample(isectlocal, sample, wi, wo, pdf);
        }
    }

    // Evaluate combined BSDF value
    float3 Evaluate(Primitive::Intersection const& isect, float3 const& wi, float3 const& wo) const
    {
        // Copy to be able to alter normal
        Primitive::Intersection isectlocal = isect;

        float ndotwi = dot(wi, isectlocal.n);

        if (!normalmap_.empty())
        {
            MapNormal(normalmap_, isectlocal);
        }

        if (!normalmap_.empty())
        {
            MapNormal(normalmap_, isectlocal);
        }
        
        float3 kd = diffusemap_.empty() ? diffuse_ : texturesys_.Sample(diffusemap_, isect.uv, float2(0,0));
        float3 ks = specular_;
        
        float r = fresnel_->Evaluate(1.f, eta_, ndotwi);
        
        float rnd = rand_float();
        
        if (rnd < r)
        {
            return ks*specularbsdf_->Evaluate(isectlocal, wi, wo);
        }
        else
        {
            return kd*diffusebsdf_->Evaluate(isectlocal, wi, wo);
        }
    }

    float eta_;
    // Diffuse color
    float3 diffuse_;
    // Specular color
    float3 specular_;
    // Diffuse map
    std::string diffusemap_;
    // Normal map
    std::string normalmap_;
    // Fresnel component
    std::unique_ptr<Fresnel> fresnel_;
    // BSDFs
    std::unique_ptr<Bsdf> diffusebsdf_;
    std::unique_ptr<Bsdf> specularbsdf_;
};

#endif // PHONG_H