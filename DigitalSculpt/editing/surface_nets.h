#ifndef digital_sculpt_surface_nets_h__
#define digital_sculpt_surface_nets_h__

#include "glm/vec3.hpp"
#include "editing/voxels.h"
#include "misc/utils.h"

/**
 * Based on Mikola Lysenko SurfaceNets
 * https://github.com/mikolalysenko/isosurface
 *
 * Based on: S.F. Gibson, "Constrained Elastic Surface Nets". (1998) MERL Tech Report.
 */
namespace DigitalSculpt
{
    class SurfaceNets
    {
    private:
        static bool BLOCK;
        static Uint32Array cubeEdges;
        static Uint32Array edgeTable;

        // This is just the vertex number of each cube
        static void computeCubeEdges();

        static void computeEdgeTable(const Uint32Array& cube_edges);
    public:


        static std::uint8_t readScalarValues(const Voxels& voxels, Float32Array& grid, Uint32Array& dims, std::uint32_t n, Float32Array& cols, Float32Array& mats);

        static void interpolateVertices(std::uint32_t edgeMask, Uint32Array& cubeEdges, Float32Array& grid, Float32Array& x, Float32Array& vertices);

        static void createFace(std::uint32_t edgeMask, std::uint32_t  mask, Uint32Array& buffer,
            Uint32Array& R, std::uint32_t m, Uint32Array& x, Uint32Array& faces);

        static Voxels computeSurface(Voxels voxels);
    };
}

#endif // digital_sculpt_surface_nets_h__