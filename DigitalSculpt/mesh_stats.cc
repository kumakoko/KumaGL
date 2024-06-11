#include "mesh_stats.h"


namespace DigitalSculpt
{
    MeshStats::MeshStats()
    {
    }

    MeshStats::~MeshStats()
    {
    }

    /*
        Collect All Relevant Stats in a mesh linearly.
    */
    void MeshStats::collectStats()
    {
        glm::vec3 max = glm::vec3(0);
        glm::vec3 min = glm::vec3(0);
        glm::vec3 tempCenter = glm::vec3(0);

        for (auto& vert : vertices)
        {
            tempCenter += vert.position;

            if (vert.position.x > max.x)
            {
                max.x = vert.position.x;
            }
            else if (vert.position.x < min.x)
            {
                min.x = vert.position.x;
            }

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
        this->center = tempCenter / (float)verticeCount();
    }
}