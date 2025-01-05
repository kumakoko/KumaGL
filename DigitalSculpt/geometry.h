#ifndef geometry_h__
#define geometry_h__

#include "jsm_type_defines.h"

using namespace jsm;

namespace geometry
{
    // Normalize mouse coordinates between -1 and 1
    jsm::vec2 normalizedMouse(float mouseX, float mouseY, float width, float height);

    // Projection of mouse coordinates on the unit sphere
    jsm::vec3 mouseOnUnitSphere(const jsm::vec2& mouseXY);

    // Compute intersection between a ray and a triangle
    float intersectionRayTriangleEdges(
        const jsm::vec3& orig,
        const jsm::vec3& dir,
        const jsm::vec3& edge1,
        const jsm::vec3& edge2,
        const jsm::vec3& v1,
        jsm::vec3& vertInter
    );

    // Compute intersection between a ray and a triangle (with vertices)
    float intersectionRayTriangle(
        const jsm::vec3& orig,
        const jsm::vec3& dir,
        const jsm::vec3& v1,
        const jsm::vec3& v2,
        const jsm::vec3& v3,
        jsm::vec3& vertInter
    );

    float distanceSqToSegment(const jsm::vec3& point, const jsm::vec3& v1, const jsm::vec3& v2);

    jsm::vec3 mirrorPoint(const jsm::vec3& point, const jsm::vec3& planePoint, const jsm::vec3& planeNormal);

} // namespace Geometry

#endif // geometry_h__
