#include <cmath>
#include "glm/gtx/norm.hpp"
#include "math3d/glm_extension.h"
#include "math3d/geometry.h"

namespace DigitalSculpt
{
    glm::vec2 Geometry::normalizedMouse(float mouseX, float mouseY, float width, float height)
    {
        return glm::vec2((2.0f * mouseX / width) - 1.0f, 1.0f - (2.0f * mouseY / height));
    }

    glm::vec3 Geometry::mouseOnUnitSphere(const glm::vec2& mouseXY)
    {
        float mouseX = mouseXY.x;
        float mouseY = mouseXY.y;
        float tempZ = 1.0 - mouseX * mouseX - mouseY * mouseY;
        float mouseZ = tempZ > 0.0f ? std::sqrt(tempZ) : 0.0f;
        return glm::normalize(glm::vec3(mouseX, mouseY, mouseZ));
        //glm::vec3 sourisSphere = [mouseX, mouseY, mouseZ];
        //return vec3.normalize(sourisSphere, sourisSphere);
    }

    float Geometry::intersectionRayTriangleEdges(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& edge1,
        const glm::vec3& edge2, const glm::vec3& v1, glm::vec3* vertInter)
    {
        float EPSILON = 1E-15;
        // hmm we favor false positive just in case...
        // mainly because of the voxel-remesh that can do weird things
        // if the ray casting fail on a border of a triangle 
        float ONE_PLUS_EPSILON = 1.0f + EPSILON;
        float ZERO_MINUS_EPSILON = 0.0f - EPSILON;
        //glm::vec3 pvec(0.0f);// = [0.0, 0.0, 0.0];
        glm::vec3 tvec(0.0f);// = [0.0, 0.0, 0.0];
        //glm::vec3 qvec(0.0f);// = [0.0, 0.0, 0.0];

        // moller trumbore intersection algorithm
        glm::vec3 pvec = glm::cross(dir, edge2);//vec3.cross(pvec, dir, edge2);
        float det = glm::dot(edge1, pvec);//var det = vec3.dot(edge1, pvec);
        if (det > -EPSILON && det < EPSILON)
            return -1.0f;

        float invDet = 1.0f / det;
        glm::vec3 tvec = orig - v1;//vec3.sub(tvec, orig, v1);
        float u = glm::dot(tvec, pvec) * invDet; //var u = vec3.dot(tvec, pvec) * invDet;
        if (u < ZERO_MINUS_EPSILON || u > ONE_PLUS_EPSILON)
            return -1.0f;

        glm::vec3 qvec = glm::cross(tvec, edge1);//vec3.cross(qvec, tvec, edge1);
        float v = glm::dot(dir, qvec) * invDet;
        if (v < ZERO_MINUS_EPSILON || u + v > ONE_PLUS_EPSILON)
            return -1.0f;

        float t = glm::dot(edge2, qvec) * invDet;
        if (t < ZERO_MINUS_EPSILON)
            return -1.0f;

        if (vertInter != nullptr)
            GlmExtension::scaleAndAdd(*vertInter, orig, dir, t);

        return t;
    }

    float Geometry::intersectionRayTriangle(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& v1,
        const glm::vec3& v2, const glm::vec3& v3, glm::vec3* vertInter)
    {
        /* var edge1 = [0.0, 0.0, 0.0];
         var edge2 = [0.0, 0.0, 0.0];*/

        glm::vec3 edge1 = v2 - v1;//vec3.sub(edge1, v2, v1);
        glm::vec3 edge2 = v3 - v1;//vec3.sub(edge2, v3, v1);
        return intersectionRayTriangleEdges(orig, dir, edge1, edge2, v1, vertInter);

    };

    float Geometry::distance2PointTriangleEdges(const glm::vec3& point, const glm::vec3& edge1, const glm::vec3& edge2,
        const glm::vec3& v1, float a00, float a01, float a11, Uint32Array* closest)
    {
        glm::vec3 diff = v1 - point;//vec3.sub(diff, v1, point);
        float b0 = glm::dot(diff, edge1);//var b0 = vec3.dot(diff, edge1);
        float b1 = glm::dot(diff, edge2);//var b1 = vec3.dot(diff, edge2);
        float c = glm::length2(diff);//var c = vec3.sqrLen(diff);
        float det = std::fabs(a00 * a11 - a01 * a01);//var det = Math.abs(a00 * a11 - a01 * a01);
        float s = a01 * b1 - a11 * b0;
        float t = a01 * b0 - a00 * b1;
        float sqrDistance;
        std::uint32_t zone = 4;

        if (s + t <= det)
        {
            if (s < 0.0f)
            {
                if (t < 0.0f)
                {
                    // region 4
                    zone = 4;
                    if (b0 < 0.0f)
                    {
                        t = 0.0f;

                        if (-b0 >= a00)
                        {
                            s = 1.0f;
                            sqrDistance = a00 + 2.0f * b0 + c;
                        }
                        else
                        {
                            s = -b0 / a00;
                            sqrDistance = b0 * s + c;
                        }
                    }
                    else
                    {
                        s = 0.0f;

                        if (b1 >= 0.0f)
                        {
                            t = 0.0f;
                            sqrDistance = c;
                        }
                        else if (-b1 >= a11)
                        {
                            t = 1.0f;
                            sqrDistance = a11 + 2.0f * b1 + c;
                        }
                        else
                        {
                            t = -b1 / a11;
                            sqrDistance = b1 * t + c;
                        }
                    }
                }
                else
                {
                    // region 3
                    zone = 3;
                    s = 0.0f;
                    if (b1 >= 0.0f)
                    {
                        t = 0.0f;
                        sqrDistance = c;
                    }
                    else if (-b1 >= a11)
                    {
                        t = 1.0f;
                        sqrDistance = a11 + 2.0 * b1 + c;
                    }
                    else
                    {
                        t = -b1 / a11;
                        sqrDistance = b1 * t + c;
                    }
                }
            }
            else if (t < 0.0f)
            {
                // region 5
                zone = 5;
                t = 0.0f;

                if (b0 >= 0.0f)
                {
                    s = 0.0f;
                    sqrDistance = c;
                }
                else if (-b0 >= a00)
                {
                    s = 1.0f;
                    sqrDistance = a00 + 2.0f * b0 + c;
                }
                else
                {
                    s = -b0 / a00;
                    sqrDistance = b0 * s + c;
                }
            }
            else
            { // region 0
                zone = 0;
                // minimum at interior point
                s /= det;
                t /= det;
                sqrDistance = s * (a00 * s + a01 * t + 2.0f * b0) + t * (a01 * s + a11 * t + 2.0f * b1) + c;
            }
        }
        else
        {
            float tmp0, tmp1, numer, denom;

            if (s < 0.0f)
            {
                // region 2
                zone = 2;
                tmp0 = a01 + b0;
                tmp1 = a11 + b1;

                if (tmp1 > tmp0)
                {
                    numer = tmp1 - tmp0;
                    denom = a00 - 2.0f * a01 + a11;

                    if (numer >= denom)
                    {
                        s = 1.0f;
                        t = 0.0f;
                        sqrDistance = a00 + 2.0f * b0 + c;
                    }
                    else
                    {
                        s = numer / denom;
                        t = 1.0f - s;
                        sqrDistance = s * (a00 * s + a01 * t + 2.0f * b0) + t * (a01 * s + a11 * t + 2.0f * b1) + c;
                    }
                }
                else
                {
                    s = 0.0f;

                    if (tmp1 <= 0.0f)
                    {
                        t = 1.0f;
                        sqrDistance = a11 + 2.0f * b1 + c;
                    }
                    else if (b1 >= 0.0)
                    {
                        t = 0.0f;
                        sqrDistance = c;
                    }
                    else
                    {
                        t = -b1 / a11;
                        sqrDistance = b1 * t + c;
                    }
                }
            }
            else if (t < 0.0)
            {
                // region 6
                zone = 6;
                tmp0 = a01 + b1;
                tmp1 = a00 + b0;

                if (tmp1 > tmp0)
                {
                    numer = tmp1 - tmp0;
                    denom = a00 - 2.0f * a01 + a11;

                    if (numer >= denom)
                    {
                        t = 1.0f;
                        s = 0.0f;
                        sqrDistance = a11 + 2.0f * b1 + c;
                    }
                    else
                    {
                        t = numer / denom;
                        s = 1.0f - t;
                        sqrDistance = s * (a00 * s + a01 * t + 2.0f * b0) + t * (a01 * s + a11 * t + 2.0f * b1) + c;
                    }
                }
                else
                {
                    t = 0.0f;

                    if (tmp1 <= 0.0f)
                    {
                        s = 1.0f;
                        sqrDistance = a00 + 2.0f * b0 + c;
                    }
                    else if (b0 >= 0.0)
                    {
                        s = 0.0f;
                        sqrDistance = c;
                    }
                    else
                    {
                        s = -b0 / a00;
                        sqrDistance = b0 * s + c;
                    }
                }
            }
            else
            { // region 1
                zone = 1;
                numer = a11 + b1 - a01 - b0;

                if (numer <= 0.0f)
                {
                    s = 0.0f;
                    t = 1.0f;
                    sqrDistance = a11 + 2.0f * b1 + c;
                }
                else
                {
                    denom = a00 - 2.0f * a01 + a11;

                    if (numer >= denom)
                    {
                        s = 1.0f;
                        t = 0.0f;
                        sqrDistance = a00 + 2.0f * b0 + c;
                    }
                    else
                    {
                        s = numer / denom;
                        t = 1.0 - s;
                        sqrDistance = s * (a00 * s + a01 * t + 2.0f * b0) + t * (a01 * s + a11 * t + 2.0f * b1) + c;
                    }
                }
            }
        }

        // Account for numerical round-off error.
        if (sqrDistance < 0.0f)
            sqrDistance = 0.0f;

        if (closest != nullptr)
        {
            closest->at(0) = v1[0] + s * edge1[0] + t * edge2[0];
            closest->at(1) = v1[1] + s * edge1[1] + t * edge2[1];
            closest->at(2) = v1[2] + s * edge1[2] + t * edge2[2];
            closest->at(3) = zone;
        }

        return sqrDistance;
    }

    float Geometry::distance2PointTriangle(const glm::vec3& point, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, Uint32Array* closest)
    {
        //glm::vec3 edge1(0.0f);// = [0.0, 0.0, 0.0];
        glm::vec3 edge2(0.0f);// = [0.0, 0.0, 0.0];

        glm::vec3 edge1 = v2 - v1;// vec3.sub(edge1, v2, v1);
        glm::vec3 edge2 = v3 - v1;//   vec3.sub(edge2, v3, v1);
        float a00 = glm::length2(edge1);//var a00 = vec3.sqrLen(edge1);
        float a01 = glm::dot(edge1, edge2);//var a01 = vec3.dot(edge1, edge2);
        float a11 = glm::length2(edge2);//var a11 = vec3.sqrLen(edge2);
        return distance2PointTriangleEdges(point, edge1, edge2, v1, a00, a01, a11, closest);
    }

    bool Geometry::pointInsideTriangle(const glm::vec3& point, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
    {
        /* var vec1 = [0.0, 0.0, 0.0];
         var vec2 = [0.0, 0.0, 0.0];
         var vecP1 = [0.0, 0.0, 0.0];
         var vecP2 = [0.0, 0.0, 0.0];
         var temp = [0.0, 0.0, 0.0];*/

        glm::vec3 vec1 = v1 - v2;//vec3.sub(vec1, v1, v2);
        glm::vec3 vec2 = v1 - v3;//vec3.sub(vec2, v1, v3);
        glm::vec3 vecP1 = point - v2;//vec3.sub(vecP1, point, v2);
        glm::vec3 vecP2 = point - v3;//vec3.sub(vecP2, point, v3);
        float total = glm::length(glm::cross(vec1, vec2));//var total = vec3.len(vec3.cross(temp, vec1, vec2));
        float area1 = glm::length(glm::cross(vec1, vecP1));//var area1 = vec3.len(vec3.cross(temp, vec1, vecP1));
        float area2 = glm::length(glm::cross(vec2, vecP2));//var area2 = vec3.len(vec3.cross(temp, vec2, vecP2));
        float area3 = glm::length(glm::cross(vecP1, vecP2));//var area3 = vec3.len(vec3.cross(temp, vecP1, vecP2));
        return std::fabs(total - (area1 + area2 + area3)) < 1E-20;

    }


    bool Geometry::triangleInsideSphere(const glm::vec3& point, float radiusSq, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
    {
        if (Geometry::distanceSqToSegment(point, v1, v2) < radiusSq) return true;
        if (Geometry::distanceSqToSegment(point, v2, v3) < radiusSq) return true;
        if (Geometry::distanceSqToSegment(point, v1, v3) < radiusSq) return true;
        return false;
    };


    float Geometry::distanceSqToSegment(const glm::vec3& point, const glm::vec3& v1, const glm::vec3& v2)
    {
        //  var pt = [0.0, 0.0, 0.0];
        //  var v2v1 = [0.0, 0.0, 0.0];

        glm::vec3 pt = point - v1;// vec3.sub(pt, point, v1);
        glm::vec3 v2v1 = v2 - v1;//    vec3.sub(v2v1, v2, v1);
        float len = glm::length2(v2v1); //var len = vec3.sqrLen(v2v1);
        float t = glm::dot(pt, v2v1) / len;
        if (t < 0.0f)
            return glm::length2(pt);
        if (t > 1.0f)
        {
            pt = point - v2;
            return glm::length2(pt);
        }

        pt.x = point.x - v1.x - t * v2v1.x;
        pt.y = point.y - v1.y - t * v2v1.y;
        pt.z = point.z - v1.z - t * v2v1.z;

        pt = point;
        pt -= v1;
        pt -= t * v2v1;

        return glm::length(pt);
    }


    float Geometry::signedAngle2d(const glm::vec2& v1, const glm::vec2& v2)
    {
        float v1x = v1.x,
            float v1y = v1.y,
            float v2x = v2.x,
            float v2y = v2.y;
        return std::atan2f(v1x * v2y - v1y * v2x, v1x * v2x + v1y * v2y);
    };


    float Geometry::pointPlaneDistance(const glm::vec3& v, const glm::vec3& ptPlane, const glm::vec3& nPlane)
    {
        glm::vec3 temp = v - ptPlane;
        return glm::dot(temp, nPlane);
    }


    glm::vec3& Geometry::mirrorPoint(glm::vec3& v, const glm::vec3& ptPlane, const glm::vec3& nPlane)
    {
        //var temp = [0.0, 0.0, 0.0];

            //return vec3.sub(v, v, vec3.scale(temp, nPlane, Geometry.pointPlaneDistance(v, ptPlane, nPlane) * 2.0));
        glm::vec3 temp(nPlane);
        float s = pointPlaneDistance(v, ptPlane, nPlane) * 2.0f;
        temp *= s;
        v -= temp;
        return v;
    }


    glm::vec3 Geometry::vertexOnLine(const glm::vec3& vertex, const glm::vec3& vNear, const glm::vec3& vFar)
    {
        glm::vec3 ab = vFar - vNear;
        glm::vec3 proj = vertex - vNear;
        float dot = glm::dot(ab, proj);//var dot = vec3.dot(ab, vec3.sub(proj, vertex, vNear));
        return GlmExtension::scaleAndAdd(proj, vNear, ab, dot / glm::length2(ab));
    }


    glm::vec3& Geometry::intersectLinePlane(glm::vec3& s1, glm::vec3& s2, const glm::vec3& origin, const glm::vec3& normal, glm::vec3& out)
    {
        out = s1 - origin;
        float dist1 = glm::dot(out, normal);//var dist1 = vec3.dot(vec3.sub(out, s1, origin), normal);
        out = s2 - origin;
        float dist2 = glm::dot(out, normal);//var dist2 = vec3.dot(vec3.sub(out, s2, origin), normal);
        // ray copplanar to triangle
        if (dist1 == dist2)
            return s2;
        // intersection between ray and triangle
        float val = -dist1 / (dist2 - dist1);
        //return vec3.scaleAndAdd(out, s1, vec3.sub(out, s2, s1), val);
        out = s2 - s1;
        return GlmExtension::scaleAndAdd(out, s1, out, val);
    }

    glm::vec3 Geometry::getPerpendicularVector(const glm::vec3& vec)
    {
        glm::vec3 perp(0.0f);// = [0.0, 0.0, 0.0];

        if (vec[0] == 0.0f)
        {
            perp[0] = 1.0f;
        }
        else if (vec[1] == 0.0f)
        {
            perp[1] = 1.0f;
        }
        else if (vec[2] == 0.0f)
        {
            perp[2] = 1.0f;
        }
        else
        {
            perp.x = vec.z;
            perp.y = vec.z;
            perp.z = -vec.x - vec.y;
            perp = glm::normalize(perp);//vec3.normalize(perp, perp);
        }
        return perp;
    }

}