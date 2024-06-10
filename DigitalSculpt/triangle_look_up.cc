#include "glm/glm.hpp"
#include "glm/gtx/normal.hpp"
#include "triangle_look_up.h"


namespace DigitalSculpt
{
    TriangleLookUp::TriangleLookUp()
    {
    }

    TriangleLookUp::~TriangleLookUp()
    {
    }

    glm::vec3 TriangleLookUp::getTriangleNormal(int key) const
    {
        const IndexedTriangle& triangle = triangles[key];
        return glm::triangleNormal(vertices[triangle[0]].position, vertices[triangle[1]].position, vertices[triangle[2]].position);
    }

    glm::vec3 TriangleLookUp::getTriangleCentroid(int key) const
    {
        const IndexedTriangle& triangle = triangles[key];
        const glm::vec3& pos0 = vertices[triangle[0]].position;
        const glm::vec3& pos1 = vertices[triangle[1]].position;
        const glm::vec3& pos2 = vertices[triangle[2]].position;
        float x = (pos0.x + pos1.x + pos2.x) / 3.0f;
        float y = (pos0.y + pos1.y + pos2.y) / 3.0f;
        float z = (pos0.z + pos1.z + pos2.z) / 3.0f;
        return glm::vec3(x, y, z);
    }

    float TriangleLookUp::getTriangleArea(int key) const
    {
        const IndexedTriangle& triangle = triangles[key];
        const glm::vec3& pos0 = vertices[triangle[0]].position;
        const glm::vec3& pos1 = vertices[triangle[1]].position;
        const glm::vec3& pos2 = vertices[triangle[2]].position;
        float a = glm::distance(pos0,pos1);
        float b = glm::distance(pos1,pos2);
        float c = glm::distance(pos2,pos0);
        float s = (a + b + c) * 0.5f;
        return glm::sqrt(s * (s - a) * (s - b) * (s - c));
    }
}