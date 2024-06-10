#ifndef vertex_look_up_h__
#define vertex_look_up_h__

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "v3d.h"
#include "vertex_list.h"
#include "triangle_list.h"
#include "indexed_triangle.h"

namespace DigitalSculpt
{
    class VertexLookUp : public VertexList, public TriangleList
    {
    public:
        VertexLookUp();
        ~VertexLookUp();
        GLuint closest(const glm::vec3& pointToTest, const IndexedTriangle& tri);
        V3D& lookUpV3D(GLuint key);
        const V3D& lookUpV3D(GLuint key) const;
        IndexedTriangle& lookUpTriangle(GLuint key);
        const IndexedTriangle& lookUpTriangle(GLuint key) const;
        GLuint findClosest(const glm::vec3& testpoint, GLuint p0, GLuint p1, GLuint p2);
    };
}

#endif // vertex_look_up_h__


