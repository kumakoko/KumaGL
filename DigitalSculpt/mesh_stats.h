#ifndef mesh_stats_h__
#define mesh_stats_h__

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

        glm::vec3 limMax = v3(0);
        glm::vec3 limMin = v3(0);
        glm::vec3 center = v3(0);

    };
}

#endif // mesh_stats_h__