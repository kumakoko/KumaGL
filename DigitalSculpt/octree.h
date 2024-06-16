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
#include <cmath>

#include "mesh_stats.h"
#include "octant.h"
#include "octree_collision.h"
#include "octree_stats.h"
#include "vertex_id_hashing.h"


namespace DigitalSculpt
{
// #define MortonCodeConvert_Safe(MortonCodeVertexPosition, MortonCodeCenterPosition) (((((MortonCodeVertexPosition) == 0.0f) && std::signbit((MortonCodeVertexPosition))) ? 0.0f : (MortonCodeVertexPosition)) >= (MortonCodeCenterPosition))

    template<typename T>
    bool MortonCodeConvert_Safe(T MortonCodeVertexPosition, T MortonCodeCenterPosition) 
    {
        T adjustedPosition = (MortonCodeVertexPosition == 0.0 && std::signbit(MortonCodeVertexPosition)) ? 0.0 : MortonCodeVertexPosition;
        return adjustedPosition >= MortonCodeCenterPosition;
    }


    class Octree : public OctreeStats
    {
    public:
        /******************************************************************************************************************
         * Desc: 构建八叉树，把mesh中的所有三角形都插入到本树中，且初始化根八叉树元
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
         * Desc: Resize octree to fit the given triangle by creating a new root octant and placing the old root as a child
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
         * Desc: 根据本八叉树要负责的三角形个数，开辟空间，且先初始化一些值进去
         * Method:    loadTriangleOctantList
         * Returns:   void
         ****************************************************************************************************************/
        void loadTriangleOctantList();

        /******************************************************************************************************************
         * Desc: 
         * Method:    resizeOctreeParallel
         * Returns:   void
         * Parameter: int tri
         ****************************************************************************************************************/
         void resizeOctreeParallel(int tri);

 
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
         * Desc: 检查给定的三角形的三个顶点，是否都在给定的八叉树元内
         * Method:    isTriangleInOctantBounds
         * Returns:   bool 是true否false
         * Parameter: int tri 给定的三角形的索引值
         * Parameter: int octantID 给定的八叉树元的索引值
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

        /******************************************************************************************************************
         * Desc: Insert triangle into octree, using the triangleID. Corrects state and subdivides if necessary.
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
        bool isOriginInOctantBounds(glm::vec3 origin, Octant& octant);
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

        /// <summary>
        /// 本八叉树要管理的三角形的索引值
        /// </summary>
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