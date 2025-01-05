#include "../klib/kgl_lib_pch.h"
#include "geometry.h"

namespace geometry {

    // Normalize mouse coordinates between -1 and 1
    jsm::vec2 normalizedMouse(float mouseX, float mouseY, float width, float height)
    {
        return { (2.0f * mouseX / width) - 1.0f, 1.0f - (2.0f * mouseY / height) };
    }

    // Projection of mouse coordinates on the unit sphere
    jsm::vec3 mouseOnUnitSphere(const jsm::vec2& mouseXY)
    {
        float mouseX = mouseXY[0];
        float mouseY = mouseXY[1];
        float tempZ = 1.0f - mouseX * mouseX - mouseY * mouseY;
        float mouseZ = tempZ > 0.0f ? std::sqrt(tempZ) : 0.0f;
        jsm::vec3 sphere = { mouseX, mouseY, mouseZ };
        float length = std::sqrt(sphere[0] * sphere[0] + sphere[1] * sphere[1] + sphere[2] * sphere[2]);
        for (float& val : sphere) val /= length;
        return sphere;
    }

    // Compute intersection between a ray and a triangle
    float intersectionRayTriangleEdges(
        const jsm::vec3& orig,
        const jsm::vec3& dir,
        const jsm::vec3& edge1,
        const jsm::vec3& edge2,
        const jsm::vec3& v1,
        jsm::vec3& vertInter
    )
    {
        constexpr float EPSILON = 1e-15f;
        jsm::vec3 pvec, tvec, qvec;

        // Cross product of dir and edge2
        pvec = { dir[1] * edge2[2] - dir[2] * edge2[1],
                dir[2] * edge2[0] - dir[0] * edge2[2],
                dir[0] * edge2[1] - dir[1] * edge2[0] };

        float det = std::inner_product(edge1.begin(), edge1.end(), pvec.begin(), 0.0f);
        if (std::abs(det) < EPSILON) return -1.0f;

        float invDet = 1.0f / det;
        std::transform(orig.begin(), orig.end(), v1.begin(), tvec.begin(), std::minus<>());
        float u = std::inner_product(tvec.begin(), tvec.end(), pvec.begin(), 0.0f) * invDet;
        if (u < 0.0f || u > 1.0f) return -1.0f;

        qvec = { tvec[1] * edge1[2] - tvec[2] * edge1[1],
                tvec[2] * edge1[0] - tvec[0] * edge1[2],
                tvec[0] * edge1[1] - tvec[1] * edge1[0] };

        float v = std::inner_product(dir.begin(), dir.end(), qvec.begin(), 0.0f) * invDet;
        if (v < 0.0f || u + v > 1.0f) return -1.0f;

        float t = std::inner_product(edge2.begin(), edge2.end(), qvec.begin(), 0.0f) * invDet;
        if (t < 0.0f) return -1.0f;

        vertInter = orig;
        for (size_t i = 0; i < 3; ++i) vertInter[i] += t * dir[i];
        return t;
    }

    // Compute intersection between a ray and a triangle (with vertices)
    float intersectionRayTriangle(
        const jsm::vec3& orig,
        const jsm::vec3& dir,
        const jsm::vec3& v1,
        const jsm::vec3& v2,
        const jsm::vec3& v3,
        jsm::vec3& vertInter
    ) 
    {
        jsm::vec3 edge1, edge2;
        std::transform(v2.begin(), v2.end(), v1.begin(), edge1.begin(), std::minus<>());
        std::transform(v3.begin(), v3.end(), v1.begin(), edge2.begin(), std::minus<>());
        return intersectionRayTriangleEdges(orig, dir, edge1, edge2, v1, vertInter);
    }

    float distanceSqToSegment(const jsm::vec3& point, const jsm::vec3& v1, const jsm::vec3& v2)
    {
        jsm::vec3 pt, v2v1;
        std::transform(point.begin(), point.end(), v1.begin(), pt.begin(), std::minus<>());
        std::transform(v2.begin(), v2.end(), v1.begin(), v2v1.begin(), std::minus<>());

        float lenSq = std::inner_product(v2v1.begin(), v2v1.end(), v2v1.begin(), 0.0f);
        float t = std::inner_product(pt.begin(), pt.end(), v2v1.begin(), 0.0f) / lenSq;

        if (t < 0.0f)
        {
            return std::inner_product(pt.begin(), pt.end(), pt.begin(), 0.0f);
        }
        else if (t > 1.0f)
        {
            jsm::vec3 ptToV2;
            std::transform(point.begin(), point.end(), v2.begin(), ptToV2.begin(), std::minus<>());
            return std::inner_product(ptToV2.begin(), ptToV2.end(), ptToV2.begin(), 0.0f);
        }
        else 
        {
            jsm::vec3 projection;
            std::transform(v2v1.begin(), v2v1.end(), projection.begin(), [t](float val) { return val * t; });
            jsm::vec3 ptToProjection;
            std::transform(pt.begin(), pt.end(), projection.begin(), ptToProjection.begin(), std::minus<>());
            return std::inner_product(ptToProjection.begin(), ptToProjection.end(), ptToProjection.begin(), 0.0f);
        }
    }

    // Mirror a point about a plane
    jsm::vec3 mirrorPoint(const jsm::vec3& point, const jsm::vec3& planePoint, const jsm::vec3& planeNormal)
    {
        jsm::vec3 diff;
        std::transform(point.begin(), point.end(), planePoint.begin(), diff.begin(), std::minus<>());
        float distance = std::inner_product(diff.begin(), diff.end(), planeNormal.begin(), 0.0f);

        jsm::vec3 scaledNormal;
        std::transform(planeNormal.begin(), planeNormal.end(), scaledNormal.begin(), [distance](float val) { return val * 2.0f * distance; });

        jsm::vec3 mirroredPoint;
        std::transform(point.begin(), point.end(), scaledNormal.begin(), mirroredPoint.begin(), std::minus<>());

        return mirroredPoint;
    }

} // namespace Geometry