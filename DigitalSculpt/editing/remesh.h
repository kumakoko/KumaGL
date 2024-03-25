#ifndef digital_sculpt_remesh_h__
#define digital_sculpt_remesh_h__

#include <vector>
#include <cstdint>
#include <limits>

#include "editing/voxels.h"
#include "mesh/mesh_atrribute.h"
#include "mesh/mesh.h"
#include "misc/enums.h"
#include "misc/utils.h"

namespace DigitalSculpt
{
    class Remesh
    {
    public:
        static std::uint32_t RESOLUTION;
        static bool BLOCK;
        static bool SMOOTHING;

        static void floodFill(Voxels voxels);

        static void voxelize(MeshSPtr mesh, Voxels voxels);

        // grid structure
        static Voxels createVoxelData(const SixElemArray& box);

        static MeshSPtr createMesh(MeshSPtr mesh, const Uint32Array& faces, const Float32Array& vertices, const Float32Array& colors, const Float32Array& materials);

        // hole filling + transform to world + ComputeBox
        static SixElemArray prepareMeshes(std::vector<MeshSPtr>& meshes);

        static void alignMeshBound(MeshSPtr mesh, const SixElemArray& box);

        static void tangentialSmoothing(MeshSPtr mesh);

        static MeshSPtr remesh(std::vector<MeshSPtr>& meshes, MeshSPtr baseMesh, bool manifold);

        static MeshAttribute& mergeArrays(std::vector<MeshSPtr>& meshes, MeshAttribute& arr);

        static MeshSPtr mergeMeshes(std::vector<MeshSPtr>& meshes, MeshSPtr baseMesh);

    };
}
#endif // digital_sculpt_remesh_h__