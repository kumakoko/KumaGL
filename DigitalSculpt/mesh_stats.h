#pragma once

#include "glm/glm.hpp"
#include "graphics_data.h"

namespace DigitalSculpt
{
    class MeshStats : public GraphicsData
    {
    public:
        MeshStats();
        ~MeshStats();

        void collectStats();

        glm::vec3 limMax = glm::vec3(0);
        glm::vec3 limMin = glm::vec3(0);
        glm::vec3 center = glm::vec3(0);

    };
}