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

    /******************************************************************************************************************
     * Desc: "要编码的顶点的位置向量的分量"是否小于“要编码的八叉树节点的中点位置的分量”。是的话返回false，不是返回true
     * Method:    MortonCodeConvert_Safe
     * Returns:   bool 
     * Parameter: float MortonCodeVertexPosition 要编码的顶点的位置向量的分量
     * Parameter: float MortonCodeCenterPosition 要编码的八叉树节点的中点位置的分量
     ****************************************************************************************************************/
    inline bool MortonCodeConvert_Safe(float MortonCodeVertexPosition, float MortonCodeCenterPosition)
    {
        // https://c-cpp.com/cpp/numeric/math/signbit
        // std::signbit 确定给定的浮点数是否为负,浮点数0.0有正负之分
        // signbit(+0.0) 返回 false
        // signbit(-0.0) 返回 true
        float adjustedPosition = (MortonCodeVertexPosition == 0.0f && std::signbit(MortonCodeVertexPosition)) ? 0.0f : MortonCodeVertexPosition;
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
         * Desc: 细分给定的八叉树节点。将子节点添加到给定的八叉树节点和及其子节点列表。如果子节点需要细分，则可以递归。
         * Method:    subdivideOctant
         * Returns:   void
         * Parameter: int octantID 待细分的八叉树节点的编号
         ****************************************************************************************************************/
        void subdivideOctant(int octantID);

        /******************************************************************************************************************
         * Desc: 给索引值为parentIndex的八叉树节点，创建一个子节点。
         * 将这个子节点添加到节点列表。
         * 根据八叉树的松散程度调整子节点的中心和尺寸。
         *
         * 注意：可以通过预先计算未调整和松散的半尺寸进行优化，因为这些尺寸在节点的所有子节点之间共享。
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

        int mortonCodeHash(const glm::vec3& point, const glm::vec3& center); // returns the morton code position with respect to octant

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