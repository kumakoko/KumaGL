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
        /***************************************************
        
        @name: DigitalSculpt::SurfaceNets::computeCubeEdges
        @return: void
        ***************************************************/
        static void computeCubeEdges();

        /***************************************************
        
        @name: DigitalSculpt::SurfaceNets::computeEdgeTable
        @return: void
        @param: const Uint32Array & cube_edges
        ***************************************************/
        static void computeEdgeTable(const Uint32Array& cube_edges);
    public:


        /***************************************************
        
        @name: DigitalSculpt::SurfaceNets::readScalarValues
        @return: std::uint8_t
        @param: const Voxels & voxels
        @param: Float32Array & grid
        @param: Uint32Array & dims
        @param: std::uint32_t n
        @param: Float32Array & cols
        @param: Float32Array & mats
        ***************************************************/
        static std::uint8_t readScalarValues(const Voxels& voxels, Float32Array& grid, Uint32Array& dims, std::uint32_t n, Float32Array& cols, Float32Array& mats);

        /***************************************************
        
        @name: DigitalSculpt::SurfaceNets::interpolateVertices
        @return: void
        @param: std::uint32_t edgeMask
        @param: Uint32Array & cubeEdges
        @param: Float32Array & grid
        @param: Float32Array & x
        @param: Float32Array & vertices
        ***************************************************/
        static void interpolateVertices(std::uint32_t edgeMask, Uint32Array& cubeEdges, Float32Array& grid, Float32Array& x, Float32Array& vertices);

        /***************************************************
        
        @name: DigitalSculpt::SurfaceNets::createFace
        @return: void
        @param: std::uint32_t edgeMask
        @param: std::uint32_t mask
        @param: Uint32Array & buffer
        @param: Uint32Array & R
        @param: std::uint32_t m
        @param: Uint32Array & x
        @param: Uint32Array & faces
        ***************************************************/
        static void createFace(std::uint32_t edgeMask, std::uint32_t  mask, Uint32Array& buffer,
            Uint32Array& R, std::uint32_t m, Uint32Array& x, Uint32Array& faces);

        static Voxels computeSurface(Voxels voxels);
    };
}

#endif // digital_sculpt_surface_nets_h__