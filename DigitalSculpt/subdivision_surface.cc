#include <unordered_map>
#include <mutex>
#include <limits>
#include "indexed_triangle.h"
#include "subdivision_surface.h"

/**
 * @brief Performs simple 4:1 subdivision of the mesh.
 *  - Creates new vertices at the midpoints of each edge.
 *  - Creates 3 new triangles, using one old vertex and two midpoints.
 *  - Replaces the original triangles with triangles created from the midpoints.
 *  - Removes the edges and connects the old vertices to the midpoints.
 *  - Assigns the new Triangles and Edges to every vertex.
 *
 *   Creating new triangles
 *
 *        v0
 *        /\
 *   mp0 /__\ mp2
 *      /\  /\
 *     /__\/__\
 *   v1   mp1  v2
 *
 *  v_ = original vertex
 *  mp_ = midpoint
 *
 *   New Triangles are:
 *       <v0,  mp0, mp2>
 *       <v1,  mp1, mp0>
 *       <v2,  mp2, mp1>
 *       <mp0, mp1, mp2> // This replaces the original triangle
 */

namespace DigitalSculpt
{
    const float InfiniteCreaseAngle = std::numeric_limits<float>::max();
        // #define PI 3.141592f
    const     float TWO_PI = 6.283184f;

    void SubdivisionSurface::simpleSubdivision4to1(int level, bool octreeRebuild, bool refreshDisplay, bool isLoop) 
    {
        // Loops for number of levels to subdivide
        for (int levelCounter = 0; levelCounter < level; levelCounter++)
        {
            vertexOffset = (int)vertices.size(); // Offset to where new vertices are placed in the vertices vector.
            int vertexIndex = vertexOffset;      // Index for new vertices.
            const int trianglesSize = (int)triangles.size();

            std::unordered_map<glm::vec3, GLuint> midpointMap; // Map of midpoints to their index in the vertices vector.
            std::vector<std::vector<GLuint>> triangleMidpointMap;

            if (isLoop)
            {
                vertexABCDs = std::vector<std::vector<GLuint>>(vertices.size() * 4);
            }

            // Fill midpoint map with all midpoints
            // Parallelizable
            for (GLuint vertexID = 0; vertexID < (GLuint)vertexOffset; vertexID++)
            {
                for(auto otherVertexID : edges[vertexID].vertexEdges)
                {
                    if (vertexID < otherVertexID)
                    {
                        glm::vec3 midpoint = (vertices[vertexID].position + vertices[otherVertexID].position) * 0.5f;
                        V3D newVert = midpoint;
                        newVert.color = (vertices[vertexID].color + vertices[otherVertexID].color) * 0.5f;
                        vertices.emplace_back(newVert);
                        midpointMap.emplace(midpoint, vertexIndex);

                        if (isLoop)
                        {
                            vertexABCDs[vertexIndex].emplace_back(vertexID);
                            vertexABCDs[vertexIndex].emplace_back(otherVertexID);
                        }

                        vertexIndex++;
                    }
                }
            }

            // Clear edges, triangles, ABCD of each vertex
            for (int vertexID = 0; vertexID < vertexOffset; vertexID++)
            {
                edges[vertexID].vertexEdges.clear();
                vertices[vertexID].triangleIDs.clear();
            }

            for (int tri = 0; tri < trianglesSize; tri++)
            {
                std::vector<GLuint> midpointIDs = {
                    midpointMap[getEdgeMidpoint(triangles[tri][0], triangles[tri][1])],
                    midpointMap[getEdgeMidpoint(triangles[tri][1], triangles[tri][2])],
                    midpointMap[getEdgeMidpoint(triangles[tri][2], triangles[tri][0])] };
                triangleMidpointMap.push_back(midpointIDs);

                if (isLoop)
                {
                    vertexABCDs[midpointIDs[0]].emplace_back(triangles[tri][2]);
                    vertexABCDs[midpointIDs[1]].emplace_back(triangles[tri][0]);
                    vertexABCDs[midpointIDs[2]].emplace_back(triangles[tri][1]);
                }
            }

            int triangleIndex = 0;
            std::vector<IndexedTriangle> newTriangles;
            for (int tri = 0; tri < trianglesSize; tri++)
            {
                std::vector<GLuint> midpoints = triangleMidpointMap[tri];

                for (int i = 0; i < 3; i++)
                {
                    IndexedTriangle newTriangle;
                    newTriangle[0] = triangles[tri][i];
                    newTriangle[1] = midpoints[i];
                    newTriangle[2] = midpoints[(i + 2) % 3];
                    vertices[newTriangle[0]].triangleIDs.insert(triangleIndex);
                    vertices[newTriangle[1]].triangleIDs.insert(triangleIndex);
                    vertices[newTriangle[2]].triangleIDs.insert(triangleIndex);
                    newTriangles.emplace_back(newTriangle);
                    triangleIndex++;
                }

                IndexedTriangle midpointTriangle;
                midpointTriangle[0] = midpoints[0];
                midpointTriangle[1] = midpoints[1];
                midpointTriangle[2] = midpoints[2];
                vertices[midpointTriangle[0]].triangleIDs.insert(triangleIndex);
                vertices[midpointTriangle[1]].triangleIDs.insert(triangleIndex);
                vertices[midpointTriangle[2]].triangleIDs.insert(triangleIndex);
                newTriangles.emplace_back(midpointTriangle);
                triangleIndex++;
            }

            triangles = newTriangles;
            generateEdges();
        }

        if (octreeRebuild)
        {
            octreeReinsertTrianglesParallel();
        }
        if (refreshDisplay)
        {
            refresh();
        }
    }

    inline glm::vec3 SubdivisionSurface::getEdgeMidpoint(GLuint v1, GLuint v2) 
    {
        return (vertices[v1].position + vertices[v2].position) * 0.5f;
    }

    // TODO: Need to adapt to include calculation for boundary vertices
    // Currently Assumes that all vertices are interior
    // Can Optimize with precomputed constants
    std::mutex loopmtx;
    void SubdivisionSurface::loopSubdivision(int level, bool rebuildRefresh) 
    {

        for (int levelCounter = 0; levelCounter < level; levelCounter++)
        {
            simpleSubdivision4to1(1, false, false, true); // 4:1 subdivision. First step of loop subdivision

            std::vector<V3D> newVertexList = vertices; // New vertex list

            // Calculate new position for every original vertex before subdivision
            for (int vertexID = 0; vertexID < vertexOffset; vertexID++)
            {
                int k = (int)edges[vertexID].vertexEdges.size();     // Number of neighboring vertices
                float beta = getBeta(k);                             // Get Beta
                glm::vec3 ksum = sumNeighbors(edges[vertexID].vertexEdges); // Sum of neighboring vertices

                newVertexList[vertexID] = (vertices[vertexID].position * (1.0f - (k * beta))) + (ksum * beta);
            }

            // Calculate new position for every vertex that was created during subdivision
            int verticesSize = (int)vertices.size();
            for (int vertexID = vertexOffset; vertexID < verticesSize; vertexID++)
            {
                vector<GLuint>& ABCDmap = vertexABCDs[vertexID]; // Map of ABCD vector

                newVertexList[vertexID] = (0.375f * (vertices[ABCDmap[0]].position + vertices[ABCDmap[1]].position));

                if (ABCDmap.size() == 4)
                {
                    newVertexList[vertexID] += (0.125f * (vertices[ABCDmap[2]].position + vertices[ABCDmap[3]].position));
                }
            }

            // Calculate new position for every original vertex before subdivision
            // for (int vertexID = 0; vertexID < vertexOffset; vertexID++)
            // {
            //     int k = (int)edges[vertexID].vertexEdges.size();     // Number of neighboring vertices
            //     float beta = getBeta(k);                             // Get Beta
            //     glm::vec3 ksum = sumNeighbors(edges[vertexID].vertexEdges); // Sum of neighboring vertices
            //     newVertexList[vertexID] = (vertices[vertexID].position * (1.0f - (k * beta))) + (ksum * beta);
            //     Boundary/crease Condition
            //     if (isVertexOnCrease(vertexID))
            //     if(false)
            //     {
            //         vector<int> ABCD = vertices[vertexID].ABCD;
            //         newVertexList[vertexID] = (1.0f / 8.0f) * (vertices[ABCD[0]].position + vertices[ABCD[1]].position) +
            //                                   (3.0f / 4.0f) * vertices[vertexID].position;
            //     }
            //     else
            //     {
            //     newVertexList[vertexID] = (vertices[vertexID].position * (1.0f - (k * beta)) + (ksum * beta));
            //     }
            // }
            // Calculate new position for every vertex that was created during subdivision
            // for (GLuint vertexID = vertexOffset; vertexID < vertices.size(); vertexID++)
            // {
            //     vector<int> ABCD = vertices[vertexID].ABCD; // Get the ABCD vector for the vertex
            //     newVertexList[vertexID] = (0.375f * (vertices[ABCD[0]].position + vertices[ABCD[1]].position)) +
            //                               (0.125f * (vertices[ABCD[2]].position + vertices[ABCD[3]].position));
            // Boundary/crease Condition
            // if (isVertexOnCrease(vertexID))
            // if(false)
            // {
            //     newVertexList[vertexID] = 0.5f * (vertices[ABCD[0]].position + vertices[ABCD[1]].position);
            // }
            // // Interior Condition
            // else
            // {
            // newVertexList[vertexID] = (3.0f / 8.0f) * (vertices[ABCD[0]].position + vertices[ABCD[1]].position) +
            //                           (1.0f / 8.0f) * (vertices[ABCD[2]].position + vertices[ABCD[3]].position);
            // }
            // }

            vertices = newVertexList;
        }

        if (rebuildRefresh)
        {
            octreeReinsertTrianglesParallel();
            refresh();
        }

        vertexABCDs.clear();
    }

    // Ignore for now

    // bool SubdivisionSurface::isVertexOnCrease(GLuint vertexID)
    // {
    //     V3D vertex = vertices[vertexID];
    //     // Differentiate between new and old vertices
    //     if ((int)vertexID > vertexOffset) // New Vertex
    //     {
    //         return getCreaseAngleOfNewVertex(vertexID) <= angleLimit; // Return true if angle is less than the limit
    //     }
    //     else // Old Vertex
    //     {
    //         int count = 0;
    //         unordered_map<int, float> creaseAngles;
    //         creaseAngles[0] = InfiniteCreaseAngle;
    //         creaseAngles[1] = InfiniteCreaseAngle;
    //         vertices[vertexID].ABCD.push_back(0); // Add a dummy value to the ABCD vector
    //         vertices[vertexID].ABCD.push_back(0); // Add a dummy value to the ABCD vector
    //         // Check the surrounding vertices, which are all new vertices
    //         for (otherVertexID, edges[vertexID].vertexEdges)
    //         {
    //             float neighborCreaseAngle = getCreaseAngleOfNewVertex(otherVertexID);
    //             // Record neighboring vertices are on a crease
    //             if (neighborCreaseAngle <= angleLimit)
    //             {
    //                 vertices[vertexID].ABCD.push_back(otherVertexID);
    //                 // Replace values if the angle is less than the previous one
    //                 // if (neighborCreaseAngle < creaseAngles[0])
    //                 // {
    //                 //     creaseAngles[0] = neighborCreaseAngle;
    //                 //     vertices[vertexID].ABCD[0] = otherVertexID;
    //                 // }
    //                 // else if (neighborCreaseAngle < creaseAngles[1])
    //                 // {
    //                 //     creaseAngles[1] = neighborCreaseAngle;
    //                 //     vertices[vertexID].ABCD[1] = otherVertexID;
    //                 // }
    //                 count++;
    //             }
    //         }
    //         if (count < 2)
    //         {
    //             vertices[vertexID].ABCD.clear();
    //             return false;
    //         }
    //         return true; // Return true if there are at least 2 neighboring vertices on a crease
    //     }
    // }

    // float SubdivisionSurface::getCreaseAngleOfNewVertex(GLuint vertexID)
    // {
    //     V3D vertex = vertices[vertexID];
    //     if (vertex.ABCD.size() == 4)
    //     {
    //         return abs(angle(vertices[vertex.ABCD[2]].position - vertex.position,
    //                          vertices[vertex.ABCD[3]].position - vertex.position));
    //     }
    //     else
    //     {
    //         return 0.0f;
    //     }
    // }

    float SubdivisionSurface::getBeta(int nEdges) 
    {
        float n = (float)nEdges;
        // nEdges = 1;
        if (nEdges == 3)
        {
            return 0.375f; // 3.0f / 8.0f; should be 3/16
            // return 3.0f/16.0f;
        }
        else if (nEdges > 3)
        {
            return 0.75f * (1.0f / n); // 3.0f / (4.0f * n); should be 3/(8n)
            // return 3.0f/(8.0f*n);
        }
        else
        {
            // Original loop equation for beta with modifier
            // Shouldn't be unused, only here as backup for borderline cases for now
            // float temp = (3.0f / 8.0f) + (1.0f / 4.0f) * cos(TWO_PI / nEdges);
            // return (1.0f / nEdges) * ((5.0f / 8.0f) - temp * temp);
            float temp = 0.375f + (0.25f * cos(TWO_PI / nEdges));
            return (1.0f / n) * (0.625f - (temp * temp));
        }
    }

    glm::vec3 SubdivisionSurface::sumNeighbors(std::unordered_set<GLuint> neighbors) 
    {
        glm::vec3 sum = glm::vec3(0);
        for (GLuint neighbor : neighbors)
        {
            sum += vertices[neighbor].position;
        }
        return sum;
    }

    void SubdivisionSurface::subdivisionTest() 
    {
        loopSubdivision(2);
        octreePrintStats();
    }
}
