#include "glm/vec3.hpp"
#include "editing/surface_nets.h"
#include "misc/utils.h"

namespace DigitalSculpt
{

    bool SurfaceNets::BLOCK = false;

    //Precompute edge table, like Paul Bourke does.
    Uint32Array SurfaceNets::cubeEdges;// = computeCubeEdges();
    Uint32Array  SurfaceNets::edgeTable;// = computeEdgeTable(cubeEdges);

    // This is just the vertex number of each cube
    void SurfaceNets::computeCubeEdges()
    {
        if (cubeEdges.size() > 0)
            return;

        cubeEdges.resize(24, 0);// = new Uint32Array(24);
        std::uint32_t k = 0;

        for (std::uint32_t i = 0; i < 8; ++i)
        {
            for (std::uint32_t j = 1; j <= 4; j <<= 1)
            {
                std::uint32_t p = i ^ j;

                if (i <= p)
                {
                    cubeEdges[k++] = i;
                    cubeEdges[k++] = p;
                }
            }
        }
    }

    void SurfaceNets::computeEdgeTable(const Uint32Array& cube_edges)
    {
        computeCubeEdges();

        if (edgeTable.size() > 0)
            return;


        //Initialize the intersection table.
        //  This is a 2^(cube configuration) ->  2^(edge configuration) map
        //  There is one entry for each possible cube configuration, and the output is a 12-bit vector enumerating all edges crossing the 0-level.
        edgeTable.resize(256, 0);//auto edgeTable = new Uint32Array(256);

        for (std::uint32_t i = 0; i < 256; ++i)
        {
            std::uint32_t em = 0;

            for (std::uint32_t j = 0; j < 24; j += 2)
            {
                bool a = (i & (1 << cubeEdges[j + 0])) != 0;
                bool b = (i & (1 << cubeEdges[j + 1])) != 0;
                em |= a != b ? (1 << (j >> 1)) : 0;
            }

            edgeTable[i] = em;
        }
    }


    std::uint8_t SurfaceNets::readScalarValues(const Voxels& voxels, Float32Array& grid, Uint32Array& dims, std::uint32_t n, Float32Array& cols, Float32Array& mats)
    {
        //auto colors = voxels.colorField;
        //auto materials = voxels.materialField;
        //auto data = voxels.distanceField;

        //Read in 8 field values around this vertex and store them in an array
        //Also calculate 8-bit mask, like in marching cubes, so we can speed up sign checks later
        float c1 = 0;
        float c2 = 0;
        float c3 = 0;
        float m1 = 0;
        float m2 = 0;
        float m3 = 0;
        float invSum = 0.0f;

        std::uint8_t mask = 0;
        std::uint8_t g = 0;
        std::uint32_t rx = dims[0];
        std::uint32_t rxy = dims[0] * dims[1];

        float Infinity = std::numeric_limits<float>::infinity();

        for (auto k = 0; k < 2; ++k)
        {
            for (auto j = 0; j < 2; ++j)
            {
                for (auto i = 0; i < 2; ++i)
                {
                    auto id = n + i + j * rx + k * rxy;
                    auto id3 = id * 3;
                    float p = voxels.distanceField[id];
                    grid[g] = p;
                    mask |= (p < 0.0f) ? (1 << g) : 0;
                    g++;

                    if (p != Infinity)
                    {
                        p = Utils::min(1.0f / std::fabs(p), 1e15);
                        invSum += p;
                        c1 += voxels.colorField[id3 + 0] * p;
                        c2 += voxels.colorField[id3 + 1] * p;
                        c3 += voxels.colorField[id3 + 2] * p;
                        m1 += voxels.materialField[id3 + 0] * p;
                        m2 += voxels.materialField[id3 + 1] * p;
                        m3 += voxels.materialField[id3 + 2] * p;
                    }
                }
            }
        }

        if (mask != 0 && mask != 0xff)
        {
            if (invSum > 0.0f)
                invSum = 1.0f / invSum;

            Utils::PushToVector(cols, c1 * invSum, c2 * invSum, c3 * invSum);
            Utils::PushToVector(mats, m1 * invSum, m2 * invSum, m3 * invSum);
        }

        return mask;
    }

    //auto vTemp = [0.0, 0.0, 0.0];

    static void SurfaceNets::interpolateVertices(std::uint32_t edgeMask, Uint32Array& cubeEdges, Float32Array& grid, Float32Array& x, Float32Array& vertices)
    {
        float vTemp[3] = { 0.0f,0.0f,0.0f }; //vTemp[0] = vTemp[1] = vTemp[2] = 0.0;
        auto edgeCount = 0;

        //For every edge of the cube...
        for (auto i = 0; i < 12; ++i)
        {
            //Use edge mask to check if it is crossed
            if (!(edgeMask & (1 << i)))
                continue;

            ++edgeCount; //If it did, increment number of edge crossings

            if (SurfaceNets::BLOCK)
                continue;

            //Now find the point of intersection
            std::uint32_t e0 = cubeEdges[(i << 1) + 0]; //Unpack vertices
            std::uint32_t e1 = cubeEdges[(i << 1) + 1];
            float g0 = grid[e0]; //Unpack grid values
            float t = g0 - grid[e1]; //Compute point of intersection

            if (fabs(t) < 1e-7)
                continue;

            t = g0 / t;

            //Interpolate vertices and add up intersections (this can be done without multiplying)
            for (auto j = 0, k = 1; j < 3; ++j, k <<= 1)
            {
                auto a = e0 & k;

                if (a != (e1 & k))
                    vTemp[j] += a ? 1.0 - t : t;
                else
                    vTemp[j] += a ? 1.0 : 0.0;
            }
        }

        //Now we just average the edge intersections and add them to coordinate
        float s = 1.0f / edgeCount;
        for (auto l = 0; l < 3; ++l)
            vTemp[l] = x[l] + s * vTemp[l];

        Utils::PushToVector(vertices, vTemp[0], vTemp[1], vTemp[2]);//vertices.push(vTemp[0], vTemp[1], vTemp[2]);
    }

    void SurfaceNets::createFace(std::uint32_t edgeMask, std::uint32_t  mask, Uint32Array& buffer,
        Uint32Array& R, std::uint32_t m, Uint32Array& x, Uint32Array& faces)
    {
        //Now we need to add faces together, to do this we just loop over 3 basis components
        for (std::uint32_t i = 0; i < 3; ++i)
        {
            //The first three entries of the edgeMask count the crossings along the edge
            if (!(edgeMask & (1 << i)))
                continue;

            // i = axes we are point along.  iu, iv = orthogonal axes
            std::uint32_t  iu = (i + 1) % 3;
            std::uint32_t  iv = (i + 2) % 3;

            //If we are on a boundary, skip it
            if (x[iu] == 0 || x[iv] == 0)
                continue;

            //Otherwise, look up adjacent edges in buffer
            std::uint32_t du = R[iu];
            std::uint32_t dv = R[iv];

            //Remember to flip orientation depending on the sign of the corner.
            if (mask & 1)
            {
                Utils::PushToVector(faces, buffer[m], buffer[m - du], buffer[m - du - dv], buffer[m - dv]);
                //faces.push(buffer[m], buffer[m - du], buffer[m - du - dv], buffer[m - dv]);
            }
            else
            {
                Utils::PushToVector(faces, buffer[m], buffer[m - dv], buffer[m - du - dv], buffer[m - du]);
                //faces.push(buffer[m], buffer[m - dv], buffer[m - du - dv], buffer[m - du]);
            }
        }
    }

    Voxels SurfaceNets::computeSurface(Voxels voxels)
    {
        auto dims = voxels.dims;

        Float32Array vertices;// = [];
        Float32Array cols;// = [];
        Float32Array mats;// = [];
        Uint32Array faces;// = [];
        auto n = 0;
        Int32Array x(3, 0);// = new Int32Array(3);
        Int32Array R{ 1, (dims[0] + 1), (dims[0] + 1) * (dims[1] + 1) };// auto R = new Int32Array([1, (dims[0] + 1), (dims[0] + 1) * (dims[1] + 1)]);
        Float32Array grid(8, 0.0f);//auto grid = new Float32Array(8);
        auto nbBuf = 1;
        auto buffer = new Int32Array(R[2] * 2);

        //March over the voxel grid
        for (x[2] = 0; x[2] < dims[2] - 1; ++x[2], n += dims[0], nbBuf ^= 1, R[2] = -R[2]) {

            //m is the pointer into the buffer we are going to use.  
            //This is slightly obtuse because javascript does not have good support for packed data structures, so we must use typed arrays :(
            //The contents of the buffer will be the indices of the vertices on the previous x/y slice of the volume
            auto m = 1 + (dims[0] + 1) * (1 + nbBuf * (dims[1] + 1));

            for (x[1] = 0; x[1] < dims[1] - 1; ++x[1], ++n, m += 2) {
                for (x[0] = 0; x[0] < dims[0] - 1; ++x[0], ++n, ++m) {

                    auto mask = readScalarValues(voxels, grid, dims, n, cols, mats);
                    //Check for early termination if cell does not intersect boundary
                    if (mask == 0 || mask == 0xff)
                        continue;
                    //Sum up edge intersections
                    auto edgeMask = edgeTable[mask];
                    buffer[m] = vertices.length / 3;
                    interpolateVertices(edgeMask, cubeEdges, grid, x, vertices);
                    createFace(edgeMask, mask, buffer, R, m, x, faces);
                }
            }
        }

        //All done!  Return the result
        return {
          colors: new Float32Array(cols),
          materials : new Float32Array(mats),
          vertices : new Float32Array(vertices),
          faces : new Uint32Array(faces)
        };
    }

}