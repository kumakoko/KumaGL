#ifndef digital_sculpt_hole_filling_h__
#define digital_sculpt_hole_filling_h__

#include <vector>
#include <memory>
#include <cmath>
#include "glm/vec3.hpp"

#include "misc/utils.h"
#include "mesh/mesh_static/mesh_static.h"
#include "mesh/mesh.h"

namespace DigitalSculpt
{
    struct Edge
    {
    public:
        std::shared_ptr<Edge> previous;
        std::shared_ptr<Edge> next;
        std::uint32_t v1;
        std::uint32_t v2;
        float angle;

        Edge(std::uint32_t v1, std::uint32_t v2);
    };

    typedef std::shared_ptr<Edge> EdgeSPtr;

    class HoleFilling
    {
    public:
        static EdgeSPtr detectHole(std::vector<EdgeSPtr>& borderEdges);

        static std::vector<EdgeSPtr> detectHoles(MeshSPtr mesh);

        static void advancingFrontMesh(MeshSPtr mesh, EdgeSPtr firstEdge, Uint32Array& newTris, Float32Array& newVerts, Float32Array& newColors, Float32Array& newMaterials);

        static MeshSPtr createMesh(MeshSPtr mesh, const Float32Array& vertices, const Float32Array& faces, const Float32Array& colors, const Float32Array& materials);

        static MeshSPtr closeHoles(MeshSPtr mesh);

        static MeshSPtr createClosedMesh(MeshSPtr mesh);
    };
}

#endif // digital_sculpt_hole_filling_h__
