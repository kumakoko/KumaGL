#pragma once

#include "mesh_stats.h"

namespace DigitalSculpt
{
    class OctreeStats : public MeshStats
    {
    public:
        int octantTriangleLimit = 1000; // Max triangles per octant
        int octreeDepthLimit = 10;      // 八叉树结构的最大树深度 Max depth of octree
        int octreeDepth = 0;            // 八叉树当前的深度，即树状结构分了几层 Depth of octree
        float octreeBuffer = 1.5f;      // Adds buffer to root bounds
        float octreeLooseness = 1.1f;   // Looseness of octant bounds to allow overlap at borders
        int octreeCurrentDepth = 0;     // Temp value to track current depth functions operate at
        int root = 0;           // Root octant index (Always 0, should define or make constant)
    };
}