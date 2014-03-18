//
//  main.cpp
//  BVHOQ
//
//  Created by Dmitry Kozlov on 05.10.13.
//  Copyright (c) 2013 Dmitry Kozlov. All rights reserved.
//
#include <chrono>
#include <cassert>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <limits>
#include <stdlib.h>


#include "BVH.h"
#include "SplitBVHBuilder.h"
#include "OCLBVHBackEnd.h"
#include "TestScene.h"

int main(int argc, const char * argv[])
{
    auto scene  = TestScene::Create();
    
    BVH bvh;
    SplitBVHBuilder builder(scene->GetVertices(), scene->GetVertexCount(), scene->GetIndices(), scene->GetIndexCount(), scene->GetMaterials(), 8U, 1U, 1.f, 1.f);
    
	static auto prevTime = std::chrono::high_resolution_clock::now();
    
    //std::shared_ptr<BVHAccelerator> accel = BVHAccelerator::CreateFromScene(*scene);
	builder.SetBVH(&bvh);
    builder.Build();
    
    // generate points
    BVH::Iterator* iter       = bvh.CreateDepthFirstIterator();
    BBox           rootBounds = bvh.GetNodeBbox(iter->GetNodeId());
    
    
    std::vector<vector3> points;
    float xrange = rootBounds.GetExtents().x() * 2;
    float yrange = rootBounds.GetExtents().y() * 2;
    float zrange = rootBounds.GetExtents().z() * 2;
    
    const int NUM_POINTS = 100000;
    for (int i = 0; i < NUM_POINTS; ++i)
    {
        float x = ((float)rand() / RAND_MAX) * xrange;
        float y = ((float)rand() / RAND_MAX) * yrange;
        float z = ((float)rand() / RAND_MAX) * zrange;
        
        vector3 p = rootBounds.GetMinPoint() - rootBounds.GetExtents() * 0.5f;
        p += vector3(x,y,z);
        points.push_back(p);
    }
    
    BVH::RayQueryStatistics globalStat;
    
    globalStat.maxDepthVisited = 0;
    globalStat.numNodesVisited = 0;
    globalStat.numTrianglesTested = 0;
    
    int globalRayHits = 0;
    int globalRayLeafHits = 0;
    
    const int NUM_RAYS = 100000;
    for (int i = 0; i < NUM_RAYS; ++i)
    {
        int idx1 = rand() % points.size();
        int idx2 = rand() % points.size();
        
        vector3 dir = normalize(points[idx2] - points[idx1]);
        vector3 origin = points[idx1];
        
        BVH::RayQuery q;
        q.o = origin;
        q.d = dir;
        q.t = std::numeric_limits<float>::max();
        
        BVH::RayQueryStatistics stat;
        bvh.CastRay(q, stat, scene->GetVertices(), scene->GetIndices());
        
        if (stat.hitBvh)
        {
            globalStat.numNodesVisited += stat.numNodesVisited;
            globalStat.numTrianglesTested += stat.numTrianglesTested;
            globalStat.maxDepthVisited += stat.maxDepthVisited;
            globalRayHits++;
            
            if (stat.numTrianglesTested > 0)
            {
                globalRayLeafHits++;
            }
        }
    }
    
    auto currentTime = std::chrono::high_resolution_clock::now();
	auto deltaTime   = std::chrono::duration_cast<std::chrono::duration<double> >(currentTime - prevTime);
    
    std::cout << "\nBuilding time " << deltaTime.count() << " secs\n";
    
    std::cout << "-------\n";
    std::cout << "BVH node count " << bvh.GetNodeCount() << "\n";
    std::cout << "Rays emitted: " << NUM_RAYS << "\n";
    std::cout << "Rays hit BVH root: " << globalRayHits << "\n";
    std::cout << "Rays hit BVH leaf: " << globalRayLeafHits << "\n";
    std::cout << "Avg nodes visited per ray: " << ((float)globalStat.numNodesVisited) / globalRayHits << "\n";
    std::cout << "Avg triangles tested per ray: " << ((float)globalStat.numTrianglesTested) / globalRayHits << "\n";
    std::cout << "Avg max depth visited per ray: " << ((float)globalStat.maxDepthVisited) / globalRayHits << "\n";
    
	return 0;
}


