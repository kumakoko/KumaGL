#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "indexed_triangle.h"
#include "v3d.h"
#include "vertex_list.h"
#include "triangle_list.h"


namespace DigitalSculpt
{
    class VertexLookUp : public VertexList, public TriangleList
    {
    public:
        VertexLookUp();
        ~VertexLookUp();

        /******************************************************************************************************************
         * Desc:
         * Method:    closest
         * Returns:   GLuint
         * Parameter: const glm::vec3 & pointToTest
         * Parameter: const IndexedTriangle & tri
         ****************************************************************************************************************/
        GLuint closest(const glm::vec3& pointToTest, const IndexedTriangle& tri);

        /******************************************************************************************************************
         * Desc:
         * Method:    lookUpV3D
         * Returns:   DigitalSculpt::V3D&
         * Parameter: GLuint key
         ****************************************************************************************************************/
        V3D& lookUpV3D(GLuint key);

        /******************************************************************************************************************
         * Desc:
         * Method:    lookUpV3D
         * Returns:   const DigitalSculpt::V3D&
         * Parameter: GLuint key
         ****************************************************************************************************************/
        const V3D& lookUpV3D(GLuint key) const;

        /******************************************************************************************************************
         * Desc:
         * Method:    lookUpTriangle
         * Returns:   DigitalSculpt::IndexedTriangle&
         * Parameter: GLuint key
         ****************************************************************************************************************/
        IndexedTriangle& lookUpTriangle(GLuint key);

        /******************************************************************************************************************
         * Desc:
         * Method:    lookUpTriangle
         * Returns:   const DigitalSculpt::IndexedTriangle&
         * Parameter: GLuint key
         ****************************************************************************************************************/
        const IndexedTriangle& lookUpTriangle(GLuint key) const;

        /******************************************************************************************************************
         * Desc:
         * Method:    findClosest
         * Returns:   GLuint
         * Parameter: const glm::vec3 & testpoint
         * Parameter: GLuint p0
         * Parameter: GLuint p1
         * Parameter: GLuint p2
         ****************************************************************************************************************/
        GLuint findClosest(const glm::vec3& testpoint, GLuint p0, GLuint p1, GLuint p2);
    };
}