#include "Octree.hpp"
#include "VertexIDHashing.hpp"

using namespace OctreeDefinition;
concurrency::concurrent_vector<unique_ptr<mutex>> octantMutexes;
/**
 * @brief Print octree information for debugging.
 */
void OctreeDefinition::Octree::octreePrintStats() ONOEXCEPT
{
    say "Octree Debug" done;
    say "\tVertex Limit:" spc octantTriangleLimit done;
    say "\tDepth Limit:" spc octreeDepthLimit done;
    say "\tOctree Buffer Size:" spc octreeBuffer done;
    say "\tOctree Looseness:" spc octreeLooseness done;
    say "\tDepth:" spc octreeDepth done;
    say "\tCenter:" spc to_string(octants[root].octantCenter) done;
    say "\tHalfsize:" spc octants[root].octantHalfSize done;
    say "\tVertices: " << verticeCount() done;
    say "\tTriangles:" spc totalTriangles() done;
    say "\tOctants:" spc octants.size() done;
    say "\tTriangles in Root:" spc octants[root].triangleIDs->size() done;
    // say "\tLeaves:" spc leaves.size() done;

    int count = 0; // Counts leaves and pseudo-leaves
    int owt = 0;   // Octants with triangles
    int pwt = 0;   // Triangles in pseudo leaves
    foreach(o, octants)
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
    say "\tActive Octants:" spc count done;
    say "\tOctants w/ Triangles:" spc owt done;
    say "\tPsuedo-Leaves w/ Traingles:" spc pwt done;
}

/**
 * @brief Tests various octree functions.
 * Same ideas can be used for unit testing.
 *
 */
void OctreeDefinition::Octree::testOctree() ONOEXCEPT
{

    this->octreePrintStats();

    // Testing Intersection and Collection
    v3 rayOrigin(0, 0, 3);
    v3 rayDirection(0, 0, -1);
    // OctreeCollision collision = this->octreeRayIntersection(rayOrigin, rayDirection);
    if (collision.isCollision)
    {
        say "Collision: " done;
        say "\tRay Origin: " << to_string(rayOrigin) done;
        say "\tRay Direction: " << to_string(rayDirection) done;
        say "\tIntersected Triangle: "
            << collision.triangleID << "\n\t\t< "
            << glm::to_string(vertices[triangles[collision.triangleID][0]].position) << ", "
            << glm::to_string(vertices[triangles[collision.triangleID][1]].position) << ", "
            << glm::to_string(vertices[triangles[collision.triangleID][2]].position) << " >" done;
        say "\tIntersection Point: " << glm::to_string(collision.position) done;
        // double collectRange = glm::sqrt(2) / 8;
        // KeyList verticesCollected = collectVerticesAroundCollision(collision, collectRange);
        // say "\tVertices Around Collision (Range " << collectRange << "): " << verticesCollected.size();
        // for (int i = 0; i < verticesCollected.size(); i++)
        // {
        //     say(i % 20 == 0 ? "\n\t\t" : "") << verticesCollected[i] << " ";
        // }
        // say endl;
    }

    // Testing removal
    int count = 0;
    foreach(octant, octants)
    {
        count += (int)octant.triangleIDs->size();
    }
    say "Triangles in Octree: " << count done;
    say "Triangles in Mesh (should be same ^): " << totalTriangles() done;

    for (int i = 0; i < totalTriangles(); i++)
    {
        removeTriangleFromOctree(i);
    }

    count = 0;
    foreach(octant, octants)
    {
        count += (int)octant.triangleIDs->size();
    }
    say "Triangles in Octree after Removal (should be 0): " << count done;

    say "---Clearing Octree---" done;
    clearOctree();
    say "\t# of Octants (should be 0): " << octants.size() done;
    say "---Rebuilding Octree---" done;
    say "(Should be same as first build)" done;
    rebuildOctree();
    octreePrintStats();
}

/**
 * @brief Build the octree by inserting all triangles of the mesh into the octree.
 * Initializes the root octant.
 */
void OctreeDefinition::Octree::buildOctree() ONOEXCEPT
{
    // TriangleOctantKeyPairList::loadTriangleOctantKeyPairList();
    loadTriangleOctantList();

    // Adjust the extents of the mesh vertices wrt the center
    float maxExtent = compMax(abs(this->center - limMax));
    float minExtent = compMin(abs(this->center - limMin));

    // Create root octant with appropriate center and half size
    Octant rootOctant;
    rootOctant.octantIndex = 0;
    rootOctant.octantCenter = this->center;
    rootOctant.octantHalfSize = glm::max(maxExtent, minExtent) * octreeBuffer;
    octants.push_back(rootOctant);
    octantMutexes.push_back(make_unique<mutex>());

    // Insert all triangles into the octree
    insertTrianglesParallel();
}

/**
 * @brief Clear the octree and reset appropriate values.
 *
 */
void OctreeDefinition::Octree::clearOctree() ONOEXCEPT
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
void OctreeDefinition::Octree::rebuildOctree() ONOEXCEPT
{
    clearOctree();
    collectStats();
    buildOctree();
}

void Octree::loadTriangleOctantList() ONOEXCEPT {
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
void OctreeDefinition::Octree::resizeOctree(TriangleID tri) ONOEXCEPT
{
    say "resizing octree" done;
    v3 triangleCentroid = getTriangleCentroid(tri); // Get the triangle's centroid

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
        v3 newRootPositionVector = octantPositionVectors[newRootDirection]; // Vector from old root to new root

        // Adjusted halfsize removing looseness
        // New Root's halfsize, already loose no need for adjustment
        float oldRootAdjustedHalfSize = (float)(oldRoot.octantHalfSize / octreeLooseness);
        float newRootHalfsize = oldRootAdjustedHalfSize * 2;

        // Adjusted old root center, based on old root's center and adjusted half size
        // New Root's Center based on old root's center and adjusted half size
        v3 oldRootAdjustedCenter = oldRoot.octantCenter - newRootPositionVector * (oldRoot.octantHalfSize - oldRootAdjustedHalfSize);
        v3 newRootCenter = oldRootAdjustedCenter + newRootPositionVector * oldRootAdjustedHalfSize;

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
        OctantIndex oldRootNewIndex = octants[root].children[oldRootNewPosition];
        oldRoot.octantIndex = oldRootNewIndex;
        octants[oldRootNewIndex] = oldRoot;

        foreach(triangleID, *(oldRoot.triangleIDs))
        {
            triangleToOctantList[triangleID] = oldRootNewIndex;
        }

        // Adjust the parent values of the old roots children to point to its new index
        if (oldRoot.octantState != OctantLeaf)
        {
            foreach(child, oldRoot.children)
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
void OctreeDefinition::Octree::resizeOctreeParallel(TriangleID tri) ONOEXCEPT
{
    if (tri >= triangleToOctantList.size())
    {
        say "Triangle ID out of bounds" done;
        return;
    }

    // Only one thread can resize at a time
    lock_guard<mutex> lock(mtxResize);

    v3 triangleCentroid = getTriangleCentroid(tri); // Get the triangle's centroid

    // Loop until triangle fits
    while (!isTriangleInOctantBounds(tri, root))
    {
        if (octreeDepth == octreeDepthLimit)
        {
            octreeDepth++;
            octreeDepthLimit++;
        }
        OctantReference oldRoot = octants[root]; // Save oldRoot to replace one of the new root's children

        // Get morton code of triangle centroid to determine direction to grow octree
        OctantPosition newRootDirection = (OctantPosition)mortonCodeHash(triangleCentroid, oldRoot.octantCenter);
        v3 newRootPositionVector = octantPositionVectors[newRootDirection]; // Vector from old root to new root

        // Adjusted halfsize removing looseness
        // New Root's halfsize, already loose no need for adjustment
        float oldRootAdjustedHalfSize = (float)(oldRoot.octantHalfSize / octreeLooseness);
        float newRootHalfsize = oldRootAdjustedHalfSize * 2;

        // Adjusted old root center, based on old root's center and adjusted half size
        // New Root's Center based on old root's center and adjusted half size
        // Obtain morton code of old root in relation to new root
        v3 oldRootAdjustedCenter = oldRoot.octantCenter - newRootPositionVector * (oldRoot.octantHalfSize - oldRootAdjustedHalfSize);
        v3 newRootCenter = oldRootAdjustedCenter + newRootPositionVector * oldRootAdjustedHalfSize;
        OctantPosition oldRootNewPosition = (OctantPosition)mortonCodeHash(oldRootAdjustedCenter, newRootCenter);

        Octant newRoot;                                          // Initialize new root
        newRoot.octantState = OctantEmptyInternal;               // Assign new root state
        newRoot.octantHalfSize = newRootHalfsize;                // Assign new root halfsize
        newRoot.octantCenter = newRootCenter;                    // Assign new root center
        newRoot.children[oldRootNewPosition] = root;             // Place old root as child
        OctantIndex octantIndex = insertOctantParallel(newRoot); // insert new root into octree
        octants[root].parent = octantIndex;                      // Set old root's parent to new root

        // Create new root's children
        for (int i = 0; i < 8; i++)
        {
            if (i == oldRootNewPosition) // Skip old root's position
            {
                continue;
            }
            octants[octantIndex].children[i] = createChildOctantParallel((OctantPosition)i, octantIndex);
            // OctantIndex childIndex = createChildOctantParallel((OctantPosition)i, octantIndex);
            // octantMutexes[childIndex]->unlock();
        }
        root = octantIndex; // Set root to new root
    }
}