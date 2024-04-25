#ifndef digital_sculpt_voxels_h__
#define digital_sculpt_voxels_h__

#include <array>
#include <vector>
#include <cstdint>

#include "misc/utils.h"

namespace DigitalSculpt
{
    class Voxels
    {
    public:
        float step;
        Float32Array dims;
        Float32Array min;
        Float32Array max;
        Float32Array distanceField;
        Float32Array colorField;
        Float32Array materialField;
        Uint8Array crossedEdges;
    };
}

#endif // digital_sculpt_voxels_h__
