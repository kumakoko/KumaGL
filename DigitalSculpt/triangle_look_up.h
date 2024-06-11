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

        /******************************************************************************************************************
         * Desc: 根据给定的三角形索引id，查找到该三角形，然后获取到该三角形的法线值
         * Method:    getTriangleNormal
         * Returns:   glm::vec3
         * Parameter: int key
         ****************************************************************************************************************/
         glm::vec3 getTriangleNormal(int key) const;

        /******************************************************************************************************************
         * Desc: 根据给定的三角形索引id，查找到该三角形，然后获取到该三角形的中心点
         * Method:    getTriangleCentroid
         * Returns:   glm::vec3
         * Parameter: int key
         ****************************************************************************************************************/
         glm::vec3 getTriangleCentroid(int key) const;
        
         /******************************************************************************************************************
         * Desc: 根据给定的三角形索引id，查找到该三角形，然后获取到该三角形的面积
         * Method:    getTriangleArea
         * Returns:   float
         * Parameter: int key
         ****************************************************************************************************************/
         float getTriangleArea(int key) const;
    };
}

#endif // triangle_look_up_h__

