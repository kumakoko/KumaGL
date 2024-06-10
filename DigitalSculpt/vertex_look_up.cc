
#include "glm/geometric.hpp"
#include "vertex_look_up.h"

namespace DigitalSculpt
{
    VertexLookUp::VertexLookUp()
    {

    }

    VertexLookUp::~VertexLookUp()
    {
    }

    GLuint VertexLookUp::closest(const glm::vec3& pointToTest, const IndexedTriangle& tri)
    {
        const float dist1 = glm::distance(vertices[tri[0]].position, pointToTest);
        const float dist2 = glm::distance(vertices[tri[1]].position, pointToTest);
        const float dist3 = glm::distance(vertices[tri[2]].position, pointToTest);

        if (dist1 <= dist2 && dist1 <= dist3)
        {
            return tri[0];
        }
        else if(dist2 <= dist1 && dist2 <= dist3)
        {
            return tri[1];
        }

        return tri[2];
    }

    V3D& VertexLookUp::lookUpV3D(GLuint key)
    {
        return vertices[key];
    }

    const V3D& VertexLookUp::lookUpV3D(GLuint key) const
    {
        return vertices[key];
    }

    IndexedTriangle& VertexLookUp::lookUpTriangle(GLuint key)
    {
        return triangles[key];
    }

    const IndexedTriangle& VertexLookUp::lookUpTriangle(GLuint key) const
    {
        return triangles[key];
    }

    GLuint VertexLookUp::findClosest(const glm::vec3& testpoint, GLuint p0, GLuint p1, GLuint p2)
    {
        GLuint closest = ImpossibleKey;
        return closest;
    }
}