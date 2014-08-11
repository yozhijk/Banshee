#ifndef INDEXED_TRIANGLE_H
#define INDEXED_TRIANGLE_H

#include "primitive.h"
#include "../math/float3.h"
#include "../math/float2.h"
#include "../math/bbox.h"

class Mesh;

///< IndexedTriangle stores indices of the position/normal/uv data
///< along with a pointers to their storages
///<
class IndexedTriangle : public Primitive
{
public:
    // Constructor
    IndexedTriangle(unsigned pidx1, unsigned pidx2, unsigned pidx3,
                    unsigned nidx1, unsigned nidx2, unsigned nidx3,
                    unsigned tidx1, unsigned tidx2, unsigned tidx3,
                    unsigned m, Mesh const* mesh)
                    : pidx1_(pidx1), pidx2_(pidx2), pidx3_(pidx3)
                    , nidx1_(nidx1), nidx2_(nidx2), nidx3_(nidx3)
                    , tidx1_(tidx1), tidx2_(tidx2), tidx3_(tidx3)
                    , m_(m), mesh_(mesh)
    {
    }

    // Intersection override
    bool Intersect(ray& r, float& t, Intersection& isect) const;
    // Intersection check override
    bool Intersect(ray& r) const;
    // Bounding box override
    bbox Bounds() const;

private:
    // Data indices
    unsigned pidx1_, pidx2_, pidx3_;
    unsigned nidx1_, nidx2_, nidx3_;
    unsigned tidx1_, tidx2_, tidx3_;
    // Material index
    unsigned m_;

    // Pointer to parent mesh
    Mesh const* mesh_;
};

#endif // INDEXED_TRIANGLE_H    