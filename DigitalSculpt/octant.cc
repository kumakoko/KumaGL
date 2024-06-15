#include "octant.h"

namespace DigitalSculpt
{
    const int NoOctantParentSet = -1;
    const int NoOctantChildSet = -1;
    const int NonExistentOctantIndex = -1;

    const std::vector<glm::vec3> octantPositionVectors =
    {
        glm::vec3(-1, -1, -1), // BottomBackLeft
        glm::vec3(-1, -1, 1),  // BottomFrontLeft
        glm::vec3(-1, 1, -1),  // TopBackLeft
        glm::vec3(-1, 1, 1),   // TopFrontLeft
        glm::vec3(1, -1, -1),  // BottomBackRight
        glm::vec3(1, -1, 1),   // BottomFrontRight
        glm::vec3(1, 1, -1),   // TopBackRight
        glm::vec3(1, 1, 1)     // TopFrontRight
    };

    Octant::Octant()
    {
        triangleIDs = std::make_shared<std::unordered_set<int>>();
    }

}