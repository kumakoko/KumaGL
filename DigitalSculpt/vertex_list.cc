// 顶点列表
#include "vertex_list.h"

namespace DigitalSculpt
{
    VertexList::VertexList()
    {
    }
    VertexList::~VertexList()
    {
    }
    // sets the element by index
    void VertexList::setVertex(const V3D& element, int index)
    {
        vertices[index] = element;
    }

    const int VertexList::verticesMemorySize()
    {
        return (const int)vertices.size() * sizeof(V3D);
    }

    void VertexList::colorDataUniformly(const glm::vec4& uniformColor)
    {
        int countOfVertices = (int)vertices.size();
        if (countOfVertices == 0)
        {
            // throw error, there are no vertices to color.
        }
        else
        {
            for (auto& vert : vertices)
            {
                vert.color = uniformColor;
            }
        }
    }

    int VertexList::verticeCount()
    {
        return (int)vertices.size();
    }

    // returns the underlying vector array component.
    // auto returned because figuring out this data type is a waste of time.
    const V3D* VertexList::getVertexArray()
    {
        return vertices.data();
    }

    // gets the element referenece by index
    V3D& VertexList::getVertex(int index)
    {
        return vertices[index];
    }

    const V3D& VertexList::getVertex(int index) const
    {
        return vertices[index];
    }

    std::vector<int> VertexList::getTrianglesFromVertices(const std::vector<GLuint>& vertexIDs)
    {
        std::unordered_set<int> triangleIDset;

        for (auto& vertex : vertexIDs)
        {
            for (auto& triangle : vertices[vertex].triangleIDs)
            {
                triangleIDset.insert(triangle);
            }
        }

        return std::vector<int>(triangleIDset.begin(), triangleIDset.end());
    }
}
