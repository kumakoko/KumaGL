#ifndef digital_sculpt_geometry_h__
#define digital_sculpt_geometry_h__

#include <cmath>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "math3d/glm_extension.h"
#include "misc/utils.h"

namespace DigitalSculpt
{
    class Geometry
    {
    public:
        /******************************************************************************************************************
         * Desc:  Normalize coordinate mouse between -1 and 1
         * Method:    normalizedMouse
         * Returns:   glm::vec2
         * Parameter: float mouseX
         * Parameter: float mouseY
         * Parameter: float width
         * Parameter: float height
         ****************************************************************************************************************/
        static glm::vec2 normalizedMouse(float mouseX, float mouseY, float width, float height);

        /******************************************************************************************************************
         * Desc: Projection of mouse coordinate on sphere unit
         * Method:    mouseOnUnitSphere
         * Returns:   glm::vec3
         * Parameter: const glm::vec2 & mouseXY
         ****************************************************************************************************************/
        static glm::vec3 mouseOnUnitSphere(const glm::vec2& mouseXY);

        /******************************************************************************************************************
         * Desc: Compute intersection between a ray and a triangle. Returne the distance to the triangle if a hit occurs.
         * Method:    intersectionRayTriangleEdges
         * Returns:   float
         * Parameter: const glm::vec3 & orig
         * Parameter: const glm::vec3 & dir
         * Parameter: const glm::vec3 & edge1
         * Parameter: const glm::vec3 & edge2
         * Parameter: const glm::vec3 & v1
         * Parameter: glm::vec3 * vertInter
         ****************************************************************************************************************/
        static float intersectionRayTriangleEdges(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& edge1,
            const glm::vec3& edge2, const glm::vec3& v1, glm::vec3* vertInter = nullptr);

        /******************************************************************************************************************
         * Desc: Compute intersection between a ray and a triangle. Returne the distance to the triangle if a hit occurs.
         * Method:    intersectionRayTriangle
         * Returns:   float
         * Parameter: const glm::vec3 & orig
         * Parameter: const glm::vec3 & dir
         * Parameter: const glm::vec3 & v1
         * Parameter: const glm::vec3 & v2
         * Parameter: const glm::vec3 & v3
         * Parameter: glm::vec3 * vertInter
         ****************************************************************************************************************/
        static float intersectionRayTriangle(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& v1,
            const glm::vec3& v2, const glm::vec3& v3, glm::vec3* vertInter = nullptr);

        //
        // \2|
        //  \|
        //   \
        // 3 |\  1
        //   |0\
        // __|__\___
        // 4 | 5 \ 6
        /******************************************************************************************************************
         * Desc: Compute distance between a point and a triangle
         * Method:    distance2PointTriangleEdges
         * Returns:   float
         * Parameter: const glm::vec3 & point
         * Parameter: const glm::vec3 & edge1
         * Parameter: const glm::vec3 & edge2
         * Parameter: const glm::vec3 & v1
         * Parameter: float a00
         * Parameter: float a01
         * Parameter: float a11
         * Parameter: Uint32Array * closest
         ****************************************************************************************************************/
        static float distance2PointTriangleEdges(const glm::vec3& point, const glm::vec3& edge1, const glm::vec3& edge2,
            const glm::vec3& v1, float a00, float a01, float a11, Uint32Array* closest);

        /******************************************************************************************************************
         * Desc: Compute distance between a point and a triangle
         * Method:    distance2PointTriangle
         * Returns:   float
         * Parameter: const glm::vec3 & point
         * Parameter: const glm::vec3 & v1
         * Parameter: const glm::vec3 & v2
         * Parameter: const glm::vec3 & v3
         * Parameter: Uint32Array * closest
         ****************************************************************************************************************/
        static float distance2PointTriangle(const glm::vec3& point, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, Uint32Array* closest);


        /******************************************************************************************************************
         * Desc: If point is inside the triangle, test the sum of the areas
         * Method:    pointInsideTriangle
         * Returns:   bool
         * Parameter: const glm::vec3 & point
         * Parameter: const glm::vec3 & v1
         * Parameter: const glm::vec3 & v2
         * Parameter: const glm::vec3 & v3
         ****************************************************************************************************************/
        static bool pointInsideTriangle(const glm::vec3& point, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);

        /******************************************************************************************************************
         * Desc: If a sphere intersect a triangle
         * Method:    triangleInsideSphere
         * Returns:   bool
         * Parameter: const glm::vec3 & point
         * Parameter: float radiusSq
         * Parameter: const glm::vec3 & v1
         * Parameter: const glm::vec3 & v2
         * Parameter: const glm::vec3 & v3
         ****************************************************************************************************************/
        static bool triangleInsideSphere(const glm::vec3& point, float radiusSq, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);

        /******************************************************************************************************************
         * Desc: Minimum squared distance to a segment
         * Method:    distanceSqToSegment
         * Returns:   float
         * Parameter: const glm::vec3 & point
         * Parameter: const glm::vec3 & v1
         * Parameter: const glm::vec3 & v2
         ****************************************************************************************************************/
        static float distanceSqToSegment(const glm::vec3& point, const glm::vec3& v1, const glm::vec3& v2);

        /******************************************************************************************************************
         * Desc: Sign angle between two 2d vectors in radians
         * Method:    signedAngle2d
         * Returns:   float
         * Parameter: const glm::vec2 & v1
         * Parameter: const glm::vec2 & v2
         ****************************************************************************************************************/
        static float signedAngle2d(const glm::vec2& v1, const glm::vec2& v2);

        /******************************************************************************************************************
         * Desc: Distance from a vertex and a plane
         * Method:    pointPlaneDistance
         * Returns:   float
         * Parameter: const glm::vec3 & v
         * Parameter: const glm::vec3 & ptPlane
         * Parameter: const glm::vec3 & nPlane
         ****************************************************************************************************************/
        static float pointPlaneDistance(const glm::vec3& v, const glm::vec3& ptPlane, const glm::vec3& nPlane);

        /******************************************************************************************************************
         * Desc: Mirror a vertex according to a plane
         * Method:    mirrorPoint
         * Returns:   glm::vec3&
         * Parameter: glm::vec3 & v
         * Parameter: const glm::vec3 & ptPlane
         * Parameter: const glm::vec3 & nPlane
         ****************************************************************************************************************/
        static glm::vec3& mirrorPoint(glm::vec3& v, const glm::vec3& ptPlane, const glm::vec3& nPlane);

        /******************************************************************************************************************
         * Desc: Compute the projection of a vertex on a line
         * Method:    vertexOnLine
         * Returns:   glm::vec3
         * Parameter: const glm::vec3 & vertex
         * Parameter: const glm::vec3 & vNear
         * Parameter: const glm::vec3 & vFar
         ****************************************************************************************************************/
        static glm::vec3 vertexOnLine(const glm::vec3& vertex, const glm::vec3& vNear, const glm::vec3& vFar);

        /******************************************************************************************************************
         * Desc: Return the intersection between a ray and a plane
         * Method:    intersectLinePlane
         * Returns:   glm::vec3&
         * Parameter: glm::vec3 & s1
         * Parameter: glm::vec3 & s2
         * Parameter: const glm::vec3 & origin
         * Parameter: const glm::vec3 & normal
         * Parameter: glm::vec3 & out
         ****************************************************************************************************************/
        glm::vec3& intersectLinePlane(glm::vec3& s1, glm::vec3& s2, const glm::vec3& origin, const glm::vec3& normal, glm::vec3& out);

        /******************************************************************************************************************
         * Desc: Return any perpendicular vector to another (normalized) vector
         * Method:    getPerpendicularVector
         * Returns:   glm::vec3
         * Parameter: const glm::vec3 & vec
         ****************************************************************************************************************/
        glm::vec3 getPerpendicularVector(const glm::vec3& vec);
    };
}
#endif // digital_sculpt_geometry_h__
