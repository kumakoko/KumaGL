#ifndef digital_sculpt_marching_cubes_h__
#define digital_sculpt_marching_cubes_h__

#include <vector>
#include <cstdint>
#include "misc/utils.h"//import Utils from 'misc/Utils';

namespace DigitalSculpt
{
    class MarchingCubes
    {
    public:
        static bool BLOCK;

        static Uint32Array edgeTable;

        static std::vector<std::vector<std::uint32_t>> triTable;

        static std::vector<std::vector<std::uint32_t>> cubeVerts;

        static std::vector<std::vector<std::uint32_t>> edgeIndex;

        static std::uint32_t readScalarValues(voxels, grid, dims, n, cols, mats);

        static Voxel computeSurface(voxels);
    };
}

#endif // digital_sculpt_marching_cubes_h__