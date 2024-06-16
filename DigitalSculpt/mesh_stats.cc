#include "mesh_stats.h"


namespace DigitalSculpt
{
    MeshStats::MeshStats()
    {
        limMax = glm::vec3(0);
        limMin = glm::vec3(0);
        center = glm::vec3(0);
    }

    MeshStats::~MeshStats()
    {
    }

    void MeshStats::collectStats()
    {
        glm::vec3 max = glm::vec3(0);
        glm::vec3 min = glm::vec3(0);
        glm::vec3 tempCenter = glm::vec3(0);

        // 遍历每一个顶点
        for (auto& vert : vertices)
        {
            // 累加每一个顶点坐标的xyz
            tempCenter += vert.position;

            // 如果当前遍历到的顶点，它的x值大于临时最大X值，则将它的x值设为临时最大x值
            if (vert.position.x > max.x)
            {
                max.x = vert.position.x;
            }
            // 如果当前遍历到的顶点，它的x值小于临时最小X值，则将它的x值设为临时最小x值
            else if (vert.position.x < min.x)
            {
                min.x = vert.position.x;
            }

            // y,z的同理
            if (vert.position.y > max.y)
            {
                max.y = vert.position.y;
            }
            else if (vert.position.y < min.y)
            {
                min.y = vert.position.y;
            }

            if (vert.position.z > max.z)
            {
                max.z = vert.position.z;
            }
            else if (vert.position.z < min.z)
            {
                min.z = vert.position.z;
            }
        }

        this->limMax = max;
        this->limMin = min;

        // xyz坐标值之和，各自除以顶点的个数，得到xyz坐标值的平均值
        this->center = tempCenter / (float)verticeCount();
    }
}