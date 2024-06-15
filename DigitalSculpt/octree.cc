#include "spdlog/spdlog.h"
#include "glm/gtx/component_wise.hpp"

#include "octree.h"
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
        spdlog::info("\tCenter: {} \n", to_string(octants[root].octantCenter));;
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
            spdlog::info("\tRay Origin: {} \n", to_string(rayOrigin));
            spdlog::info("\tRay Direction: {} \n ", to_string(rayDirection));
            spdlog::info("\tIntersected Triangle: {} \n\t\t< {}, {}, {}>\n", , collision.triangleID,
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
        // TriangleOctantKeyPairList::loadTriangleOctantKeyPairList();
        loadTriangleOctantList();

        // Adjust the extents of the mesh vertices wrt the center
        float maxExtent = glm::compMax(abs(this->center - limMax));
        float minExtent = glm::compMin(abs(this->center - limMin));

        // Create root octant with appropriate center and half size
        Octant rootOctant;
        rootOctant.octantIndex = 0;
        rootOctant.octantCenter = this->center;
        rootOctant.octantHalfSize = glm::max(maxExtent, minExtent) * octreeBuffer;
        octants.push_back(rootOctant);
        octantMutexes.push_back(std::make_unique<std::mutex>());

        // Insert all triangles into the octree
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
        // triangleToOctantList.clear();
        const int triCount = totalTriangles();
        triangleToOctantList.reserve(triCount);
        for (int i = 0; i < triCount; i++) {
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
        glm::vec3 triangleCentroid = getTriangleCentroid(tri); // Get the triangle's centroid

        // Continuously resize until triangle fits into the root octant
        while (!isTriangleInOctantBounds(tri, root))
        {
            // If the depth limit is currently reached, increase limit to allow resizing
            if (octreeDepth == octreeDepthLimit)
            {
                octreeDepth++;
                octreeDepthLimit++;
            }

            Octant oldRoot = octants[root]; // Save oldRoot to replace one of the new root's children

            // Get morton code of triangle centroid to determine direction to grow octree
            OctantPosition newRootDirection = (OctantPosition)mortonCodeHash(triangleCentroid, oldRoot.octantCenter);
            glm::vec3 newRootPositionVector = octantPositionVectors[newRootDirection]; // Vector from old root to new root

            // Adjusted halfsize removing looseness
            // New Root's halfsize, already loose no need for adjustment
            float oldRootAdjustedHalfSize = (float)(oldRoot.octantHalfSize / octreeLooseness);
            float newRootHalfsize = oldRootAdjustedHalfSize * 2;

            // Adjusted old root center, based on old root's center and adjusted half size
            // New Root's Center based on old root's center and adjusted half size
            glm::vec3 oldRootAdjustedCenter = oldRoot.octantCenter - newRootPositionVector * (oldRoot.octantHalfSize - oldRootAdjustedHalfSize);
            glm::vec3 newRootCenter = oldRootAdjustedCenter + newRootPositionVector * oldRootAdjustedHalfSize;

            // Create new root octant, set its values accordingly
            Octant newRoot;
            newRoot.octantState = OctantEmptyInternal;
            newRoot.octantHalfSize = newRootHalfsize;
            newRoot.octantCenter = newRootCenter;
            newRoot.octantIndex = root;
            octants[root] = newRoot;

            // Subdivide the new root to add missing child octants
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
                for (child, oldRoot.children)
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
    mutex mtxResize;
    void Octree::resizeOctreeParallel(int tri)
    {
        if (tri >= triangleToOctantList.size())
        {
            spdlog::info("Triangle ID out of bounds \n");
            return;
        }

        // Only one thread can resize at a time
        lock_guard<mutex> lock(mtxResize);

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

    extern concurrency::concurrent_vector<unique_ptr<mutex>> octantMutexes;
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
        if (tri >= triangleToOctantList.size())
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

    // Parallel Functions
    /**
     * @brief Insert triangle into octree, using the triangleID. Corrects state and subdivides if necessary.
     *
     * @param tri
     * @return true
     * @return false
     */
    bool Octree::insertTriangleParallel(int tri) 
    {
        if (tri > triangleToOctantList.size())
        {
            return false;
        }
        // Check if the triangle is in the octree
        if (!Octree::isTriangleInOctantBounds(tri, root))
        {
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
                octants[oix].triangleIDs->size() > octantTriangleLimit)
            {
                subdivideOctantParallel(oix, localDepth);
            }

            octantMutexes[oix]->unlock();
            return true;
        }
    }

    bool Octree::insertTrianglesParallel() 
    {
        int nThreads = std::thread::hardware_concurrency();

        if (nThreads == 0)
        {
            nThreads = 1;
        }

        int triangleCount = totalTriangles();
        vector<thread> threads;
        for (int i = 0; i < nThreads; i++)
        {
            int id = i;
            // I'm doin lambda functions, what're you gonna do about it?
            threads.push_back(thread([&, id]() {
                for (int j = id; j < triangleCount; j += nThreads)
                {
                    insertTriangleParallel(j);
                }
                }));
        }

        foreach(t, threads)
        {
            t.join();
        }
        return true;
    }

    void Octree::octreeReinsertTrianglesParallel() 
    {
        // TriangleOctantKeyPairList::loadTriangleOctantKeyPairList();
        loadTriangleOctantList();
        foreach(octant, octants)
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
            threads.push_back(thread([&, id]() {
                for (int j = id; j < triangles_count; j += nThreads)
                {
                    updateSuccessfulList[id] = updateSuccessfulList[id] && updateTriangleInOctreeParallel(tris[j]);
                }
                }));
        }
        joinThreads;

        return all_of(updateSuccessfulList.begin(), updateSuccessfulList.end(), [](bool b) { return b; });
    }

    bool Octree::removeTriangleFromOctreeParallel(int tri) 
    {
        // Get octant that triangle is in
        int oix = triangleToOctantList[tri];

        // If triangle is not in an octant, return false
        if (oix == NonExistentOctantIndex || oix >= octants.size())
        {
            return false;
        }

        lock_guard<mutex> lock(*octantMutexes[oix]);

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
}