#ifndef digital_sculpt_reversion_h__
#define digital_sculpt_reversion_h__

#include "mesh/mesh.h"
#include "misc/utils.h"
#include "misc/array_nd.h"

namespace DigitalSculpt
{
    class Reversion
    {
    public:

        static Int8Array detectExtraordinaryVertices(MeshSPtr mesh);

        /** Return the first extraordinary vertex if it exists... or a random vertex otherwise */
        static std::uint32_t getSeed(MeshSPtr mesh, const Int8Array& vEvenTags, Int8Array& vExtraTags);

        /** Tag the even vertices */
        static bool tagVertices(MeshSPtr mesh, Int8Array& vExtraTags, Int8Array& vEvenTags);

        /** Tag the even vertices */
        static bool tagEvenVertices(MeshSPtr mesh,  Int8Array& vEvenTags);

        /** Creates the coarse faces from the tagged vertices */
        static bool createFaces(MeshSPtr baseMesh, MeshSPtr newMesh, const Int8Array& vEvenTags, const Int32Array& triFaceOrQuadCenter);

        /** Creates the vertices of the mesh */
        static void createVertices(MeshSPtr baseMesh, MeshSPtr newMesh, const Int32Array& triFaceOrQuadCenter);

        /** Copy the vertices data from up to low */
        static void copyVerticesData(MeshSPtr baseMesh, MeshSPtr newMesh);

        /** Computes uv faces and uv coordinates for center vertices */
        static void computeFaceTexCoords(MeshSPtr baseMesh, MeshSPtr newMesh, const Int32Array& triFaceOrQuadCenter, const Uint32Array& uvMap);

        /** Apply the reverse of a subdivision for the texCoord mesh */
        static void computeTexCoords(MeshSPtr baseMesh, MeshSPtr newMesh, const Int32Array& triFaceOrQuadCenter);

        /** Apply the reverse of a subdivision */
        static bool computeReverse(MeshSPtr baseMesh, MeshSPtr newMesh);

    };
}

#endif // digital_sculpt_reversion_h__