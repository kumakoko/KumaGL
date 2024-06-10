#ifndef triangle_look_up_h__
#define triangle_look_up_h__

#include "glm/glm.hpp"

#include "edge_list.h"

namespace DigitalSculpt
{
    class TriangleLookUp : public EdgeList
    {
    public:
        TriangleLookUp();
        ~TriangleLookUp();

        // 根据给定的三角形索引id，查找到该三角形，然后获取到该三角形的法线值
        glm::vec3 getTriangleNormal(int key) const; // compute the normal from a given key corresponding to a *triangle*
        glm::vec3 getTriangleCentroid(int key) const; // get the triangle centroid -> center of a triangle
        float getTriangleArea(int key) const; // get the area of a triangle
    };
}

#endif // triangle_look_up_h__

