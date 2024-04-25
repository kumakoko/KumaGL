#include <cstdint>
#include "misc/utils.h"
#include "mesh/mesh.h"

auto Subdivision = {};
Subdivision::LINEAR = false;

//       v3
//       /\
//      /3T\ 
//   m3/____\m2
//    /\ 0T /\
//   /1T\  /2T\
//  /____\/____\ 
// v1    m1    v2

// v4____m3____v3
// |     |     |
// |     |     |
// |m4___|c____|m2
// |     |     |
// |     |     |
// |_____|_____|
// v1   m1     v2


namespace DigitalSculpt
{
    // Helper class
    class OddVertexComputer
    {
    private:
        Float32Array* _vArOut;// = vArOut;
        Float32Array* _cArOut;// = cArOut;
        Float32Array* _mArOut;// = mArOut;
        //Float32Array _vAr;// = mesh.getVertices();
        //Float32Array _cAr;// = mesh.getColors();
        //Float32Array _mAr;// = mesh.getMaterials();
        //_eAr = mesh.getEdges();
        std::uint32_t _nbVertices;// = mesh.getNbVertices();
        Int32Array _tagEdges;// = new Int32Array(mesh.getNbEdges());
        MeshSPtr _mesh;
    public:
        inline Int32Array& getTagEdges()
        {
            return _tagEdges;
        }

        inline const Int32Array& getTagEdges() const
        {
            return _tagEdges;
        }

        ~OddVertexComputer();

        OddVertexComputer(MeshSPtr mesh, Float32Array* vArOut, Float32Array* cArOut, Float32Array* mArOut);

        std::int32_t computeTriangleEdgeVertex(std::uint32_t iv1, std::uint32_t iv2, std::uint32_t iv3, std::uint32_t  ide);

        std::uint32_t computeQuadEdgeVertex(std::uint32_t iv1, std::uint32_t iv2, std::uint32_t iv3, std::uint32_t  iv4, std::uint32_t ide);

        std::uint32_t computeFaceVertex(std::uint32_t iv1, std::uint32_t  iv2, std::uint32_t  iv3, std::uint32_t  iv4);
    };

    class Subdivision
    {
    public:
        static bool LINEAR;
    public:
        /** Even vertices smoothing */
        static void applyEvenSmooth(MeshSPtr baseMesh, Float32Array& even, Float32Array& colorOut, Float32Array& materialOut);

        /** Odd vertices smoothing */
        static Int32Array applyOddSmooth(MeshSPtr mesh, Float32Array* odds, Float32Array* colorOut, Float32Array* materialOut, Uint32Array* fArOut);

        /** Computes uv faces and uv coordinates for center vertices */
        static void computeFaceTexCoords(MeshSPtr mesh, MeshSPtr newMesh, Uint32Array& tagEdges);

        /** Subdivide tex coords mesh */
        static void computeTexCoords(MeshSPtr mesh, MeshSPtr newMesh, Int32Array& tagEdges);

        /** Apply a complete subdivision (by updating the topology) */
        static void fullSubdivision(MeshSPtr baseMesh, MeshSPtr newMesh);

        /** Apply subdivision without topology computation */
        static void partialSubdivision(MeshSPtr baseMesh, Float32Array& vertOut, Float32Array& colorOut, Float32Array& materialOut);

    };
}