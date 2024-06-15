#ifndef octree_collision_h__
#define octree_collision_h__

#include <limits>

#include "glm/glm.hpp"

#include "mesh_stats.h"
#include "octant.h"
#include "triangle_octant_key_pair_list.h"

namespace DigitalSculpt
{
#define NoCollisionDistance std::numeric_limits<float>::max()

    struct OctreeCollision
    {
        int triangleID; // triangle id the collision occurs
        float distance; // distance from start to collision point.
        glm::vec3 position; // of collision
        bool isCollision = false; // is there a collision
        int octantID;
    };
}

#endif // octree_collision_h__