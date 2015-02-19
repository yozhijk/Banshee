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
#ifndef BVH_H
#define BVH_H

#include <memory>
#include <vector>

#include "../math/bbox.h"
#include "../primitive/primitive.h"

///< The class represents bounding volume hierarachy
///< intersection accelerator
///<
class Bvh : public Primitive
{
public:
    Bvh()
    : root_(nullptr)
    {
    }
    // Intersection test
    bool Intersect(ray& r, float& t, Intersection& isect) const;
    // Intersection check test
    bool Intersect(ray& r) const;
    // World space bounding box
    bbox Bounds() const;
    // Build function
    void Build(std::vector<Primitive*> const& prims);
    
    // Query BVH statistics
    struct Statistics;
    void QueryStatistics(Statistics& stat) const;
    
    
protected:
    // Build function
    virtual void BuildImpl(std::vector<Primitive*> const& prims);
    // BVH node
    struct Node;
    // Enum for node type
    enum NodeType
    {
        kInternal,
        kLeaf
    };
    
    // Primitves owned by this instance
    std::vector<std::unique_ptr<Primitive> > primitive_storage_;
    // Primitves to intersect (includes refined prims)
    std::vector<Primitive*> primitives_;
    // BVH nodes
    std::vector<std::unique_ptr<Node> > nodes_;
    // Bounding box
    bbox bounds_;
    // Root node
    Node* root_;
    
private:
    Bvh(Bvh const&);
    Bvh& operator = (Bvh const&);
};

struct Bvh::Node
{
    // Node bounds in world space
    bbox bounds;
    // Type of the node
    NodeType type;
    union
    {
        // For internal nodes: left and right children
        struct
        {
            Node* lc;
            Node* rc;
        };
        
        // For leaves: starting primitive index and number of primitives
        struct
        {
            int startidx;
            int numprims;
        };
    };
};

struct Bvh::Statistics
{
    int internalcount;
    int leafcount;
    float minoverlaparea;
    float maxoverlaparea;
    float avgoverlaparea;
};

#endif // BVH_H