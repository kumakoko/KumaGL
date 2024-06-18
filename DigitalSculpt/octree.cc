/*
八叉树（Octree）是一种常用于三维空间数据组织的树形数据结构，其中每个节点最多有八个子节点。
这种结构在计算机图形学、空间索引、以及其他需要高效空间划分的领域中非常有用。在这些应用中，
Morton码（也称为Z-order曲线）是一种将多维数据转换为一维数据的方法，用于维护元素在多维空
间中的邻近性，同时可以简化数据结构的访问和修改。

Morton Code（Z-order曲线）:
Morton码是一种空间填充曲线，它将多维数据映射到一维数据，同时尽可能保持原始数据中的局部性。
这种映射是通过交织各维度的坐标位来实现的。

如何生成Morton码:
以三维空间为例，如果有一个点的坐标是 (x, y, z)，Morton码可以通过以下步骤生成：

将每个坐标值（x, y, z）的二进制表示进行扩展，使得每个二进制位之间有空位（即从 xyzb 变为 x0y0z0b0）。
将扩展后的坐标位交织在一起，形成一个单一的二进制数。例如，扩展的坐标位可能是x0y0z0，然后交织为
xyzxyz...，这就形成了Morton码。

应用:
Morton码的一个主要优势是它保持了数据的空间局部性。在八叉树等数据结构中，接近的点在Morton码中也往往
是数值接近的，这使得空间查询变得更加高效。因此，Morton码在以下场景中特别有用：

1 空间索引和查询，如快速查找空间中的点云数据。
2 多维数据的排序和存储，以优化访问模式和缓存利用。
3 在并行计算中，通过Morton码可以有效地将数据分割和分配给多个处理单元，减少数据处理中的通信需求。

总之，Morton码是一种强大的工具，用于在八叉树和其他层次结构中管理和优化三维空间数据。
*/

#include "octree.h"
#include "spdlog/spdlog.h"
#include "glm/gtx/component_wise.hpp"
#include "glm/gtx/string_cast.hpp"
#include "vertex_id_hashing.h"
#include "utils.h"

namespace DigitalSculpt
{
    concurrency::concurrent_vector<std::unique_ptr<std::mutex>> octantMutexes;
    /**
     * @brief Print octree information for debugging.
     */
    void Octree::octreePrintStats()
    {
        spdlog::info("Octree Debug\n");
        spdlog::info("\tVertex Limit: {} \n", this->octantTriangleLimit);
        spdlog::info("\tDepth Limit: {} \n", this->octreeDepthLimit);;
        spdlog::info("\tOctree Buffer Size: {} \n", this->octreeBuffer);;
        spdlog::info("\tOctree Looseness: {} \n", this->octreeLooseness);;
        spdlog::info("\tDepth {} \n", this->octreeDepth);;
        spdlog::info("\tCenter: {} \n", glm::to_string(octants[root].octantCenter));
        spdlog::info("\tHalfsize: {} \n", octants[root].octantHalfSize);;
        spdlog::info("\tVertices: {} \n", verticeCount());
        spdlog::info("\tTriangles: {} \n", totalTriangles());
        spdlog::info("\tOctants: {} \n", octants.size());
        spdlog::info("\tTriangles in Root: {} \n", octants[root].triangleIDs->size());
        // say "\tLeaves:" spc leaves.size() \n";


        int count = 0; // Counts leaves and pseudo-leaves
        int owt = 0;   // Octants with triangles
        int pwt = 0;   // Triangles in pseudo leaves

        for (auto& o : octants)
        {
            if (o.octantState)
            {
                count++;
            }

            if (o.triangleIDs->size())
            {
                owt++;
                if (o.octantState == OctantState::OctantNotEmptyInternal)
                {
                    pwt++;
                }
            }
        }

        // Ignore for now
        spdlog::info("\tActive Octants: {} \n", count);
        spdlog::info("\tOctants w/ Triangles: {} \n", owt);
        spdlog::info("\tPsuedo-Leaves w/ Traingles: {} \n", pwt);
    }

    /**
     * @brief Tests various octree functions.
     * Same ideas can be used for unit testing.
     *
     */
    void Octree::testOctree()
    {
        this->octreePrintStats();

        // Testing Intersection and Collection
        glm::vec3 rayOrigin(0, 0, 3);
        glm::vec3 rayDirection(0, 0, -1);
        // OctreeCollision collision = this->octreeRayIntersection(rayOrigin, rayDirection);
        if (collision.isCollision)
        {
            spdlog::info("Collision: \n");
            spdlog::info("\tRay Origin: {} \n", glm::to_string(rayOrigin));
            spdlog::info("\tRay Direction: {} \n ", glm::to_string(rayDirection));
            spdlog::info("\tIntersected Triangle: {} \n\t\t< {}, {}, {}>\n", collision.triangleID
                , glm::to_string(vertices[triangles[collision.triangleID][0]].position)
                , glm::to_string(vertices[triangles[collision.triangleID][1]].position)
                , glm::to_string(vertices[triangles[collision.triangleID][2]].position));
            spdlog::info("\tIntersection Point: {} \n", glm::to_string(collision.position));
        }

        // Testing removal
        int count = 0;

        for (auto& octant : octants)
        {
            count += (int)octant.triangleIDs->size();
        }

        spdlog::info("Triangles in Octree: {}\n", count);
        spdlog::info("Triangles in Mesh (should be same ^): {}\n", totalTriangles());

        for (int i = 0; i < totalTriangles(); i++)
        {
            removeTriangleFromOctree(i);
        }

        count = 0;

        for (auto& octant : octants)
        {
            count += (int)octant.triangleIDs->size();
        }

        spdlog::info("Triangles in Octree after Removal (should be 0): {} \n", count);
        spdlog::info("---Clearing Octree---\n");

        clearOctree();

        spdlog::info("\t# of Octants (should be 0): {} \n", octants.size());
        spdlog::info("---Rebuilding Octree--- \n");
        spdlog::info("Should be same as first build \n");

        rebuildOctree();
        octreePrintStats();
    }

    /**
     * @brief Build the octree by inserting all triangles of the mesh into the octree.
     * Initializes the root octant.
     */
    void Octree::buildOctree()
    {
        // 根据本八叉树要负责的三角形个数，开辟空间，且先初始化一些值进去
        loadTriangleOctantList();

        // 算出几何中心点到物体包络盒最大点的距离值，以及几何中心点到物体包络盒最小点的距离值
        float maxExtent = glm::compMax(abs(this->center - limMax));
        float minExtent = glm::compMin(abs(this->center - limMin));

        // 先创建出作为根节点的八叉树元
        Octant rootOctant;
        rootOctant.octantIndex = 0;
        rootOctant.octantCenter = this->center;
        rootOctant.octantHalfSize = glm::max(maxExtent, minExtent) * octreeBuffer;
        octants.push_back(rootOctant);
        octantMutexes.push_back(std::make_unique<std::mutex>());

        // 用并行操作的方法把所有的三角形插入到本八叉树内
        insertTrianglesParallel();
    }

    /**
     * @brief Clear the octree and reset appropriate values.
     *
     */
    void Octree::clearOctree()
    {
        octants.clear();
        octantMutexes.clear();
        octreeDepth = 0;
        octreeCurrentDepth = 0;
        root = 0; // This mother fucker wasn't here, caused crashing when octree was resized then rebuilt
        triangleToOctantList.clear();
    }

    /**
     * @brief Rebuild the octree for the mesh.
     * 1. Clears the octree
     * 2. Recollects stats of the mesh
     * 3. Builds the octree
     *
     */
    void Octree::rebuildOctree()
    {
        clearOctree();
        collectStats();
        buildOctree();
    }

    void Octree::loadTriangleOctantList()
    {
        // 根据本八叉树要负责的三角形个数，开辟空间，且先初始化一些值进去
        const int triCount = totalTriangles();
        triangleToOctantList.reserve(triCount);

        for (int i = 0; i < triCount; i++)
        {
            triangleToOctantList.push_back(-1);
        }
    }

    /**
     * @brief Resize octree to fit the given triangle by creating a new root
     * octant and placing the old root as a child
     *
     * @param tri
     */
    void Octree::resizeOctree(int tri)
    {
        spdlog::info("resizing octree \n");
        glm::vec3 triangleCentroid = getTriangleCentroid(tri); // 获取到三角形的质心点

        // Continuously resize until triangle fits into the root octant
        // 从八叉树的根八叉树元开始，判断给定的三角形是否落在本八叉树元中，不是的话就递归进去
        while (!isTriangleInOctantBounds(tri, root))
        {
            // If the depth limit is currently reached, increase limit to allow resizing
            if (octreeDepth == octreeDepthLimit)
            {
                octreeDepth++;
                octreeDepthLimit++;
            }

            // 缓存一下根八叉树元
            Octant oldRoot = octants[root];

            // 根据三角形的质心位置，和根八叉树元的中心位置，求出要插入新的八叉树元（即增加新的八叉树子节点）的方向
            OctantPosition newRootDirection = (OctantPosition)mortonCodeHash(triangleCentroid, oldRoot.octantCenter);

            // 拿到新根的方向
            glm::vec3 newRootPositionVector = octantPositionVectors[newRootDirection]; //  Vector from old root to new root

            // 调整八叉树元的半长值
            // New Root 的半长，已经很宽松，无需调整
            float oldRootAdjustedHalfSize = (float)(oldRoot.octantHalfSize / octreeLooseness);
            float newRootHalfsize = oldRootAdjustedHalfSize * 2;

            // 调整旧根节点的中心点得到新中心点，根据的是旧根节点中心值和调整过的节点半长值
            glm::vec3 oldRootAdjustedCenter = oldRoot.octantCenter - newRootPositionVector * (oldRoot.octantHalfSize - oldRootAdjustedHalfSize);
            glm::vec3 newRootCenter = oldRootAdjustedCenter + newRootPositionVector * oldRootAdjustedHalfSize;

            // 创建一个新的八叉树根节点
            Octant newRoot;
            newRoot.octantState = OctantEmptyInternal;
            newRoot.octantHalfSize = newRootHalfsize;
            newRoot.octantCenter = newRootCenter;
            newRoot.octantIndex = this->root;
            octants[root] = newRoot;

            // Subdivide the new root to add missing child octants
            // 针对根节点进行细分，添加丢失的子八叉树元
            subdivideOctant(root);

            oldRoot.parent = root; // Set the old root's parent to the new root

            // Get the position where the old root will reside in the new root's children
            // Then get the index of child to replace
            // Then replace octant at the index with the old root
            OctantPosition oldRootNewPosition = (OctantPosition)mortonCodeHash(oldRootAdjustedCenter, newRootCenter);
            int oldRootNewIndex = octants[root].children[oldRootNewPosition];
            oldRoot.octantIndex = oldRootNewIndex;
            octants[oldRootNewIndex] = oldRoot;

            for (auto& triangleID : *(oldRoot.triangleIDs))
            {
                triangleToOctantList[triangleID] = oldRootNewIndex;
            }

            // Adjust the parent values of the old roots children to point to its new index
            if (oldRoot.octantState != OctantLeaf)
            {
                for (auto& child : oldRoot.children)
                {
                    octants[child].parent = oldRootNewIndex;
                }
            }
        }
    }

    /**
     * @brief Resize octree to fit the given triangle by creating a new root
     * octant and placing the old root as a child
     *
     * @param tri
     */
    std::mutex mtxResize;
    void Octree::resizeOctreeParallel(int tri)
    {
        if (tri >= (int)triangleToOctantList.size())
        {
            spdlog::info("Triangle ID out of bounds \n");
            return;
        }

        // Only one thread can resize at a time
        std::lock_guard<std::mutex> lock(mtxResize);

        glm::vec3 triangleCentroid = getTriangleCentroid(tri); // Get the triangle's centroid

        // Loop until triangle fits
        while (!isTriangleInOctantBounds(tri, root))
        {
            if (octreeDepth == octreeDepthLimit)
            {
                octreeDepth++;
                octreeDepthLimit++;
            }
            Octant& oldRoot = octants[root]; // Save oldRoot to replace one of the new root's children

            // Get morton code of triangle centroid to determine direction to grow octree
            OctantPosition newRootDirection = (OctantPosition)mortonCodeHash(triangleCentroid, oldRoot.octantCenter);
            glm::vec3 newRootPositionVector = octantPositionVectors[newRootDirection]; // Vector from old root to new root

            // Adjusted halfsize removing looseness
            // New Root's halfsize, already loose no need for adjustment
            float oldRootAdjustedHalfSize = (float)(oldRoot.octantHalfSize / octreeLooseness);
            float newRootHalfsize = oldRootAdjustedHalfSize * 2;

            // Adjusted old root center, based on old root's center and adjusted half size
            // New Root's Center based on old root's center and adjusted half size
            // Obtain morton code of old root in relation to new root
            glm::vec3 oldRootAdjustedCenter = oldRoot.octantCenter - newRootPositionVector * (oldRoot.octantHalfSize - oldRootAdjustedHalfSize);
            glm::vec3 newRootCenter = oldRootAdjustedCenter + newRootPositionVector * oldRootAdjustedHalfSize;
            OctantPosition oldRootNewPosition = (OctantPosition)mortonCodeHash(oldRootAdjustedCenter, newRootCenter);

            Octant newRoot;                                          // Initialize new root
            newRoot.octantState = OctantEmptyInternal;               // Assign new root state
            newRoot.octantHalfSize = newRootHalfsize;                // Assign new root halfsize
            newRoot.octantCenter = newRootCenter;                    // Assign new root center
            newRoot.children[oldRootNewPosition] = root;             // Place old root as child
            int octantIndex = insertOctantParallel(newRoot); // insert new root into octree
            octants[root].parent = octantIndex;                      // Set old root's parent to new root

            // Create new root's children
            for (int i = 0; i < 8; i++)
            {
                if (i == oldRootNewPosition) // Skip old root's position
                {
                    continue;
                }
                octants[octantIndex].children[i] = createChildOctantParallel((OctantPosition)i, octantIndex);
                // int childIndex = createChildOctantParallel((OctantPosition)i, octantIndex);
                // octantMutexes[childIndex]->unlock();
            }
            root = octantIndex; // Set root to new root
        }
    }

    extern concurrency::concurrent_vector<std::unique_ptr<std::mutex>> octantMutexes;
    /**
     * @brief Insert triangle into octree, using the triangleID. Corrects state and subdivides if necessary.
     *
     * @param tri
     * @return true
     * @return false
     */
    bool Octree::insertTriangle(int tri)
    {
        // expand triangletooctantlist to match the number of triangles
        if (tri >= static_cast<int>(triangleToOctantList.size()))
        {
            int diff = tri - triangleToOctantList.size();
            for (int i = 0; i <= diff; i++)
            {
                triangleToOctantList.push_back(NonExistentOctantIndex);
            }
        }

        // fuck it, just use the parallel version. don't wanna rewrite this.
        return insertTriangleParallel(tri);
    }

    /**
     * @brief Remove triangle from octree
     *
     * @param tri
     * @return true
     * @return false
     */
    bool Octree::removeTriangleFromOctree(int tri)
    {
        // Get octant that triangle is in
        int oix = triangleToOctantList[tri];

        // If triangle is not in an octant, return false
        if (oix == NonExistentOctantIndex)
        {
            return false;
        }

        Octant& octantRef = octants[oix]; // Get the octant reference for clarity

        const int triangleIDsSize = (int)octantRef.triangleIDs->size();

        if (octants[oix].triangleIDs->find(tri) != octants[oix].triangleIDs->end())
        {
            octants[oix].triangleIDs->erase(tri);
        }

        return false; // Triangle was not found in the octant
    }

    void Octree::updateAffectedTriangles()
    {
        updateAffectedTrianglesParallel(); // testing
    }

    bool Octree::insertTriangleParallel(int tri)
    {
        // 插入的三角形编号不能大于待插入的三角形个数值
        if (tri > static_cast<int>(triangleToOctantList.size()))
        {
            return false;
        }

        // 检查编号为tri的三角形，是否已经存在于本八叉树中
        if (!Octree::isTriangleInOctantBounds(tri, root))
        {
            // 不存在的话，就以并行操作的方式插入到八叉树中
            resizeOctreeParallel(tri);
        }

        int oix = root;

        // Loop until the triangle is inserted into the correct octant
        while (true)
        {
            std::pair<int, int> foundPair = findOctantForTriangleParallel(tri, oix); // Find the octant which encloses the triangle
            oix = foundPair.first;                                                      // Get the octant index
            int localDepth = foundPair.second;                                          // Get the local depth of the octant
            octantMutexes[oix]->lock();

            // Validate the octant
            if (octants[oix].octantState != OctantLeaf)
            {
                int newOix = findOctantForTriangleParallel(tri, oix).first;
                if (newOix != oix)
                {
                    octantMutexes[oix]->unlock();
                    oix = newOix;
                    continue;
                }
            }

            // Octant& octant = octants[oix]; // Get the octant reference
            octants[oix].triangleIDs->emplace(tri); // Add the triangle to the octant's triangle list
            triangleToOctantList[tri] = oix;        // Set triangle's octant index to the new octant index

            // If octant is internal and empty, change state to not empty internal
            if (octants[oix].octantState == OctantEmptyInternal)
            {
                octants[oix].octantState = OctantNotEmptyInternal;
            }
            // Else If octant is a leaf, check depth and subdivide if # of triangles held exceeds limit
            else if (octants[oix].octantState == OctantLeaf &&
                localDepth < octreeDepthLimit &&
                static_cast<int>(octants[oix].triangleIDs->size()) > octantTriangleLimit)
            {
                subdivideOctantParallel(oix, localDepth);
            }

            octantMutexes[oix]->unlock();
            return true;
        }
    }

    bool Octree::insertTrianglesParallel()
    {
        int nThreads = getNThreads();
        int triangleCount = totalTriangles();
        std::vector<std::thread> threads;

        // 假定nThread为4，triangleCount为12，开了4条线程去进行并行插入三角形
        // 第0条线程插入三角形的编号为：0，4，8
        // 第1条线程插入三角形的编号为：1，5，9
        // 第2条线程插入三角形的编号为：2，6，10
        // 第3条线程插入三角形的编号为：3，7，11
        for (int i = 0; i < nThreads; i++)
        {
            int id = i;
            // 可被 joinable 的 std::thread 对象必须在他们销毁之前被主线程 join 或者将其设置为 detached.
            threads.push_back(std::thread([&, id]()
                {
                    for (int j = id; j < triangleCount; j += nThreads)
                    {
                        insertTriangleParallel(j);
                    }
                })
            );
        }

        // c++11中关于std::thread的join的详解
        // https://blog.csdn.net/FL1768317420/article/details/136290423
        // 在使用std::thread的时候，对创建的线程有两种操作：等待 / 分离，也就是join / detach操作。
        // join()操作是在std::thread t(func)后“某个”合适的地方调用，其作用是回收对应创建的线程的资源，
        // 避免造成资源的泄露。detach()操作是在std::thread t(func)后马上调用，用于把被创建的线程与做
        // 创建动作的线程分离，分离的线程变为后台线程, 其后，创建的线程的“死活”就与其做创建动作的线程无
        // 关，它的资源会被init进程回收。
        // 在C++中，如果你尝试对已经执行完毕的线程使用join()，将不会发生任何问题。join()成员函数的作用
        // 是等待线程完成其执行，但如果线程已经结束，调用join()将立即返回。
        // 所以主线程会在这些插入三角形的线程都执行完之后，才返回，否则就block住
        for (auto& t : threads)
        {
            t.join();
        }

        return true;
    }

    void Octree::octreeReinsertTrianglesParallel()
    {
        // TriangleOctantKeyPairList::loadTriangleOctantKeyPairList();
        loadTriangleOctantList();

        for (auto& octant : octants)
        {
            octant.triangleIDs->clear();
        }

        insertTrianglesParallel();
    }

    bool Octree::updateTriangleInOctreeParallel(int tri)
    {
        if (removeTriangleFromOctreeParallel(tri))
        {
            insertTriangleParallel(tri);
            return true;
        }
        return false;
    }

    bool Octree::updateTrianglesInOctreeParallel(std::vector<int> tris)
    {
        int triangles_count = (int)tris.size();

        if (triangles_count == 0)
        {
            return true;
        }

        int nThreads = getNThreads();
        std::vector<std::thread> threads;
        std::vector<bool> updateSuccessfulList;
        for (int i = 0; i < nThreads; i++)
        {
            updateSuccessfulList.push_back(true);
            int id = i;
            threads.push_back(std::thread([&, id]() {
                for (int j = id; j < triangles_count; j += nThreads)
                {
                    updateSuccessfulList[id] = updateSuccessfulList[id] && updateTriangleInOctreeParallel(tris[j]);
                }
                }));
        }

        for (auto& t : threads)
        {
            t.join();
        }

        return all_of(updateSuccessfulList.begin(), updateSuccessfulList.end(), [](bool b) { return b; });
    }

    bool Octree::removeTriangleFromOctreeParallel(int tri)
    {
        // Get octant that triangle is in
        int oix = triangleToOctantList[tri];

        // If triangle is not in an octant, return false
        if (oix == NonExistentOctantIndex || oix >= static_cast<int>(octants.size()))
        {
            return false;
        }

        std::lock_guard<std::mutex> lock(*octantMutexes[oix]);

        if (octants[oix].triangleIDs->find(tri) != octants[oix].triangleIDs->end())
        {
            octants[oix].triangleIDs->erase(tri);
            return true;
        }

        return false; // Triangle was not found in the octant
    }

    void Octree::updateAffectedTrianglesParallel()
    {
        updateTrianglesInOctreeParallel(affectedTriangles);
        clearCollision();
    }

    void Octree::clearCollision()
    {
        collision = OctreeCollision();
        verticesInRange.clear();
        trianglesInRange.clear();
        affectedTriangles.clear();

        reflectedCollision = OctreeCollision();
        reflectedVerticesInRange.clear();
        reflectedTrianglesInRange.clear();
    }

    extern concurrency::concurrent_vector<std::unique_ptr<std::mutex>> octantMutexes;

    /**
     * @brief Search octree for octant which encloses triangle. Standard Traversal.
     *
     * @param triangle
     * @return int
     */
    int Octree::findOctantForTriangle(int triangle)
    {
        octreeCurrentDepth = 0;                                                                                                    // Set the current depth to 0
        glm::vec3 triangleCentroid = getTriangleCentroid(triangle);                                                                       // get the triangle centroid, used for morton code
        int currOctant = root;                                                                                             // Track the current octant index, start at root
        int nextOctant = octants[currOctant].children[mortonCodeHash(triangleCentroid, octants[currOctant].octantCenter)]; // Calculate and track the next octant

        while (nextOctant != NoOctantChildSet && isTriangleInOctantBounds(triangle, nextOctant)) // While the next octant exists and the triangle is within its bounds
        {
            currOctant = nextOctant;                                                                                       // Set the current octant to the next octant
            nextOctant = octants[currOctant].children[mortonCodeHash(triangleCentroid, octants[currOctant].octantCenter)]; // Calculate the next octant
            octreeCurrentDepth++;                                                                                          // increment the current depth
        }
        return currOctant;
    }

    int Octree::mortonCodeHash(const glm::vec3& point, const glm::vec3& center)
    {
        // this function is performing a safety test on point.members that are "0" on an axis.
        // Due to the nature of floating points, -0.0f is possible and less than 0.0f, so this function takes this into account and auto converts -0.0f into 0.0f if it exists so there are no errors in the MortonCode produced.
        return (MortonCodeConvert_Safe(point.x, center.x) << 2) | (MortonCodeConvert_Safe(point.y, center.y) << 1) | (MortonCodeConvert_Safe(point.z, center.z));
    }

    void Octree::subdivideOctant(int oix)
    {
        // 如果“当前操作的树深度值”已经等于“树深度值”，则“树深度值”加1，“当前操作的树深度值”也加1
        if (octreeCurrentDepth == octreeDepth)
        {
            octreeDepth++;
        }

        octreeCurrentDepth++;

        // 給待细分的八叉树节点，创建八个新的子节点
        for (int i = 0; i < 8; i++)
        {
            createChildOctant((OctantPosition)i, oix);
        }
        // Reinsert triangles into children if they fit entirely inside
        // 遍历本节点管理的三角形。取出本节点插入的如果三角形完全适合子代，则将其重新插入
        auto& ts = *(octants[oix].triangleIDs.get());

        for (auto& tri : ts)
        {
            // 拿到编号为tri的那个三角形的质心，用质心和本节点的中心点，算出morton码，
            // 拿到三角形所在的那个子节点的索引
            glm::vec3 triangleCentroid = getTriangleCentroid(tri);
            int childID = octants[oix].children[mortonCodeHash(triangleCentroid, octants[oix].octantCenter)];

            // 检查给定的三角形的三个顶点，是否都在给定的子节点内
            if (isTriangleInOctantBounds(tri, childID))
            {
                int temp = tri;
                octants[childID].triangleIDs->insert(temp); // 把三角形插入到子节点中
                triangleToOctantList[tri] = childID;        // 然后三角形对应的八叉树节点也重新映射一下
                octants[oix].triangleIDs->erase(tri);       // 从待细分节点所持有的三角形列表中移除掉这个三角形
            }
        }

        // 细分完本节点后，递归式逐个细分本节点的子节点
        for (int i = 0; i < 8; i++)
        {
            int childOctant = octants[oix].children[i];

            // 如果“当前操作的树深度值”，小于树深度值”，且当前子节点管理的三角形个数，大于每个八叉树节点所
            // 管理的最大三角形数则可以进行进一步细分
            if (octreeCurrentDepth < octreeDepthLimit && octants[childOctant].triangleIDs->size() > octantTriangleLimit)
            {
                subdivideOctant(childOctant);
            }
        }

        // 设置待细分的节点的状态，如果其管理的三角形个数为0了，设置其为空节点，否则设置为非空节点
        octants[oix].octantState = octants[oix].triangleIDs->size() == 0 ? OctantEmptyInternal : OctantNotEmptyInternal;
        octreeCurrentDepth--; // 因为添加了子节点，意味着增加了一层树节点，那自然就需要把“当前操作的树深度值”减去一层
    }

    /**
     * @brief Creates a child octant for the octant at the given index.
     * Adds the child to the octant list.
     * Adjusts the center and halfsize of the child octant according to the looseness of the octree.
     *
     * NOTE: Can be optimized by calculating the unadjusted and loose halfsizes before hand since
     * those are shared between all children of an octant.
     *
     * @param octantPosition
     * @param parentIndex
     */
    void Octree::createChildOctant(OctantPosition octantPosition, int parentIndex)
    {
        Octant childOctant; // Empty octant

        Octant& parentOctant = octants[parentIndex]; // Get the parent octant reference

        float unadjustedHalfSize = parentOctant.octantHalfSize / 2.0f;     // 給子节点的未调整的半尺寸
        float looseHalfSize = (float)octreeLooseness * unadjustedHalfSize; // 給子节点的松弛的半尺寸
        float halfSizeDifference = looseHalfSize - unadjustedHalfSize;     // 算出两者的差值

        glm::vec3 positionVector = octantPositionVectors[(int)octantPosition]; // Position vector for child octant

        // Unadjusted child center, based on parent center and unadjusted half size
        // 首先计算出未经调整之子节点中心，其值未父节点中心值，乘以方向向量，再乘以未经调整的半长宽高值
        glm::vec3 unadjustedChildCenter = parentOctant.octantCenter + glm::vec3(unadjustedHalfSize) * positionVector;

        // Set child center, adjusted for looseness
        // Subtraction used to reverse direction of position vector
        // Moves the child's center towards the parent's center based on the half size difference
        childOctant.octantCenter = unadjustedChildCenter - glm::vec3(halfSizeDifference) * positionVector;
        childOctant.octantHalfSize = looseHalfSize;    // Set half size of child octant, adjusted for looseness
        childOctant.octantIndex = (int)octants.size(); // Set octants index to the next available index
        childOctant.parent = parentIndex;              // Set parent of child octant

        // Set the parent's child at the octant position to the child's index in the octants vector
        parentOctant.children[(int)octantPosition] = childOctant.octantIndex;

        octants.push_back(childOctant); // Add child octant to octants list
        octantMutexes.push_back(std::make_unique<std::mutex>());
    }

    bool Octree::isTriangleInOctantBounds(int tri, int octantID)
    {
        Octant& oct = octants[octantID]; // retrieve the current processed octant
        IndexedTriangle triangle = triangles[tri]; // 取出编号为tri的三角形
        glm::vec3 halfSizeVector = glm::vec3(octants[octantID].octantHalfSize + std::numeric_limits<float>::epsilon()); // Half size vec3 for comparison

        // 检查三角形的每个顶点，与八叉树元中心点之间的距离，是否小于八叉树元中心点的一半大小
        // glm::all方法判断传递进来的bool向量中，是不是所有的分量都为true
        // glm::lessThanEqual函数判断第1个向量是不是小于第2个向量，每个向量的分量各自对应独立比较，判断出来的bool值存在一个向量中返回
        // glm::abs函数把传入的向量的每个分量都取其正数然后返回 
        // 把待检测的三角形的三个顶点的xyz，分别于八叉树元的中点xyz相减，如果其绝对值都小于八叉树元的半长宽高值，即为待检测三角形
        // 完全在本八叉树元内
        return
            glm::all(glm::lessThanEqual(glm::abs(oct.octantCenter - vertices[triangle[0]].position), halfSizeVector)) &&
            glm::all(glm::lessThanEqual(glm::abs(oct.octantCenter - vertices[triangle[1]].position), halfSizeVector)) &&
            glm::all(glm::lessThanEqual(glm::abs(oct.octantCenter - vertices[triangle[2]].position), halfSizeVector));
    }

    // Parallel Functions
    std::mutex mtxOctants;
    std::mutex mtxDepth;

    void Octree::subdivideOctantParallel(int oix, int localDepth)
    {
        mtxDepth.lock();
        if (localDepth == octreeDepth) // Update the current depth value of the octree if subdivision increases it
        {
            octreeDepth++;
        }
        mtxDepth.unlock();

        localDepth++;
        std::vector<int> children;
        children.reserve(8);
        for (int i = 0; i < 8; i++) // Create this octants children
        {
            children.push_back(createChildOctantParallel((OctantPosition)i, oix));
        }

        // // Reinsert triangles into children if they fit entirely inside
        // // Reverse order to preserve positions on removal
        // for (int i = (int)octants[oix].triangleIDs->size() - 1; i >= 0; i--)
        // {
        //     int tri = octants[oix].triangleIDs->at(i);
        //     glm::vec3 triangleCentroid = getTriangleCentroid(tri);
        //     int childID = children[mortonCodeHash(triangleCentroid, octants[oix].octantCenter)];

        //     // If triangle fits in child octant, insert it
        //     if (isTriangleInOctantBounds(tri, childID))
        //     {
        //         octants[childID].triangleIDs->emplace_back(tri);                       // Insert triangle into child octant
        //         triangleToOctantList[tri] = childID;                                  // Correct the triangle - octant mapping
        //         octants[oix].triangleIDs->erase(octants[oix].triangleIDs->begin() + i); // Remove triangle from the parent octant
        //     }
        // }

        // Reinsert triangles into children if they fit entirely inside
        auto ts = *(octants[oix].triangleIDs.get());
        for (auto& tri : ts)
        {
            glm::vec3 triangleCentroid = getTriangleCentroid(tri);
            int childID = children[mortonCodeHash(triangleCentroid, octants[oix].octantCenter)];

            // If triangle fits in child octant, insert it
            if (isTriangleInOctantBounds(tri, childID))
            {
                int temp = tri;
                octants[childID].triangleIDs->insert(temp); // Insert triangle into child octant
                triangleToOctantList[tri] = childID;        // Correct the triangle - octant mapping
                octants[oix].triangleIDs->erase(tri);       // Remove triangle from the parent octant
            }
        }

        for (int i = 0; i < 8; i++) // Subdivide any child octants if necessary
        {
            int childID = children[i]; // Avoiding nested [] operators for clarity

            if (localDepth < octreeDepthLimit && octants[childID].triangleIDs->size() > octantTriangleLimit)
            {
                subdivideOctantParallel(childID, localDepth);
            }
            // octantMutexes[childOctant]->unlock();
        }

        // Set state of subdivided octant
        // Checks if octant contains any triangles ands sets state accordingly
        if (octants[oix].triangleIDs->size() == 0)
        {
            octants[oix].octantState = OctantEmptyInternal;
        }
        else
        {
            octants[oix].octantState = OctantNotEmptyInternal;
        }

        octants[oix].children = children;
    }

    // Generating colors
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<> dist(50, 100);

    // -- WARNING: CHILD OCTANTS ARE LOCKED WHEN CREATED -- Not currently
    int Octree::createChildOctantParallel(OctantPosition octantPosition, int parentIndex)
    {
        Octant childOctant;                         // Empty octant
        Octant parentOctant = octants[parentIndex]; // Get the parent octant reference

        childOctant.color = glm::vec4((float)dist(e2) / 100.0f,
            (float)dist(e2) / 100.0f,
            (float)dist(e2) / 100.0f,
            1.0f); // Generate a random color for the child octant

        float unadjustedHalfSize = parentOctant.octantHalfSize / 2.0f;     // Unadjusted half size for child octant
        float looseHalfSize = (float)octreeLooseness * unadjustedHalfSize; // Looseness adjusted half size for child octant
        float halfSizeDifference = looseHalfSize - unadjustedHalfSize;     // Difference between the two half sizes

        glm::vec3 positionVector = octantPositionVectors[(int)octantPosition]; // Position vector for child octant

        // Unadjusted child center, based on parent center and unadjusted half size
        glm::vec3 unadjustedChildCenter = parentOctant.octantCenter + glm::vec3(unadjustedHalfSize) * positionVector;

        // Set child center, adjusted for looseness
        // Subtraction used to reverse direction of position vector
        // Moves the child's center towards the parent's center based on the half size difference
        childOctant.octantCenter = unadjustedChildCenter - glm::vec3(halfSizeDifference) * positionVector;
        childOctant.octantHalfSize = looseHalfSize; // Set half size of child octant, adjusted for looseness
        childOctant.parent = parentIndex;           // Set parent of child octant

        // Insert child into octants vector and lock it before adding to the parent
        int childIndex = insertOctantParallel(childOctant);
        // octantMutexes[childIndex]->lock();

        // set the parent's child at the octant position to the child's index in the octants vector
        // octants[parentIndex].children[(int)octantPosition] = childIndex;
        return childIndex;
    }

    int Octree::insertOctantParallel(Octant& octant)
    {
        std::lock_guard<std::mutex> lock(mtxOctants);
        int octantIndex = (int)octants.size();
        octant.octantIndex = octantIndex;
        octants.push_back(octant);
        octantMutexes.push_back(std::make_unique<std::mutex>());
        return octantIndex;
    }

    std::pair<int, int> Octree::findOctantForTriangleParallel(int triangle, int start)
    {
        int localDepth = 0;
        glm::vec3 triangleCentroid = getTriangleCentroid(triangle);
        int currOctant = start;
        int nextOctant = octants[currOctant].children[mortonCodeHash(triangleCentroid, octants[currOctant].octantCenter)];

        while (nextOctant != NoOctantChildSet && isTriangleInOctantBounds(triangle, nextOctant))
        {
            currOctant = nextOctant;
            nextOctant = octants[currOctant].children[mortonCodeHash(triangleCentroid, octants[currOctant].octantCenter)];
            localDepth++;
        }

        return std::make_pair(currOctant, localDepth);
    }
}