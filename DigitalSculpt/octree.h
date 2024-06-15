#pragma once

#include <chrono>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_set>
#include <concurrent_vector.h>
#include <concurrent_unordered_set.h>
#include <concurrent_queue.h>
#include <random>

#include "mesh_stats.h"
#include "octant.h"
#include "octree_collision.h"
#include "octree_stats.h"
#include "vertex_id_hashing.h"


namespace DigitalSculpt
{
#define MortonCodeConvert_Safe(MortonCodeVertexPosition, MortonCodeCenterPosition) (((((MortonCodeVertexPosition) == 0.0f) && std::signbit((MortonCodeVertexPosition))) ? 0.0f : (MortonCodeVertexPosition)) >= (MortonCodeCenterPosition))

    template<typename T1,typename T2>
    inline float MortonCodeConvert_Safe(T1 MortonCodeVertexPosition, T2 MortonCodeCenterPosition)
    {
        return (((((MortonCodeVertexPosition) == 0.0f) && std::signbit((MortonCodeVertexPosition))) ? 0.0f : (MortonCodeVertexPosition)) >= (MortonCodeCenterPosition));
    }


    class Octree : public OctreeStats
    {
    public:
        // Octree.cpp
        /******************************************************************************************************************
         * Desc:
         * Method:    buildOctree
         * Returns:   void
         ****************************************************************************************************************/
        void buildOctree();
        /******************************************************************************************************************
         * Desc:
         * Method:    testOctree
         * Returns:   void
         ****************************************************************************************************************/
        void testOctree();
        /******************************************************************************************************************
         * Desc:
         * Method:    octreePrintStats
         * Returns:   void
         ****************************************************************************************************************/
        void octreePrintStats();
        /******************************************************************************************************************
         * Desc:
         * Method:    resizeOctree
         * Returns:   void
         * Parameter: int tri
         ****************************************************************************************************************/
        void resizeOctree(int tri);
        /******************************************************************************************************************
         * Desc:
         * Method:    clearOctree
         * Returns:   void
         ****************************************************************************************************************/
        void clearOctree();
        /******************************************************************************************************************
         * Desc:
         * Method:    rebuildOctree
         * Returns:   void
         ****************************************************************************************************************/
        void rebuildOctree();
        /******************************************************************************************************************
         * Desc:
         * Method:    loadTriangleOctantList
         * Returns:   void
         ****************************************************************************************************************/
        void loadTriangleOctantList();

        // Parallel Octree functions
        void resizeOctreeParallel(int tri);

        // OctreeOctant.cpp
        /******************************************************************************************************************
         * Desc:
         * Method:    subdivideOctant
         * Returns:   void
         * Parameter: int octantID
         ****************************************************************************************************************/
        void subdivideOctant(int octantID);
        /******************************************************************************************************************
         * Desc:
         * Method:    createChildOctant
         * Returns:   void
         * Parameter: OctantPosition octantPosition
         * Parameter: int parentIndex
         ****************************************************************************************************************/
        void createChildOctant(OctantPosition octantPosition, int parentIndex);
        /******************************************************************************************************************
         * Desc:
         * Method:    findOctantForTriangle
         * Returns:   int
         * Parameter: int tri
         ****************************************************************************************************************/
        int findOctantForTriangle(int tri);
        /******************************************************************************************************************
         * Desc:
         * Method:    isTriangleInOctantBounds
         * Returns:   bool
         * Parameter: int tri
         * Parameter: int octantID
         ****************************************************************************************************************/
        bool isTriangleInOctantBounds(int tri, int octantID);

        // Parallel Octant Functions
        /******************************************************************************************************************
         * Desc:
         * Method:    insertOctantParallel
         * Returns:   int
         * Parameter: Octant & octant
         ****************************************************************************************************************/
        int insertOctantParallel(Octant& octant);
        /******************************************************************************************************************
         * Desc:
         * Method:    createChildOctantParallel
         * Returns:   int
         * Parameter: OctantPosition octantPosition
         * Parameter: int parentIndex
         ****************************************************************************************************************/
        int createChildOctantParallel(OctantPosition octantPosition, int parentIndex);
        void subdivideOctantParallel(int oix, int localDepth);
        /******************************************************************************************************************
         * Desc:
         * Method:    findOctantForTriangleParallel
         * Returns:   std::pair<int, int>
         * Parameter: int triangle
         * Parameter: int start
         ****************************************************************************************************************/
        std::pair<int, int> findOctantForTriangleParallel(int triangle, int start);

        // OctreeElements.cpp
        /******************************************************************************************************************
         * Desc:
         * Method:    insertTriangle
         * Returns:   bool
         * Parameter: int tri
         ****************************************************************************************************************/
        bool insertTriangle(int tri);
        /******************************************************************************************************************
         * Desc:
         * Method:    insertTriangles
         * Returns:   bool
         ****************************************************************************************************************/
        bool insertTriangles();
        /******************************************************************************************************************
         * Desc:
         * Method:    updateTriangleInOctree
         * Returns:   bool
         * Parameter: int tri
         ****************************************************************************************************************/
        bool updateTriangleInOctree(int tri);
        /******************************************************************************************************************
         * Desc:
         * Method:    updateTrianglesInOctree
         * Returns:   bool
         * Parameter: std::vector<int> & tris
         ****************************************************************************************************************/
        bool updateTrianglesInOctree(std::vector<int>& tris);
        /******************************************************************************************************************
         * Desc:
         * Method:    removeTriangleFromOctree
         * Returns:   bool
         * Parameter: int tri
         ****************************************************************************************************************/
        bool removeTriangleFromOctree(int tri);
        /******************************************************************************************************************
         * Desc:
         * Method:    octreeReinsertTriangles
         * Returns:   void
         ****************************************************************************************************************/
        void octreeReinsertTriangles();
        /******************************************************************************************************************
         * Desc:
         * Method:    updateAffectedTriangles
         * Returns:   void
         ****************************************************************************************************************/
        void updateAffectedTriangles();
        /******************************************************************************************************************
         * Desc:
         * Method:    clearCollision
         * Returns:   void
         ****************************************************************************************************************/
        void clearCollision();

        // Parallel OctreeElements functions
        /******************************************************************************************************************
         * Desc:
         * Method:    insertTriangleParallel
         * Returns:   bool
         * Parameter: int tri
         ****************************************************************************************************************/
        bool insertTriangleParallel(int tri);
        /******************************************************************************************************************
         * Desc:
         * Method:    insertTrianglesParallel
         * Returns:   bool
         ****************************************************************************************************************/
        bool insertTrianglesParallel();
        /******************************************************************************************************************
         * Desc:
         * Method:    octreeReinsertTrianglesParallel
         * Returns:   void
         ****************************************************************************************************************/
        void octreeReinsertTrianglesParallel();
        /******************************************************************************************************************
         * Desc:
         * Method:    updateTriangleInOctreeParallel
         * Returns:   bool
         * Parameter: int tri
         ****************************************************************************************************************/
        bool updateTriangleInOctreeParallel(int tri);
        /******************************************************************************************************************
         * Desc:
         * Method:    updateTrianglesInOctreeParallel
         * Returns:   bool
         * Parameter: std::vector<int> tris
         ****************************************************************************************************************/
        bool updateTrianglesInOctreeParallel(std::vector<int> tris);
        /***************************************************
        
        @name: DigitalSculpt::Octree::removeTriangleFromOctreeParallel
        @return: bool
        @param: int tri
        ***************************************************/
        bool removeTriangleFromOctreeParallel(int tri);
        /***************************************************
        
        @name: DigitalSculpt::Octree::updateAffectedTrianglesParallel
        @return: void
        ***************************************************/
        void updateAffectedTrianglesParallel();

        // OctreeIntersection.cpp
        std::vector<GLuint> collectVerticesAroundCollisionOriginal(OctreeCollision collision, float range);
        std::vector<int> collectTrianglesAroundCollisionOriginal(OctreeCollision collision, float range);
        OctreeCollision octreeRayIntersectionOriginal(glm::vec3 origin, glm::vec3 direction);

        void collectVerticesAroundCollision(float range);
        void collectTrianglesAroundCollision(float range);

        void octreeRayIntersectionCore(glm::vec3 origin, glm::vec3 direction, OctreeCollision& collisionRef);
        void octreeRayIntersection(glm::vec3 origin, glm::vec3 direction, bool isSymmetric = false, glm::vec3 planeOrigin = glm::vec3(0), glm::vec3 planeNormal = glm::vec3(1, 0, 0));
        bool isOriginInOctantBounds(glm::vec3 origin, OctantReference octant);
        void reflectRay(glm::vec3& origin, glm::vec3& direction, glm::vec3& planeOrigin, glm::vec3& planeNormal, glm::vec3& reflectOrigin, glm::vec3& reflectDirection);

        void collectAroundCollision(float range, bool collectAffectedTriangles = true, bool collectTrianglesInRange = false, bool isSymmetric = false);
        void collectVerticesWithReflection(float range);

        int mortonCodeHash(glm::vec3 point, glm::vec3 center); // returns the morton code position with respect to octant

        // List of octants which contain triangles
        // OctantIndexList activeOctants;

        concurrency::concurrent_vector<Octant> octants;
        // OctantList octants;
        // OctantList leaves;

        OctreeCollision collision;
        std::vector<GLuint> verticesInRange;
        std::vector<int> affectedTriangles;
        std::vector<int> trianglesInRange;

        OctreeCollision reflectedCollision;
        std::vector<GLuint> reflectedVerticesInRange;
        std::vector<int> reflectedTrianglesInRange;

        concurrency::concurrent_vector<int> triangleToOctantList;

        // Plane normals may be wrong here, need to double check
        glm::vec3 planeNormals[3] = 
        {
            {0, 0, 1}, // xy plane
            {1, 0, 0}, // yz plane
            {0, 1, 0}  // xz plane
        };
    };

} // namespace OctreeDefinition