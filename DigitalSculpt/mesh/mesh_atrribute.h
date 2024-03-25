#ifndef digital_sculpt_mesh_atrribute_h__
#define digital_sculpt_mesh_atrribute_h__

#include <cstdint>
#include "misc/utils.h"

namespace DigitalSculpt
{
    class MeshAttribute
    {
    public:
        Float32Array vertices;//var vAr = arr.vertices = arr.vertices != = undefined ? new Float32Array(nbVertices * 3) : null;
        Float32Array colors;//var cAr = arr.colors = arr.colors != = undefined ? new Float32Array(nbVertices * 3) : null;
        Float32Array materials;//var mAr = arr.materials = arr.materials != = undefined ? new Float32Array(nbVertices * 3) : null;
        Uint32Array faces;//var fAr = arr.faces = arr.faces != = undefined ? new Uint32Array(nbFaces * 4) : null;
        Uint32Array triangles;//var iAr = arr.triangles = arr.triangles != = undefined ? new Uint32Array(nbTriangles * 3) : null;

        std::uint32_t nbVertices;
        std::uint32_t nbFaces;
        std::uint32_t nbQuads;
        std::uint32_t nbTriangles;
    };
}
#endif // digital_sculpt_mesh_atrribute_h__
