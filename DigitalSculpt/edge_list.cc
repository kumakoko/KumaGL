#include <thread>
#include <unordered_set>
#include "spdlog/spdlog.h"
#include "glm/common.hpp"
#include "edge_list.h"

namespace DigitalSculpt
{
    EdgeList::EdgeList()
    {
    }

    EdgeList::~EdgeList()
    {
    }

    Edge& EdgeList::getEdge(GLuint key)
    {
        return this->edges[key];
    }

    const Edge& EdgeList::getEdge(GLuint key) const
    {
        return this->edges[key];
    }

    void EdgeList::generateEdges()
    {
        edges.clear();
        const std::size_t countUniqueVertices = vertices.size();
        edges.reserve(countUniqueVertices);
        for (int i = 0; i < countUniqueVertices; i++)
        {
            this->edges.emplace_back(Edge());
        }

        // forall(tri, triangles)
        // {
        // 	edges[tri[0]].vertexEdges.insert(tri[1]);
        // 	edges[tri[0]].vertexEdges.insert(tri[2]);

        // 	edges[tri[1]].vertexEdges.insert(tri[0]);
        // 	edges[tri[1]].vertexEdges.insert(tri[2]);

        // 	edges[tri[2]].vertexEdges.insert(tri[0]);
        // 	edges[tri[2]].vertexEdges.insert(tri[1]);
        // }

        int nThreads = glm::max(1, (int)std::thread::hardware_concurrency());
        std::vector<std::thread> threads;
        for (int threadID = 0; threadID < nThreads; threadID++)
        {
            const int id = threadID;
            threads.push_back(std::thread([&, id]()
                {
                    for (int vertexID = id; vertexID < countUniqueVertices; vertexID += nThreads)
                    {
                        for (auto& tri : vertices[vertexID].triangleIDs)
                        {
                            // edges[vertexID].vertexEdges.insert(triangles[tri][0]);
                            // edges[vertexID].vertexEdges.insert(triangles[tri][1]);
                            // edges[vertexID].vertexEdges.insert(triangles[tri][2]);
                            // edges[vertexID].vertexEdges.erase(vertexID);

                            if (triangles[tri][0] == vertexID)
                            {
                                edges[vertexID].vertexEdges.insert(triangles[tri][1]);
                                edges[vertexID].vertexEdges.insert(triangles[tri][2]);
                            }
                            else if (triangles[tri][1] == vertexID)
                            {
                                edges[vertexID].vertexEdges.insert(triangles[tri][0]);
                                edges[vertexID].vertexEdges.insert(triangles[tri][2]);
                            }
                            else
                            {
                                edges[vertexID].vertexEdges.insert(triangles[tri][0]);
                                edges[vertexID].vertexEdges.insert(triangles[tri][1]);
                            }
                        }
                    }
                }));
        }

        for (auto& t : threads)
        {
            t.join();
        }
    }

    void EdgeList::cleanUpEdges()
    {
        const auto countofedges = edges.size();

        for (int i = 0; i < countofedges; i++)
        {
            // edges[i].cleanUp();
        }
    }

    void EdgeList::printAllEdges()
    {
        int tracker = 0;
        const int edgeCount = (const int)edges.size();

        for (int i = 0; i < edgeCount; i++)
        {
            spdlog::info("id : spc tracker << :");
            tracker++;
            edges[i].printEdge();
        }
    }

    const int EdgeList::edgesTotal() const
    {
        return (const int)edges.size();
    }

    glm::vec4 EdgeList::colorAverageAt(GLuint key)
    {
        glm::vec4 res = glm::vec4(0);
        auto& edge = edges[key].vertexEdges;

        for (auto& element : edge)
        {
            res += vertices[element].color;
        }

        res /= (float)edge.size();
        return res;
    }

    glm::vec3 EdgeList::averageAt(GLuint key)
    {
        glm::vec3 res = glm::vec3(0);
        auto& edge = edges[key].vertexEdges;

        for (auto& element : edge)
        {
            res += vertices[element].position;
        }

        res /= (float)edge.size();
        return res;
    }

    std::vector<GLuint> EdgeList::getEdgeTriangles(const std::pair<GLuint, GLuint>& edge) const
    {
        std::vector<GLuint> res;

        for (auto& tri : vertices[edge.second].triangleIDs)
        {
            if (vertices[edge.first].triangleIDs.contains(tri))
            {
                res.emplace_back(tri);
            }
        }

        return res;
    }

    std::vector<GLuint> EdgeList::getEdgeTriangles(GLuint v1, GLuint v2) const
    {
        std::vector<GLuint> res;

        for (auto& tri : vertices[v2].triangleIDs)
        {
            if (vertices[v1].triangleIDs.contains(tri))
            {
                res.emplace_back(tri);
            }
        }

        return res;
    }
}