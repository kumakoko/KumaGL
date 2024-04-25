#ifndef digital_sculpt_mesh_data_h__
#define digital_sculpt_mesh_data_h__

#include <cstdint>
#include <memory>
#include <array>
#include "misc/utils.h"
#include "misc/array_nd.h"
#include "math3d/octree_cell.h"

namespace DigitalSculpt
{
    class MeshData
    {
    public:
        std::uint32_t _nbVertices;// : 0,
        std::uint32_t _nbFaces;// : 0,
        std::uint32_t _nbTexCoords;// : 0,

        /////////////////////
        // unique vertex data
        /////////////////////

        Float32Array _verticesXYZ;// : null, // vertices (Float32Array)
        Float32Array _normalsXYZ;// : null, // normals (Float32Array)
        Float32Array _materialsPBR;// : null, // pbr vertex data (Float32
        Float32Array _colorsRGB;// : null, // color vertices (Float32Array)Array) roughness/metallic/masking

        Uint8Array _vertOnEdge; //: null, // (1 :> on edge, 0 otherwise) (Uint8ClampedArray)
        IUint32ArrayND* _vertRingFace;// : null, // array of neighborhood id faces (Uint32Array)
        Uint32Array _vrfStartCount;// : null, // reference vertRingFace start and count ring (start/count) (Uint32Array)
        Uint32Array _vrvStartCount;// : null, // array of neighborhood id vertices (start/count) (Uint32Array)
        IUint32ArrayND* _vertRingVert;// : null, // reference vertRingVert start and count ring (Uint32Array)

        Int32Array _vertTagFlags; //: null, // general purposes flag, (<: Utils.TAG_FLAG) (Int32Array)
        Int32Array _vertSculptFlags; // : null, // editing flag (tag vertices when starting sculpting session) (<: Utils.SCULPT_FLAG) (Int32Array),
        Int32Array _vertStateFlags; // : null, // state flag (tag vertices to handle undo/redo) (<: Utils.STATE_FLAG) (Int32Array)

        Float32Array _vertProxy;// : : null, // vertex proxy, for sculpting limits (Float32Array)

        ///////////////////
        // unique face data
        ///////////////////

        Uint32Array _facesABCD;//: null, // faces tri or quad, tri will have D:Utils.TRI_INDEX (Uint32Array)

        Uint32Array _faceEdges;// : null, // each face references the id edges (Uint32Array)
        Float32Array   _faceNormalsXYZ;// : null, // faces normals (Float32Array)

        Uint32Array _facesToTriangles;// : null, // faces to triangles (Uint32Array)
        Uint32Array   _trianglesABC;// : null, // triangles (Uint32Array)

        Int32Array _facesTagFlags;// : null, // triangles tag (<: Utils.TAG_FLAG) (Int32Array)

        ////////////
        // edge data
        ////////////
        Uint8Array _edges;//: null, // edges (Uint8Array) (1 :> outer edge, 0 or 2 :> inner edge, >:3 non manifold)

        /////////////////
        // wireframe data
        /////////////////

        Uint32Array     _drawArraysWireframe;//: null, // array for the wireframe (base on drawArrays vertices)
        Uint32Array    _drawElementsWireframe;// : null, // array for the wireframe (base on drawElements vertices)

        //////////
        // UV data
        //////////

        Float32Array _texCoordsST;//: null, // tex coords (Float32Array)
        Uint32Array     _duplicateStartCount;// : null, // array of vertex duplicates location (start/count) (Uint32Array)
        Uint32Array    _UVfacesABCD;// : null, // faces unwrap (Uint32Array)
        Uint32Array     _UVtrianglesABC;// : null, // triangles tex coords (Uint32Array)

        //////////////////
        // DrawArrays data
        //////////////////

        Float32Array _DAverticesXYZ;//: null, // vertices (Float32Array)
        Float32Array _DAnormalsXYZ;// : null, // normals (Float32Array)
        Float32Array _DAcolorsRGB;// : null, // color vertices (Float32Array)
        Float32Array _DAmaterialsPBR;// : null, // material vertices (Float32Array)
        Float32Array _DAtexCoordsST;// : null, // texCoords (Float32Array)

        //////////////
        // Octree data
        //////////////

        OctreeCell* _octree;//: null, // root octree cell

        Float32Array    _faceBoxes;// : null, // faces bbox (Float32Array)
        Float32Array    _faceCentersXYZ;// : null, // faces center (Float32Array)

        Uint32Array   _facePosInLeaf;// : null, // position index in the leaf (Uint32Array)
        std::vector<OctreeCell*> _faceLeaf;//: [] , // octree leaf
        std::vector<OctreeCell*>  _leavesToUpdate;// : [] , // leaves of the octree to check

        std::array<float,6> _worldBound;// [Infinity, Infinity, Infinity, -Infinity, -Infinity, -Infinity] ,
    };

    typedef std::shared_ptr<MeshData> MeshDataSPtr;
}

#endif // digital_sculpt_mesh_data_h__
