#ifndef octree_stats_h__
#define octree_stats_h__

#include "mesh_stats.h"

namespace DigitalSculpt
{
    class OctreeStats : public MeshStats
    {
    public:
        int octantTriangleLimit = 1000; // Max triangles per octant
        int octreeDepthLimit = 10;      // Max depth of octree
        int octreeDepth = 0;            // Depth of octree
        float octreeBuffer = 1.5f;      // Adds buffer to root bounds
        float octreeLooseness = 1.1f;   // Looseness of octant bounds to allow overlap at borders
        int octreeCurrentDepth = 0;     // Temp value to track current depth functions operate at
        int root = 0;           // Root octant index (Always 0, should define or make constant)
    };
}

#endif // octree_stats_h__
