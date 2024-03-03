#ifndef mesh_h__
#define mesh_h__

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "mesh_data.h"
#include "transform_data.h"

/*
import { vec3, mat3, mat4 } from 'gl-matrix';
import Enums from 'misc/Enums';
import Utils from 'misc/Utils';
import OctreeCell from 'math3d/OctreeCell';
import Shader from 'render/ShaderLib';
import RenderData from 'mesh/RenderData';
*/

/*
Basic usage:
var mesh = new MeshStatic(gl); // provide gl only if we need to render the mesh
mesh.setVertices(vertices); // vec3 xyz
mesh.setFaces(faces); // ivec4 abcd (d=Utils.TRI_INDEX if tri)

// these are optional
mesh.setColors(colors); // vec3 rgb
mesh.setMaterials(materials); // vec3 roughness/metalness/masking
mesh.initTexCoordsDataFromOBJData(UV, facesUV); // vec2, ivec4

mesh.init(); // compute octree/topo/UV, etc...
mesh.initRender(); // only if gl has been provided
*/
namespace DigitalSculpt
{
    extern float DEF_ROUGHNESS = 0.18; // 0.18;
    extern float DEF_METALNESS = 0.08; // 0.08;

    class Mesh
    {
    protected:
        std::int32_t _id;// = Mesh.ID++; // useful id to retrieve a mesh (dynamic mesh, multires mesh, voxel mesh)
        MeshData* _meshData = null;
        TransformData* _transformData = null;
        RenderData* _renderData = null;
        bool _isVisible = true;

    public:
        Mesh()
        {
            _id = Mesh.ID++; // useful id to retrieve a mesh (dynamic mesh, multires mesh, voxel mesh)
            _meshData = null;
            _transformData = null;
            _renderData = null;
            _isVisible = true;
        }

        static float sortFunction(const Mesh& meshA, const Mesh& meshB)
        {
            // render transparent (back to front) after opaque (front to back) ones
            bool aTr = meshA.isTransparent();
            bool bTr = meshB.isTransparent();
            if (aTr && !bTr) return 1.0f;
            if (!aTr && bTr) return -1.0f;
            return (meshB.getDepth() - meshA.getDepth()) * (aTr && bTr ? 1.0f : -1.0f);
        }

        void setID(std::int32_t id)
        {
            _id = id;
        }

        inline bool isVisible()
        {
            return _isVisible;
        }

        inline void setVisible(bool b)
        {
            _isVisible = b;
        }

        void setVertices(const Float32Array& vAr);

        void setFaces(const Uint32Array& fAr);

        void setTexCoords(const Float32Array& tAr);

        void setColors(const Float32Array& cAr);

        void setMaterials(const Float32Array& mAr);

        void setVerticesDuplicateStartCount(const Uint32Array& startCount);

        void setFacesTexCoord(const Uint32Array& fuAr);

        inline void setMeshData(MeshDataSPtr mdata)
        {
            _meshData = mdata;
        }

        inline void setRenderData(RenderDataSPtr rdata)
        {
            _renderData = rdata;
        }

        inline void setTransformData(TransformDataSPtr tdata)
        {
            _transformData = tdata;
        }

        inline void setNbVertices(std::uint32_t nbVertices)
        {
            _meshData->_nbVertices = nbVertices;
        }

        inline void setNbFaces(std::uint32_t nbFaces)
        {
            _meshData->_nbFaces = nbFaces;
        }

        inline std::int32_t getID() const
        {
            return _id;
        }

        inline RenderDataSPtr getRenderData()
        {
            return _renderData;
        }

        inline MeshDataSPtr getMeshData()
        {
            return _meshData;
        }

        inline TransformDataSPtr getTransformData()
        {
            return _transformData;
        }

        inline std::uint32_t getNbVertices() const
        {
            return _meshData->_nbVertices;
        }

        inline std::uint32_t getNbFaces() const
        {
            return _meshData->_nbFaces;
        }

        inline std::uint32_t getNbQuads() const
        {
            return getNbTriangles() - getNbFaces();
        }

        inline std::uint32_t getNbTriangles() const
        {
            return _meshData->_trianglesABC.size() / 3;
        }

        inline std::uint32_t getNbTexCoords()  const
        {
            return _meshData->_nbTexCoords;
        }

        inline bool hasUV()
        {
            return _meshData->_texCoordsST.size() > 0;
        }

        inline Float32Array& getVertices()
        {
            return _meshData->_verticesXYZ;
        }

        inline const Float32Array& getVertices() const
        {
            return _meshData->_verticesXYZ;
        }

        inline Float32Array& getColors()
        {
            return _meshData->_colorsRGB;
        }

        inline const Float32Array& getColors() const
        {
            return _meshData->_colorsRGB;
        }

        inline Float32Array& getNormals()
        {
            return _meshData->_normalsXYZ;
        }

        inline const Float32Array& getNormals() const
        {
            return _meshData->_normalsXYZ;
        }

        inline Float32Array& getMaterials()
        {
            return _meshData->_materialsPBR;
        }

        inline const Float32Array& getMaterials() const
        {
            return _meshData->_materialsPBR;
        }

        inline Int32Array& getVerticesTagFlags()
        {
            return _meshData->_vertTagFlags;
        }

        inline const Int32Array& getVerticesTagFlags() const
        {
            return _meshData->_vertTagFlags;
        }

        inline Int32Array& getVerticesSculptFlags()
        {
            return _meshData->_vertSculptFlags;
        }

        inline const Int32Array& getVerticesSculptFlags() const
        {
            return _meshData->_vertSculptFlags;
        }

        inline Int32Array& getVerticesStateFlags()
        {
            return _meshData->_vertStateFlags;
        }

        inline const Int32Array& getVerticesStateFlags() const
        {
            return _meshData->_vertStateFlags;
        }

        inline Uint32Array& getVerticesRingVertStartCount()
        {
            return _meshData->_vrvStartCount;
        }

        inline const Uint32Array& getVerticesRingVertStartCount() const
        {
            return _meshData->_vrvStartCount;
        }

        inline Uint32Array& getVerticesRingVert()
        {
            return _meshData->_vertRingVert;
        }

        inline const Uint32Array& getVerticesRingVert() const
        {
            return _meshData->_vertRingVert;
        }

        inline Uint32Array& getVerticesRingFaceStartCount()
        {
            return _meshData->_vrfStartCount;
        }

        inline const Uint32Array& getVerticesRingFaceStartCount() const
        {
            return _meshData->_vrfStartCount;
        }

        inline Uint32Array& getVerticesRingFace()
        {
            return _meshData->_vertRingFace;
        }

        inline const Uint32Array& getVerticesRingFace() const
        {
            return _meshData->_vertRingFace;
        }

        inline Uint8Array& getVerticesOnEdge()
        {
            return _meshData->_vertOnEdge;
        }

        inline const Uint8Array& getVerticesOnEdge() const
        {
            return _meshData->_vertOnEdge;
        }

        inline Float32Array& getVerticesProxy()
        {
            return _meshData->_vertProxy;
        }

        inline const Float32Array& getVerticesProxy() const
        {
            return _meshData->_vertProxy;
        }

        inline Uint32Array& getFaces()
        {
            return _meshData->_facesABCD;
        }

        inline const Uint32Array& getFaces() const
        {
            return _meshData->_facesABCD;
        }

        bool hasOnlyTriangles() const;

        bool hasOnlyQuads() const;

        inline Float32Array& getFaceNormals()
        {
            return _meshData->_faceNormalsXYZ;
        }

        inline const Float32Array& getFaceNormals() const
        {
            return _meshData->_faceNormalsXYZ;
        }

        inline Float32Array& getFaceBoxes()
        {
            return _meshData->_faceBoxes;
        }

        inline const Float32Array& getFaceBoxes() const
        {
            return _meshData->_faceBoxes;
        }

        inline Float32Array& getFaceCenters() {
            return _meshData->_faceCentersXYZ;
        }

        inline const Float32Array& getFaceCenters() const
        {
            return _meshData->_faceCentersXYZ;
        }

        inline Int32Array& getFacesTagFlags()
        {
            return _meshData->_facesTagFlags;
        }

        inline const Int32Array& getFacesTagFlags() const
        {
            return _meshData->_facesTagFlags;
        }

        inline Uint32Array& getFaceEdges()
        {
            return _meshData->_faceEdges;
        }

        inline const Uint32Array& getFaceEdges() const
        {
            return _meshData->_faceEdges;
        }

        inline Uint32Array& getFacesToTriangles() {
            return _meshData->_facesToTriangles;
        }

        inline const Uint32Array& getFacesToTriangles() const {
            return _meshData->_facesToTriangles;
        }

        inline Uint32Array& getTrianglesTexCoord() {
            return _meshData->_UVtrianglesABC;
        }

        inline const Uint32Array& getTrianglesTexCoord() const {
            return _meshData->_UVtrianglesABC;
        }

        inline Uint32Array& getTriangles() {
            return _meshData->_trianglesABC;
        }

        inline const Uint32Array& getTriangles() const {
            return _meshData->_trianglesABC;
        }

        inline Uint8Array& getEdges() {
            return _meshData->_edges;
        }

        inline const Uint8Array& getEdges() const {
            return _meshData->_edges;
        }

        inline std::uint32_t getNbEdges() const
        {
            return _meshData->_edges.size();
        }

        inline Float32Array& getTexCoords() {
            return _meshData->_texCoordsST;
        }

        inline const Float32Array& getTexCoords() const {
            return _meshData->_texCoordsST;
        }

        inline Uint32Array& getVerticesDuplicateStartCount() {
            return _meshData->_duplicateStartCount;
        }

        inline const Uint32Array& getVerticesDuplicateStartCount() const {
            return _meshData->_duplicateStartCount;
        }

        inline Uint32Array& getFacesTexCoord() {
            return _meshData->_UVfacesABCD;
        }

        inline const Uint32Array& getFacesTexCoord() const {
            return _meshData->_UVfacesABCD;
        }

        Float32Array& Mesh::getVerticesDrawArrays();

        inline const Float32Array& getNormalsDrawArrays() const {
            return _meshData->_DAnormalsXYZ;
        }

        inline Float32Array& getNormalsDrawArrays() {
            return _meshData->_DAnormalsXYZ;
        }

        inline Float32Array& getColorsDrawArrays() {
            return _meshData->_DAcolorsRGB;
        }

        inline const Float32Array& getColorsDrawArrays() const{
            return _meshData->_DAcolorsRGB;
        }

        inline Float32Array& getMaterialsDrawArrays() {
            return _meshData->_DAmaterialsPBR;
        }

        inline const Float32Array& getMaterialsDrawArrays() const{
            return _meshData->_DAmaterialsPBR;
        }

        inline Float32Array& getTexCoordsDrawArrays() {
            return _meshData->_DAtexCoordsST;
        }

        inline const Float32Array& getTexCoordsDrawArrays() const {
            return _meshData->_DAtexCoordsST;
        }

        inline Octree* getOctree()
        {
            return _meshData->_octree;
        }

        inline const Octree* getOctree() const
        {
            return _meshData->_octree;
        }

        inline glm::vec3 getCenter() {
            return _transformData->_center;
        }

        inline glm::mat4& getMV() {
            return _transformData->_lastComputedMV;
        }

        inline const glm::mat4& getMV() const{
            return _transformData->_lastComputedMV;
        }

        inline glm::mat4& getMVP() {
            return _transformData->_lastComputedMVP;
        }

        inline glm::mat3& getN() {
            return _transformData->_lastComputedN;
        }

        inline const glm::mat3& getN() const{
            return _transformData->_lastComputedN;
        }

        inline glm::mat3& getEN() {
            return _transformData->_lastComputedEN;
        }

        inline const glm::mat3& getEN() const{
            return _transformData->_lastComputedEN;
        }

        inline float getDepth() const{
            return _transformData->_lastComputedDepth;
        }

        inline glm::mat4& getMatrix()
        {
            return _transformData->_matrix;
        }

        inline const glm::mat4& getMatrix() const
        {
            return _transformData->_matrix;
        }

        inline glm::mat4& getEditMatrix() {
            return _transformData->_editMatrix;
        }

        inline const glm::mat4& getEditMatrix() const {
            return _transformData->_editMatrix;
        }

        inline glm::mat4& getEditMatrix() {
            return _transformData->_editMatrix;
        }

        inline const glm::mat4& getEditMatrix() const{
            return _transformData->_editMatrix;
        }

        float getScale2() const;

        float getScale() const;

        glm::vec3 getSymmetryOrigin() const;

        float getSymmetryOffset() const
        {
            return _transformData->_symmetryOffset;
        }

        inline void setSymmetryOffset(float offset)
        {
            _transformData->_symmetryOffset = offset;
        }

        inline glm::vec3& getSymmetryNormal() 
        {
            return _transformData->_symmetryNormal;
        }

        inline const glm::vec3& getSymmetryNormal() const
        {
            return _transformData->_symmetryNormal;
        }

        inline Uint32Array& getFacePosInLeaf() 
        {
            return _meshData->_facePosInLeaf;
        }

        inline const Uint32Array& getFacePosInLeaf() const {
            return _meshData->_facePosInLeaf;
        }

        inline Float32Array& getFaceLeaf() {
            return _meshData->_faceLeaf;
        }

        inline const Float32Array& getFaceLeaf() const {
            return _meshData->_faceLeaf;
        }

        inline Float32Array& getLeavesToUpdate() {
            return _meshData->_leavesToUpdate;
        }

        inline const Float32Array& getLeavesToUpdate() const {
            return _meshData->_leavesToUpdate;
        }

        getLocalBound() {
            return _meshData->_octree._aabbLoose;
        }

        inline uint32_t getRenderNbEdges() const
        {
            return getNbEdges();
        }

        void init();

        void initTopology();

        void updateGeometry(iFaces, iVerts);

        void allocateArrays();

        /** Init color and material array */
        void initColorsAndMaterials();

        /** Computes faces ring around vertices */
        void initFaceRings();

        /** Update a group of vertices' normal */
        updateVerticesNormal(iVerts) {
            var vrfStartCount = getVerticesRingFaceStartCount();
            var vertRingFace = getVerticesRingFace();
            var ringFaces = vertRingFace instanceof Array ? vertRingFace : null;
            var nAr = getNormals();
            var faceNormals = getFaceNormals();

            var full = iVerts == = undefined;
            var nbVerts = full ? getNbVertices() : iVerts.length;
            for (var i = 0; i < nbVerts; ++i) {
                var ind = full ? i : iVerts[i];
                var start, end;
                if (ringFaces) {
                    vertRingFace = ringFaces[ind];
                    start = 0;
                    end = vertRingFace.length;
                }
                else {
                    start = vrfStartCount[ind * 2];
                    end = start + vrfStartCount[ind * 2 + 1];
                }
                var nx = 0.0;
                var ny = 0.0;
                var nz = 0.0;
                for (var j = start; j < end; ++j) {
                    var id = vertRingFace[j] * 3;
                    nx += faceNormals[id];
                    ny += faceNormals[id + 1];
                    nz += faceNormals[id + 2];
                }
                var len = end - start;
                if (len != = 0.0) len = 1.0 / len;
                ind *= 3;
                nAr[ind] = nx * len;
                nAr[ind + 1] = ny * len;
                nAr[ind + 2] = nz * len;
            }
        }

        /** Computes vertex ring around vertices */
        initVertexRings() {
            var vrvStartCount = getVerticesRingVertStartCount();
            var vertRingVert = _meshData->_vertRingVert = new Uint32Array(getNbEdges() * 2);
            var vrfStartCount = getVerticesRingFaceStartCount();
            var vertRingFace = getVerticesRingFace();
            var vertTagFlags = getVerticesTagFlags();
            var vertOnEdge = getVerticesOnEdge();
            var fAr = getFaces();
            var vrvStart = 0;

            for (var i = 0, l = getNbVertices(); i < l; ++i) {
                var tagFlag = ++Utils.TAG_FLAG;
                var vrfStart = vrfStartCount[i * 2];
                var vrfEnd = vrfStart + vrfStartCount[i * 2 + 1];
                var vrvCount = 0;

                for (var j = vrfStart; j < vrfEnd; ++j) {
                    var ind = vertRingFace[j] * 4;
                    var iVer1 = fAr[ind];
                    var iVer2 = fAr[ind + 1];
                    var iVer3 = fAr[ind + 2];
                    var iVer4 = fAr[ind + 3];

                    if (iVer1 == = i) iVer1 = iVer4 != = Utils.TRI_INDEX ? iVer4 : iVer3;
                    else if (iVer2 == = i || iVer4 == = i) iVer2 = iVer3;
                    else if (iVer3 == = i && iVer4 != = Utils.TRI_INDEX) iVer1 = iVer4;

                    if (vertTagFlags[iVer1] != = tagFlag) {
                        vertRingVert[vrvStart + (vrvCount++)] = iVer1;
                        vertTagFlags[iVer1] = tagFlag;
                    }

                    if (vertTagFlags[iVer2] != = tagFlag) {
                        vertRingVert[vrvStart + (vrvCount++)] = iVer2;
                        vertTagFlags[iVer2] = tagFlag;
                    }
                }

                vrvStartCount[i * 2] = vrvStart;
                vrvStartCount[i * 2 + 1] = vrvCount;
                vrvStart += vrvCount;
                if ((vrfEnd - vrfStart) != = vrvCount)
                    vertOnEdge[i] = 1;
            }
        }

        /** Get more vertices (n-ring) */
        expandsVertices(iVerts, nRing) {
            var tagFlag = ++Utils.TAG_FLAG;
            var nbVerts = iVerts.length;
            var vrvStartCount = getVerticesRingVertStartCount();
            var vertRingVert = getVerticesRingVert();
            var ringVerts = vertRingVert instanceof Array ? vertRingVert : null;
            var vertTagFlags = getVerticesTagFlags();
            var acc = nbVerts;
            var nbVertices = getNbVertices();
            var iVertsExpanded = new Uint32Array(Utils.getMemory(4 * nbVertices), 0, nbVertices);
            iVertsExpanded.set(iVerts);

            var i = 0;
            for (i = 0; i < nbVerts; ++i)
                vertTagFlags[iVertsExpanded[i]] = tagFlag;

            var iBegin = 0;
            while (nRing) {
                --nRing;
                for (i = iBegin; i < nbVerts; ++i) {
                    var idVert = iVertsExpanded[i];
                    var start, end;
                    if (ringVerts) {
                        vertRingVert = ringVerts[idVert];
                        start = 0;
                        end = vertRingVert.length;
                    }
                    else {
                        start = vrvStartCount[idVert * 2];
                        end = start + vrvStartCount[idVert * 2 + 1];
                    }

                    for (var j = start; j < end; ++j) {
                        var id = vertRingVert[j];
                        if (vertTagFlags[id] == = tagFlag)
                            continue;

                        vertTagFlags[id] = tagFlag;
                        iVertsExpanded[acc++] = id;
                    }
                }
                iBegin = nbVerts;
                nbVerts = acc;
            }

            return new Uint32Array(iVertsExpanded.subarray(0, acc));
        }

        /** Return all the vertices linked to a group of faces */
        getVerticesFromFaces(iFaces) {
            var tagFlag = ++Utils.TAG_FLAG;
            var nbFaces = iFaces.length;
            var vertTagFlags = getVerticesTagFlags();
            var fAr = getFaces();
            var acc = 0;
            var verts = new Uint32Array(Utils.getMemory(4 * iFaces.length * 4), 0, iFaces.length * 4);

            for (var i = 0; i < nbFaces; ++i) {
                var ind = iFaces[i] * 4;
                var iVer1 = fAr[ind];
                var iVer2 = fAr[ind + 1];
                var iVer3 = fAr[ind + 2];
                var iVer4 = fAr[ind + 3];
                if (vertTagFlags[iVer1] != = tagFlag) {
                    vertTagFlags[iVer1] = tagFlag;
                    verts[acc++] = iVer1;
                }
                if (vertTagFlags[iVer2] != = tagFlag) {
                    vertTagFlags[iVer2] = tagFlag;
                    verts[acc++] = iVer2;
                }
                if (vertTagFlags[iVer3] != = tagFlag) {
                    vertTagFlags[iVer3] = tagFlag;
                    verts[acc++] = iVer3;
                }

                if (iVer4 != = Utils.TRI_INDEX && vertTagFlags[iVer4] != = tagFlag) {
                    vertTagFlags[iVer4] = tagFlag;
                    verts[acc++] = iVer4;
                }
            }
            return new Uint32Array(verts.subarray(0, acc));
        }

        /** Update a group of faces normal and aabb */
        updateFacesAabbAndNormal(iFaces) {
            var faceNormals = getFaceNormals();
            var faceBoxes = getFaceBoxes();
            var faceCenters = getFaceCenters();
            var vAr = getVertices();
            var fAr = getFaces();

            var full = iFaces == = undefined;
            var nbFaces = full ? getNbFaces() : iFaces.length;
            for (var i = 0; i < nbFaces; ++i) {
                var ind = full ? i : iFaces[i];
                var idTri = ind * 3;
                var idFace = ind * 4;
                var idBox = ind * 6;
                var ind1 = fAr[idFace] * 3;
                var ind2 = fAr[idFace + 1] * 3;
                var ind3 = fAr[idFace + 2] * 3;
                var ind4 = fAr[idFace + 3];
                var isQuad = ind4 != = Utils.TRI_INDEX;
                if (isQuad) ind4 *= 3;

                var v1x = vAr[ind1];
                var v1y = vAr[ind1 + 1];
                var v1z = vAr[ind1 + 2];
                var v2x = vAr[ind2];
                var v2y = vAr[ind2 + 1];
                var v2z = vAr[ind2 + 2];
                var v3x = vAr[ind3];
                var v3y = vAr[ind3 + 1];
                var v3z = vAr[ind3 + 2];

                // compute normals
                var ax = v2x - v1x;
                var ay = v2y - v1y;
                var az = v2z - v1z;
                var bx = v3x - v1x;
                var by = v3y - v1y;
                var bz = v3z - v1z;
                var crx = ay * bz - az * by;
                var cry = az * bx - ax * bz;
                var crz = ax * by - ay * bx;

                // compute boxes
                // for code readability of course
                var xmin = v1x < v2x ? v1x < v3x ? v1x : v3x : v2x < v3x ? v2x : v3x;
                var xmax = v1x > v2x ? v1x > v3x ? v1x : v3x : v2x > v3x ? v2x : v3x;
                var ymin = v1y < v2y ? v1y < v3y ? v1y : v3y : v2y < v3y ? v2y : v3y;
                var ymax = v1y > v2y ? v1y > v3y ? v1y : v3y : v2y > v3y ? v2y : v3y;
                var zmin = v1z < v2z ? v1z < v3z ? v1z : v3z : v2z < v3z ? v2z : v3z;
                var zmax = v1z > v2z ? v1z > v3z ? v1z : v3z : v2z > v3z ? v2z : v3z;

                if (isQuad) {
                    var v4x = vAr[ind4];
                    var v4y = vAr[ind4 + 1];
                    var v4z = vAr[ind4 + 2];
                    if (v4x < xmin) xmin = v4x;
                    if (v4x > xmax) xmax = v4x;
                    if (v4y < ymin) ymin = v4y;
                    if (v4y > ymax) ymax = v4y;
                    if (v4z < zmin) zmin = v4z;
                    if (v4z > zmax) zmax = v4z;
                    ax = v3x - v4x;
                    ay = v3y - v4y;
                    az = v3z - v4z;
                    crx += ay * bz - az * by;
                    cry += az * bx - ax * bz;
                    crz += ax * by - ay * bx;
                }

                // normals
                faceNormals[idTri] = crx;
                faceNormals[idTri + 1] = cry;
                faceNormals[idTri + 2] = crz;
                // boxes
                faceBoxes[idBox] = xmin;
                faceBoxes[idBox + 1] = ymin;
                faceBoxes[idBox + 2] = zmin;
                faceBoxes[idBox + 3] = xmax;
                faceBoxes[idBox + 4] = ymax;
                faceBoxes[idBox + 5] = zmax;
                // compute centers
                faceCenters[idTri] = (xmin + xmax) * 0.5;
                faceCenters[idTri + 1] = (ymin + ymax) * 0.5;
                faceCenters[idTri + 2] = (zmin + zmax) * 0.5;
            }
        }

        /** Get more faces (n-ring) */
        expandsFaces(iFaces, nRing) {
            var tagFlag = ++Utils.TAG_FLAG;
            var nbFaces = iFaces.length;
            var vrfStartCount = getVerticesRingFaceStartCount();
            var vertRingFace = getVerticesRingFace();
            var ringFaces = vertRingFace instanceof Array ? vertRingFace : null;
            var ftf = getFacesTagFlags();
            var fAr = getFaces();
            var acc = nbFaces;
            var iFacesExpanded = new Uint32Array(Utils.getMemory(4 * getNbFaces()), 0, getNbFaces());
            iFacesExpanded.set(iFaces);
            var i = 0;
            for (i = 0; i < nbFaces; ++i)
                ftf[iFacesExpanded[i]] = tagFlag;
            var iBegin = 0;
            while (nRing) {
                --nRing;
                for (i = iBegin; i < nbFaces; ++i) {
                    var ind = iFacesExpanded[i] * 4;

                    for (var j = 0; j < 4; ++j) {
                        var idv = fAr[ind + j];
                        if (idv == = Utils.TRI_INDEX)
                            continue;

                        var start, end;
                        if (ringFaces) {
                            vertRingFace = ringFaces[idv];
                            start = 0;
                            end = vertRingFace.length;
                        }
                        else {
                            start = vrfStartCount[idv * 2];
                            end = start + vrfStartCount[idv * 2 + 1];
                        }
                        for (var k = start; k < end; ++k) {
                            var id = vertRingFace[k];
                            if (ftf[id] == = tagFlag)
                                continue;
                            ftf[id] = tagFlag;
                            iFacesExpanded[acc++] = id;
                        }
                    }
                }
                iBegin = nbFaces;
                nbFaces = acc;
            }
            return new Uint32Array(iFacesExpanded.subarray(0, acc));
        }

        /** Return all the faces linked to a group of vertices */
        getFacesFromVertices(iVerts) {
            var tagFlag = ++Utils.TAG_FLAG;
            var nbVerts = iVerts.length;
            var vrfStartCount = getVerticesRingFaceStartCount();
            var vertRingFace = getVerticesRingFace();
            var ringFaces = vertRingFace instanceof Array ? vertRingFace : null;
            var ftf = getFacesTagFlags();
            var acc = 0;
            var iFaces = new Uint32Array(Utils.getMemory(4 * getNbFaces()), 0, getNbFaces());
            for (var i = 0; i < nbVerts; ++i) {
                var idVert = iVerts[i];
                var start, end;
                if (ringFaces) {
                    vertRingFace = ringFaces[idVert];
                    start = 0;
                    end = vertRingFace.length;
                }
                else {
                    start = vrfStartCount[idVert * 2];
                    end = start + vrfStartCount[idVert * 2 + 1];
                }
                for (var j = start; j < end; ++j) {
                    var iFace = vertRingFace[j];
                    if (ftf[iFace] != = tagFlag) {
                        ftf[iFace] = tagFlag;
                        iFaces[acc++] = iFace;
                    }
                }
            }
            return new Uint32Array(iFaces.subarray(0, acc));
        }

        /** Computes triangles */
        initRenderTriangles() {
            if (hasUV())
                _meshData->_UVtrianglesABC = computeTrianglesFromFaces(getFacesTexCoord());
            _meshData->_trianglesABC = computeTrianglesFromFaces(getFaces());
        }

        /** Computes triangles from faces */
        computeTrianglesFromFaces(faces) {
            var nbFaces = getNbFaces();
            var facesToTris = getFacesToTriangles();
            var iAr = new Uint32Array(Utils.getMemory(4 * nbFaces * 6), 0, nbFaces * 6);
            var acc = 0;
            for (var i = 0; i < nbFaces; ++i) {
                facesToTris[i] = acc;
                var iFace = i * 4;
                var iv1 = faces[iFace];
                var iv2 = faces[iFace + 1];
                var iv3 = faces[iFace + 2];
                var iv4 = faces[iFace + 3];
                var iTri = acc * 3;
                iAr[iTri] = iv1;
                iAr[iTri + 1] = iv2;
                iAr[iTri + 2] = iv3;
                ++acc;
                if (iv4 != = Utils.TRI_INDEX) {
                    iTri = acc * 3;
                    iAr[iTri] = iv1;
                    iAr[iTri + 1] = iv3;
                    iAr[iTri + 2] = iv4;
                    ++acc;
                }
            }
            return new Uint32Array(iAr.subarray(0, acc * 3));
        }

        initEdges() {
            var fAr = getFaces();
            var feAr = getFaceEdges();
            var nbEdges = 0;
            var vertEdgeTemp = new Uint32Array(getNbVertices());
            var vrfStartCount = getVerticesRingFaceStartCount();
            var vertRingFace = getVerticesRingFace();
            for (var i = 0, nbVerts = getNbVertices(); i < nbVerts; ++i) {
                var start = vrfStartCount[i * 2];
                var end = start + vrfStartCount[i * 2 + 1];
                var compTest = nbEdges;
                for (var j = start; j < end; ++j) {
                    var id = vertRingFace[j] * 4;
                    var iv1 = fAr[id];
                    var iv2 = fAr[id + 1];
                    var iv3 = fAr[id + 2];
                    var iv4 = fAr[id + 3];
                    var t = 0;
                    var idEdge = 0;
                    if (iv4 == = Utils.TRI_INDEX) {
                        if (i > iv1) {
                            t = vertEdgeTemp[iv1];
                            idEdge = id + (i == = iv2 ? 0 : 2);
                            if (t <= compTest) {
                                feAr[idEdge] = nbEdges;
                                vertEdgeTemp[iv1] = ++nbEdges;
                            }
                            else {
                                feAr[idEdge] = t - 1;
                            }
                        }
                        if (i > iv2) {
                            t = vertEdgeTemp[iv2];
                            idEdge = id + (i == = iv1 ? 0 : 1);
                            if (t <= compTest) {
                                feAr[idEdge] = nbEdges;
                                vertEdgeTemp[iv2] = ++nbEdges;
                            }
                            else {
                                feAr[idEdge] = t - 1;
                            }
                        }
                        if (i > iv3) {
                            t = vertEdgeTemp[iv3];
                            idEdge = id + (i == = iv1 ? 2 : 1);
                            if (t <= compTest) {
                                feAr[idEdge] = nbEdges;
                                vertEdgeTemp[iv3] = ++nbEdges;
                            }
                            else {
                                feAr[idEdge] = t - 1;
                            }
                        }
                        feAr[id + 3] = Utils.TRI_INDEX;
                    }
                    else {
                        if (i > iv1 && i != = iv3) {
                            t = vertEdgeTemp[iv1];
                            idEdge = id + (i == = iv2 ? 0 : 3);
                            if (t <= compTest) {
                                feAr[idEdge] = nbEdges;
                                vertEdgeTemp[iv1] = ++nbEdges;
                            }
                            else {
                                feAr[idEdge] = t - 1;
                            }
                        }
                        if (i > iv2 && i != = iv4) {
                            t = vertEdgeTemp[iv2];
                            idEdge = id + (i == = iv1 ? 0 : 1);
                            if (t <= compTest) {
                                feAr[idEdge] = nbEdges;
                                vertEdgeTemp[iv2] = ++nbEdges;
                            }
                            else {
                                feAr[idEdge] = t - 1;
                            }
                        }
                        if (i > iv3 && i != = iv1) {
                            t = vertEdgeTemp[iv3];
                            idEdge = id + (i == = iv2 ? 1 : 2);
                            if (t <= compTest) {
                                feAr[idEdge] = nbEdges;
                                vertEdgeTemp[iv3] = ++nbEdges;
                            }
                            else {
                                feAr[idEdge] = t - 1;
                            }
                        }
                        if (i > iv4 && i != = iv2) {
                            t = vertEdgeTemp[iv4];
                            idEdge = id + (i == = iv1 ? 3 : 2);
                            if (t <= compTest) {
                                feAr[idEdge] = nbEdges;
                                vertEdgeTemp[iv4] = ++nbEdges;
                            }
                            else {
                                feAr[idEdge] = t - 1;
                            }
                        }
                    }
                }
            }
            var eAr = _meshData->_edges = new Uint8ClampedArray(nbEdges);
            for (var k = 0, nbFaces = getNbFaces(); k < nbFaces; ++k) {
                var idf = k * 4;
                eAr[feAr[idf]]++;
                eAr[feAr[idf + 1]]++;
                eAr[feAr[idf + 2]]++;
                var i4 = feAr[idf + 3];
                if (i4 != = Utils.TRI_INDEX)
                    eAr[i4]++;
            }
        }

        /** Return wireframe array (or compute it if not up to date) */
        getWireframe() {
            var nbEdges = getNbEdges();
            var cdw;
            var useDrawArrays = isUsingDrawArrays();
            if (useDrawArrays) {
                if (_meshData->_drawArraysWireframe&& _meshData->_drawArraysWireframe.length == = nbEdges * 2) {
                    return _meshData->_drawArraysWireframe;
                }
                cdw = _meshData->_drawArraysWireframe = new Uint32Array(nbEdges * 2);
            }
            else {
                if (_meshData->_drawElementsWireframe&& _meshData->_drawElementsWireframe.length == = nbEdges * 2) {
                    return _meshData->_drawElementsWireframe;
                }
                cdw = _meshData->_drawElementsWireframe = new Uint32Array(nbEdges * 2);
            }

            var fAr = getFaces();
            var feAr = getFaceEdges();
            var nbFaces = getNbFaces();
            var facesToTris = getFacesToTriangles();

            var nbLines = 0;
            var tagEdges = new Uint8Array(nbEdges);

            for (var i = 0; i < nbFaces; ++i) {
                var id = i * 4;

                var iv1, iv2, iv3;
                var iv4 = fAr[id + 3];
                var isQuad = iv4 != = Utils.TRI_INDEX;

                if (useDrawArrays) {
                    var idTri = facesToTris[i] * 3;
                    iv1 = idTri;
                    iv2 = idTri + 1;
                    iv3 = idTri + 2;
                    if (isQuad) iv4 = idTri + 5;
                }
                else {
                    iv1 = fAr[id];
                    iv2 = fAr[id + 1];
                    iv3 = fAr[id + 2];
                }

                var ide1 = feAr[id];
                var ide2 = feAr[id + 1];
                var ide3 = feAr[id + 2];
                var ide4 = feAr[id + 3];

                if (tagEdges[ide1] == = 0) {
                    tagEdges[ide1] = 1;
                    cdw[nbLines * 2] = iv1;
                    cdw[nbLines * 2 + 1] = iv2;
                    nbLines++;
                }
                if (tagEdges[ide2] == = 0) {
                    tagEdges[ide2] = 1;
                    cdw[nbLines * 2] = iv2;
                    cdw[nbLines * 2 + 1] = iv3;
                    nbLines++;
                }
                if (tagEdges[ide3] == = 0) {
                    tagEdges[ide3] = 1;
                    cdw[nbLines * 2] = iv3;
                    cdw[nbLines * 2 + 1] = isQuad ? iv4 : iv1;
                    nbLines++;
                }
                if (isQuad&& tagEdges[ide4] == = 0) {
                    tagEdges[ide4] = 1;
                    cdw[nbLines * 2] = iv4;
                    cdw[nbLines * 2 + 1] = iv1;
                    nbLines++;
                }
            }
            return useDrawArrays ? _meshData->_drawArraysWireframe : _meshData->_drawElementsWireframe;
        }

        updateDuplicateGeometry(iVerts) {
            if (!isUsingTexCoords() || !hasUV())
                return;

            var vAr = getVertices();
            var cAr = getColors();
            var mAr = getMaterials();
            var nAr = getNormals();
            var startCount = getVerticesDuplicateStartCount();

            var full = iVerts == = undefined;
            var nbVerts = full ? getNbVertices() : iVerts.length;
            for (var i = 0; i < nbVerts; ++i) {
                var ind = full ? i : iVerts[i];
                var start = startCount[ind * 2];
                if (start == = 0)
                    continue;

                var end = start + startCount[ind * 2 + 1];
                var idOrig = ind * 3;
                var vx = vAr[idOrig];
                var vy = vAr[idOrig + 1];
                var vz = vAr[idOrig + 2];
                var nx = nAr[idOrig];
                var ny = nAr[idOrig + 1];
                var nz = nAr[idOrig + 2];
                var cx = cAr[idOrig];
                var cy = cAr[idOrig + 1];
                var cz = cAr[idOrig + 2];
                var mx = mAr[idOrig];
                var my = mAr[idOrig + 1];
                var mz = mAr[idOrig + 2];
                for (var j = start; j < end; ++j) {
                    var idDup = j * 3;
                    vAr[idDup] = vx;
                    vAr[idDup + 1] = vy;
                    vAr[idDup + 2] = vz;
                    nAr[idDup] = nx;
                    nAr[idDup + 1] = ny;
                    nAr[idDup + 2] = nz;
                    cAr[idDup] = cx;
                    cAr[idDup + 1] = cy;
                    cAr[idDup + 2] = cz;
                    mAr[idDup] = mx;
                    mAr[idDup + 1] = my;
                    mAr[idDup + 2] = mz;
                }
            }
        }

        updateDuplicateColorsAndMaterials(iVerts) {
            if (!isUsingTexCoords() || !hasUV())
                return;

            var cAr = getColors();
            var mAr = getMaterials();
            var startCount = getVerticesDuplicateStartCount();

            var full = iVerts == = undefined;
            var nbVerts = full ? getNbVertices() : iVerts.length;
            for (var i = 0; i < nbVerts; ++i) {
                var ind = full ? i : iVerts[i];
                var start = startCount[ind * 2];
                if (start == = 0)
                    continue;

                var end = start + startCount[ind * 2 + 1];
                var idOrig = ind * 3;
                var cx = cAr[idOrig];
                var cy = cAr[idOrig + 1];
                var cz = cAr[idOrig + 2];
                var mx = mAr[idOrig];
                var my = mAr[idOrig + 1];
                var mz = mAr[idOrig + 2];
                for (var j = start; j < end; ++j) {
                    var idDup = j * 3;
                    cAr[idDup] = cx;
                    cAr[idDup + 1] = cy;
                    cAr[idDup + 2] = cz;
                    mAr[idDup] = mx;
                    mAr[idDup + 1] = my;
                    mAr[idDup + 2] = mz;
                }
            }
        }

        initTexCoordsDataFromOBJData(uvAr, uvfArOrig) {
            var fAr = getFaces();
            var len = fAr.length;

            if (len != uvfArOrig.length) return;
            var nbUV = uvAr.length / 2;

            // fix integrity (bug with sgl export??)
            for (i = 0; i < len; ++i) {
                if (uvfArOrig[i] >= nbUV) {
                    uvfArOrig[i] = fAr[i];
                }
            }

            var nbVertices = getNbVertices();
            var i = 0;
            var j = 0;
            var iv = 0;
            var tag = 0;

            // detect duplicates vertices because of tex coords
            var tagV = new Int32Array(nbVertices);
            // vertex without uv might receive random values...
            var tArTemp = new Float32Array(Utils.getMemory(nbVertices * 4 * 2), 0, nbVertices * 2);
            var dup = [];
            var acc = 0;
            var nbDuplicates = 0;
            for (i = 0; i < len; ++i) {
                iv = fAr[i];
                if (iv == = Utils.TRI_INDEX)
                    continue;

                var uv = uvfArOrig[i];
                tag = tagV[iv];
                if (tag == = (uv + 1))
                    continue;

                if (tag == = 0) {
                    tagV[iv] = uv + 1;
                    tArTemp[iv * 2] = uvAr[uv * 2];
                    tArTemp[iv * 2 + 1] = uvAr[uv * 2 + 1];
                    continue;
                }

                // first duplicate
                if (tag > 0) {
                    tagV[iv] = --acc;
                    dup.push([uv]);
                    ++nbDuplicates;
                    continue;
                }
                // check if we need to insert a new duplicate
                var dupArray = dup[-tag - 1];
                var nbDup = dupArray.length;
                for (j = 0; j < nbDup; ++j) {
                    if (dupArray[j] == = uv)
                        break;
                }
                // insert new duplicate
                if (j == = nbDup) {
                    ++nbDuplicates;
                    dupArray.push(uv);
                }
            }

            // order the duplicates vertices (and tex coords)
            var tAr = new Float32Array((nbVertices + nbDuplicates) * 2);
            tAr.set(tArTemp);
            var startCount = _meshData->_duplicateStartCount = new Uint32Array(nbVertices * 2);
            acc = 0;
            for (i = 0; i < nbVertices; ++i) {
                tag = tagV[i];
                if (tag >= 0)
                    continue;

                var dAr = dup[-tag - 1];
                var nbDu = dAr.length;
                var start = nbVertices + acc;
                startCount[i * 2] = start;
                startCount[i * 2 + 1] = nbDu;
                acc += nbDu;
                for (j = 0; j < nbDu; ++j) {
                    var idUv = dAr[j] * 2;
                    var idUvCoord = (start + j) * 2;
                    tAr[idUvCoord] = uvAr[idUv];
                    tAr[idUvCoord + 1] = uvAr[idUv + 1];
                }
            }

            // create faces that uses duplicates vertices (with textures coordinates)
            var uvfAr = new Uint32Array(fAr);
            len = fAr.length;
            for (i = 0; i < len; ++i) {
                iv = uvfAr[i];
                if (iv == = Utils.TRI_INDEX)
                    continue;

                tag = tagV[iv];
                if (tag > 0)
                    continue;

                var idtex = uvfArOrig[i];
                var dArray = dup[-tag - 1];
                var nbEl = dArray.length;
                for (j = 0; j < nbEl; ++j) {
                    if (idtex == = dArray[j]) {
                        uvfAr[i] = startCount[iv * 2] + j;
                        break;
                    }
                }
            }

            setTexCoords(tAr);
            setFacesTexCoord(uvfAr);
        }

        setAlreadyDrawArrays() {
            // kind of a hack, to be used if the main arrays are already draw arrays
            _meshData->_DAverticesXYZ = getVertices();
            _meshData->_DAnormalsXYZ = getNormals();
            _meshData->_DAcolorsRGB = getColors();
            _meshData->_DAmaterialsPBR = getMaterials();
        }

        /** Updates the arrays that are going to be used by webgl */
        updateDrawArrays(iFaces) {
            if (!isUsingDrawArrays())
                return;

            var vAr = getVertices();
            var nAr = getNormals();
            var cAr = getColors();
            var mAr = getMaterials();

            var fAr = getFaces();

            var nbTriangles = getNbTriangles();
            var facesToTris = hasOnlyTriangles() ? null : getFacesToTriangles();

            var full = iFaces == = undefined;
            var cdv = _meshData->_DAverticesXYZ;
            var cdn = _meshData->_DAnormalsXYZ;
            var cdc = _meshData->_DAcolorsRGB;
            var cdm = _meshData->_DAmaterialsPBR;

            if (!cdv || cdv.length < nbTriangles * 9) {
                cdv = _meshData->_DAverticesXYZ = new Float32Array(nbTriangles * 9);
                cdn = _meshData->_DAnormalsXYZ = new Float32Array(nbTriangles * 9);
                cdc = _meshData->_DAcolorsRGB = new Float32Array(nbTriangles * 9);
                cdm = _meshData->_DAmaterialsPBR = new Float32Array(nbTriangles * 9);
            }

            var nbFaces = full ? getNbFaces() : iFaces.length;
            for (var i = 0; i < nbFaces; ++i) {
                var idFace = full ? i : iFaces[i];
                var ftt = facesToTris ? facesToTris[idFace] : idFace;
                var vId = ftt * 9;

                idFace *= 4;
                var id1 = fAr[idFace] * 3;
                var id2 = fAr[idFace + 1] * 3;
                var id3 = fAr[idFace + 2] * 3;

                // coordinates
                cdv[vId] = vAr[id1];
                cdv[vId + 1] = vAr[id1 + 1];
                cdv[vId + 2] = vAr[id1 + 2];
                cdv[vId + 3] = vAr[id2];
                cdv[vId + 4] = vAr[id2 + 1];
                cdv[vId + 5] = vAr[id2 + 2];
                cdv[vId + 6] = vAr[id3];
                cdv[vId + 7] = vAr[id3 + 1];
                cdv[vId + 8] = vAr[id3 + 2];

                // color
                cdc[vId] = cAr[id1];
                cdc[vId + 1] = cAr[id1 + 1];
                cdc[vId + 2] = cAr[id1 + 2];
                cdc[vId + 3] = cAr[id2];
                cdc[vId + 4] = cAr[id2 + 1];
                cdc[vId + 5] = cAr[id2 + 2];
                cdc[vId + 6] = cAr[id3];
                cdc[vId + 7] = cAr[id3 + 1];
                cdc[vId + 8] = cAr[id3 + 2];

                // material
                cdm[vId] = mAr[id1];
                cdm[vId + 1] = mAr[id1 + 1];
                cdm[vId + 2] = mAr[id1 + 2];
                cdm[vId + 3] = mAr[id2];
                cdm[vId + 4] = mAr[id2 + 1];
                cdm[vId + 5] = mAr[id2 + 2];
                cdm[vId + 6] = mAr[id3];
                cdm[vId + 7] = mAr[id3 + 1];
                cdm[vId + 8] = mAr[id3 + 2];

                // normals
                cdn[vId] = nAr[id1];
                cdn[vId + 1] = nAr[id1 + 1];
                cdn[vId + 2] = nAr[id1 + 2];
                cdn[vId + 3] = nAr[id2];
                cdn[vId + 4] = nAr[id2 + 1];
                cdn[vId + 5] = nAr[id2 + 2];
                cdn[vId + 6] = nAr[id3];
                cdn[vId + 7] = nAr[id3 + 1];
                cdn[vId + 8] = nAr[id3 + 2];

                var id4 = fAr[idFace + 3];
                if (id4 == = Utils.TRI_INDEX)
                    continue;
                id4 *= 3;

                vId += 9;
                // coordinates
                cdv[vId] = vAr[id1];
                cdv[vId + 1] = vAr[id1 + 1];
                cdv[vId + 2] = vAr[id1 + 2];
                cdv[vId + 3] = vAr[id3];
                cdv[vId + 4] = vAr[id3 + 1];
                cdv[vId + 5] = vAr[id3 + 2];
                cdv[vId + 6] = vAr[id4];
                cdv[vId + 7] = vAr[id4 + 1];
                cdv[vId + 8] = vAr[id4 + 2];

                // colors
                cdc[vId] = cAr[id1];
                cdc[vId + 1] = cAr[id1 + 1];
                cdc[vId + 2] = cAr[id1 + 2];
                cdc[vId + 3] = cAr[id3];
                cdc[vId + 4] = cAr[id3 + 1];
                cdc[vId + 5] = cAr[id3 + 2];
                cdc[vId + 6] = cAr[id4];
                cdc[vId + 7] = cAr[id4 + 1];
                cdc[vId + 8] = cAr[id4 + 2];

                // materials
                cdm[vId] = mAr[id1];
                cdm[vId + 1] = mAr[id1 + 1];
                cdm[vId + 2] = mAr[id1 + 2];
                cdm[vId + 3] = mAr[id3];
                cdm[vId + 4] = mAr[id3 + 1];
                cdm[vId + 5] = mAr[id3 + 2];
                cdm[vId + 6] = mAr[id4];
                cdm[vId + 7] = mAr[id4 + 1];
                cdm[vId + 8] = mAr[id4 + 2];

                // normals
                cdn[vId] = nAr[id1];
                cdn[vId + 1] = nAr[id1 + 1];
                cdn[vId + 2] = nAr[id1 + 2];
                cdn[vId + 3] = nAr[id3];
                cdn[vId + 4] = nAr[id3 + 1];
                cdn[vId + 5] = nAr[id3 + 2];
                cdn[vId + 6] = nAr[id4];
                cdn[vId + 7] = nAr[id4 + 1];
                cdn[vId + 8] = nAr[id4 + 2];
            }

            if (isUsingTexCoords())
                updateDrawArraysTexCoord(iFaces);
        }

        /** Updates the UV array data for drawArrays */
        updateDrawArraysTexCoord(iFaces) {
            var nbTriangles = getNbTriangles();
            var facesToTris = getFacesToTriangles();

            var full = iFaces == = undefined;
            var cdt = _meshData->_DAtexCoordsST;
            if (!cdt || cdt.length != = nbTriangles * 6)
                cdt = _meshData->_DAtexCoordsST = new Float32Array(nbTriangles * 6);

            var tAr = getTexCoords();
            var fArUV = getFacesTexCoord();

            var nbFaces = full ? getNbFaces() : iFaces.length;
            for (var i = 0; i < nbFaces; ++i) {
                var idFace = full ? i : iFaces[i];
                var ftt = facesToTris[idFace];
                var vIduv = ftt * 6;

                idFace *= 4;
                var id1uv = fArUV[idFace] * 2;
                var id2uv = fArUV[idFace + 1] * 2;
                var id3uv = fArUV[idFace + 2] * 2;

                cdt[vIduv] = tAr[id1uv];
                cdt[vIduv + 1] = tAr[id1uv + 1];
                cdt[vIduv + 2] = tAr[id2uv];
                cdt[vIduv + 3] = tAr[id2uv + 1];
                cdt[vIduv + 4] = tAr[id3uv];
                cdt[vIduv + 5] = tAr[id3uv + 1];

                var id4uv = fArUV[idFace + 3];
                if (id4uv == = Utils.TRI_INDEX)
                    continue;
                id4uv *= 3;

                vIduv += 6;
                cdt[vIduv] = tAr[id1uv];
                cdt[vIduv + 1] = tAr[id1uv + 1];
                cdt[vIduv + 2] = tAr[id3uv];
                cdt[vIduv + 3] = tAr[id3uv + 1];
                cdt[vIduv + 4] = tAr[id4uv];
                cdt[vIduv + 5] = tAr[id4uv + 1];
            }
        }

        /////////////////
        // TRANSFORM DATA
        /////////////////
        updateCenter() {
            var box = getLocalBound();
            vec3.set(_transformData->_center, (box[0] + box[3]) * 0.5, (box[1] + box[4]) * 0.5, (box[2] + box[5]) * 0.5);
        }

        /** Pre compute mv and mvp matrices as well as the depth center */
        updateMatrices(camera) {
            mat3.normalFromMat4(_transformData->_lastComputedEN, _transformData->_editMatrix);
            mat4.mul(_transformData->_lastComputedMV, camera.getView(), _transformData->_matrix);
            mat3.normalFromMat4(_transformData->_lastComputedN, _transformData->_lastComputedMV);
            mat4.mul(_transformData->_lastComputedMVP, camera.getProjection(), _transformData->_lastComputedMV);
            var cen = _transformData->_center;
            var m = _transformData->_lastComputedMVP;
            _transformData->_lastComputedDepth = m[2] * cen[0] + m[6] * cen[1] + m[10] * cen[2] + m[14];
        }

        computeLocalRadius() {
            var box = getLocalBound();
            return 0.5 * vec3.dist([box[0], box[1], box[2]], [box[3], box[4], box[5]]);
        }

        normalizeSize() {
            var scale = Utils.SCALE / (2.0 * computeLocalRadius());
            mat4.scale(_transformData->_matrix, _transformData->_matrix, [scale, scale, scale]);
        }

        computeWorldBound() {
            var worldb = _meshData->_worldBound;
            var localb = getLocalBound();
            var mat = getMatrix();

            // trans
            worldb[0] = worldb[3] = mat[12];
            worldb[1] = worldb[4] = mat[13];
            worldb[2] = worldb[5] = mat[14];

            // rotate per component
            for (var i = 0; i < 3; ++i) {
                var i4 = i * 4;
                var mini = localb[i];
                var maxi = localb[i + 3];
                for (var j = 0; j < 3; ++j) {
                    var cm = mat[i4 + j];
                    var a = cm * maxi;
                    var b = cm * mini;
                    if (a < b) {
                        worldb[j] += a;
                        worldb[j + 3] += b;
                    }
                    else {
                        worldb[j] += b;
                        worldb[j + 3] += a;
                    }
                }
            }

            return worldb;
        }

        /////////
        // OCTREE
        /////////

        /** Return faces intersected by a ray */
        intersectRay(vNear, eyeDir, collectLeaves) {
            var nbFaces = getNbFaces();
            var collectFaces = new Uint32Array(Utils.getMemory(nbFaces * 4), 0, nbFaces);
            return _meshData->_octree.collectIntersectRay(vNear, eyeDir, collectFaces, collectLeaves ? _meshData->_leavesToUpdate : undefined);
        }

        /** Return faces inside a sphere */
        intersectSphere(vert, radiusSquared, collectLeaves) {
            var nbFaces = getNbFaces();
            var collectFaces = new Uint32Array(Utils.getMemory(nbFaces * 4), 0, nbFaces);
            return _meshData->_octree.collectIntersectSphere(vert, radiusSquared, collectFaces, collectLeaves ? _meshData->_leavesToUpdate : undefined);
        }

        /**
         * Update Octree
         * For each faces we check if its position inside the octree has changed
         * if so... we mark this face and we remove it from its former cells
         * We push back the marked faces into the octree
         */
        updateOctree(iFaces) {
            if (iFaces)
                updateOctreeAdd(updateOctreeRemove(iFaces));
            else
                computeOctree();
        }

        computeAabb() {
            var nbVertices = getNbVertices();
            var vAr = getVertices();
            var xmin = Infinity;
            var ymin = Infinity;
            var zmin = Infinity;
            var xmax = -Infinity;
            var ymax = -Infinity;
            var zmax = -Infinity;
            for (var i = 0; i < nbVertices; ++i) {
                var j = i * 3;
                var vx = vAr[j];
                var vy = vAr[j + 1];
                var vz = vAr[j + 2];
                if (vx < xmin) xmin = vx;
                if (vx > xmax) xmax = vx;
                if (vy < ymin) ymin = vy;
                if (vy > ymax) ymax = vy;
                if (vz < zmin) zmin = vz;
                if (vz > zmax) zmax = vz;
            }
            return[xmin, ymin, zmin, xmax, ymax, zmax];
        }

        /** Compute the octree */
        computeOctree() {
            var abRoot = computeAabb();
            var xmin = abRoot[0];
            var ymin = abRoot[1];
            var zmin = abRoot[2];
            var xmax = abRoot[3];
            var ymax = abRoot[4];
            var zmax = abRoot[5];
            var dx = xmax - xmin;
            var dy = ymax - ymin;
            var dz = zmax - zmin;

            // add minimal thickness
            var offset = Math.sqrt(dx * dx + dy * dy + dz * dz) * 0.2;
            var eps = 1e-16;
            if (xmax - xmin < eps) {
                xmin -= offset;
                xmax += offset;
            }
            if (ymax - ymin < eps) {
                ymin -= offset;
                ymax += offset;
            }
            if (zmax - zmin < eps) {
                zmin -= offset;
                zmax += offset;
            }

            // root octree bigger than minimum aabb... (to avoid too many octree rebuild)
            var dfx = dx * 0.3;
            var dfy = dy * 0.3;
            var dfz = dz * 0.3;
            var xmin2 = xmin - dfx;
            var xmax2 = xmax + dfx;
            var ymin2 = ymin - dfy;
            var ymax2 = ymax + dfy;
            var zmin2 = zmin - dfz;
            var zmax2 = zmax + dfz;

            // octree construction
            var octree = _meshData->_octree = new OctreeCell();
            octree.resetNbFaces(getNbFaces());
            octree.setAabbLoose(xmin, ymin, zmin, xmax, ymax, zmax);
            octree.setAabbSplit(xmin2, ymin2, zmin2, xmax2, ymax2, zmax2);
            octree.build(this);
        }

        updateOctreeRemove(iFaces) {
            var faceCenters = getFaceCenters();
            var fboxes = getFaceBoxes();
            var facePosInLeaf = _meshData->_facePosInLeaf;
            var faceLeaf = _meshData->_faceLeaf;
            var nbFaces = iFaces.length;
            var acc = 0;
            var facesToMove = new Uint32Array(Utils.getMemory(4 * nbFaces), 0, nbFaces);
            // recompute position inside the octree
            for (var i = 0; i < nbFaces; ++i) {
                var idFace = iFaces[i];
                var idb = idFace * 6;
                var idCen = idFace * 3;
                var leaf = faceLeaf[idFace];
                var ab = leaf._aabbSplit;

                var vx = faceCenters[idCen];
                var vy = faceCenters[idCen + 1];
                var vz = faceCenters[idCen + 2];

                if (vx <= ab[0] || vy <= ab[1] || vz <= ab[2] || vx > ab[3] || vy > ab[4] || vz > ab[5]) {
                    // a face center has moved from its cell
                    facesToMove[acc++] = iFaces[i];
                    var facesInLeaf = leaf._iFaces;
                    if (facesInLeaf.length > 0) { // remove faces from octree cell
                        var iFaceLast = facesInLeaf[facesInLeaf.length - 1];
                        var iPos = facePosInLeaf[idFace];
                        facesInLeaf[iPos] = iFaceLast;
                        facePosInLeaf[iFaceLast] = iPos;
                        facesInLeaf.pop();
                    }
                }
                else { // expands cell aabb loose if necessary
                    leaf.expandsAabbLoose(fboxes[idb], fboxes[idb + 1], fboxes[idb + 2], fboxes[idb + 3], fboxes[idb + 4], fboxes[idb + 5]);
                }
            }
            return new Uint32Array(facesToMove.subarray(0, acc));
        }

        updateOctreeAdd(facesToMove) {
            var fc = getFaceCenters();
            var fb = getFaceBoxes();
            var facePosInLeaf = _meshData->_facePosInLeaf;
            var faceLeaf = _meshData->_faceLeaf;
            var nbFacesToMove = facesToMove.length;

            var root = _meshData->_octree;
            var rootSplit = root._aabbSplit;
            var xmin = rootSplit[0];
            var ymin = rootSplit[1];
            var zmin = rootSplit[2];
            var xmax = rootSplit[3];
            var ymax = rootSplit[4];
            var zmax = rootSplit[5];

            for (var i = 0; i < nbFacesToMove; ++i) { // add face to the octree
                var idFace = facesToMove[i];
                var idb = idFace * 6;
                var ibux = fb[idb];
                var ibuy = fb[idb + 1];
                var ibuz = fb[idb + 2];
                var iblx = fb[idb + 3];
                var ibly = fb[idb + 4];
                var iblz = fb[idb + 5];

                if (ibux > xmax || iblx < xmin || ibuy > ymax || ibly < ymin || ibuz > zmax || iblz < zmin) {
                    // a face is outside the root node, we reconstruct the whole octree, slow... but rare
                    computeOctree();
                    _meshData->_leavesToUpdate.length = 0;
                    break;
                }

                var idc = idFace * 3;
                var newleaf = root.addFace(idFace, ibux, ibuy, ibuz, iblx, ibly, iblz, fc[idc], fc[idc + 1], fc[idc + 2]);
                if (newleaf) {
                    facePosInLeaf[idFace] = newleaf._iFaces.length - 1;
                    faceLeaf[idFace] = newleaf;
                }
                else { // failed to insert face in octree, re-insert it back
                    var facesInLeaf = faceLeaf[idFace]._iFaces;
                    facePosInLeaf[idFace] = facesInLeaf.length;
                    facesInLeaf.push(facesToMove[i]);
                }
            }
        }

        /** balance octree (cut empty leaves or go deeper if needed) */
        balanceOctree() {
            ++OctreeCell.FLAG;
            var leavesToUpdate = _meshData->_leavesToUpdate;
            var nbLeaves = leavesToUpdate.length;

            for (var i = 0; i < nbLeaves; ++i) {
                var leaf = leavesToUpdate[i];
                if (leaf._flag == = OctreeCell.FLAG)
                    continue;

                if (leaf._iFaces.length == = 0)
                    leaf.pruneIfPossible();
                else if (leaf._iFaces.length > OctreeCell.MAX_FACES && leaf._depth < OctreeCell.MAX_DEPTH)
                    leaf.build(this);

                leaf._flag = OctreeCell.FLAG;
            }

            leavesToUpdate.length = 0;
        }

        //////////////
        // RENDER DATA
        //////////////
        setFlatColor(val) {
            getFlatColor().set(val);
        }

        setAlbedo(val) {
            getAlbedo().set(val);
        }

        setMode(mode) {
            _renderData._mode = mode;
        }

        setRoughness(val) {
            _renderData._roughness = val;
        }

        setMetallic(val) {
            _renderData._metallic = val;
        }

        setOpacity(alpha) {
            _renderData._alpha = alpha;
        }

        setTexture0(tex) {
            _renderData._texture0 = tex;
        }

        setMatcap(idMat) {
            _renderData._matcap = idMat;
        }

        setCurvature(cur) {
            _renderData._curvature = cur;
        }

        setFlatShading(flatShading) {
            _renderData._flatShading = flatShading;
        }

        setShowWireframe(showWireframe) {
            _renderData._showWireframe = RenderData.ONLY_DRAW_ARRAYS ? false : showWireframe;
            updateWireframeBuffer();
        }

        setUseDrawArrays(bool) {
            _renderData._useDrawArrays = bool;
        }

        getGL() {
            return _renderData._gl;
        }

        getCount() {
            var gl = getGL();
            if (getMode() == = gl.TRIANGLES) return getNbTriangles() * 3;
            if (getMode() == = gl.LINES) return getNbVertices();
            return 0;
        }

        getVertexBuffer() {
            return _renderData._vertexBuffer;
        }

        getNormalBuffer() {
            return _renderData._normalBuffer;
        }

        getColorBuffer() {
            return _renderData._colorBuffer;
        }

        getMaterialBuffer() {
            return _renderData._materialBuffer;
        }

        getTexCoordBuffer() {
            return _renderData._texCoordBuffer;
        }

        getIndexBuffer() {
            return _renderData._indexBuffer;
        }

        getWireframeBuffer() {
            return _renderData._wireframeBuffer;
        }

        getRoughness() {
            return _renderData._roughness;
        }

        getMetallic() {
            return _renderData._metallic;
        }

        getOpacity() {
            return _renderData._alpha;
        }

        getFlatColor() {
            return _renderData._flatColor;
        }

        getMode() {
            return _renderData._mode;
        }

        getAlbedo() {
            return _renderData._albedo;
        }

        getTexture0() {
            return _renderData._texture0;
        }

        getMatcap() {
            return _renderData._matcap;
        }

        getCurvature() {
            return _renderData._curvature;
        }

        getFlatShading() {
            return _renderData._flatShading;
        }

        getShowWireframe() {
            return _renderData._showWireframe;
        }

        isUsingDrawArrays() {
            return _renderData._useDrawArrays || RenderData.ONLY_DRAW_ARRAYS;
        }

        isUsingTexCoords() {
            var shaderType = _renderData._shaderType;
            return shaderType == = Enums.Shader.UV || shaderType == = Enums.Shader.PAINTUV;
        }

        isTransparent() {
            return _renderData._alpha < 0.99;
        }

        getShaderType() {
            return _renderData._shaderType;
        }

        setShaderType(shaderName) {
            var hasUV = hasUV();
            if (shaderName == = Enums.Shader.UV && !hasUV) {
                if (_renderData._shaderType != = Enums.Shader.UV)
                    return;
                shaderName = Enums.Shader.MATCAP;
            }

            _renderData._shaderType = shaderName;
            if (hasUV) {
                updateDuplicateGeometry();
                updateDrawArrays();
            }
            updateBuffers();
        }

        initRender() {
            setShaderType(_renderData._shaderType);
            setShowWireframe(getShowWireframe());
        }

        /////////
        // RENDER
        /////////
        render(main) {
            if (!isVisible()) return;
            Shader[getShaderType()].getOrCreate(getGL()).draw(this, main);
        }

        renderWireframe(main) {
            if (!isVisible()) return;
            Shader[Enums.Shader.WIREFRAME].getOrCreate(getGL()).draw(this, main);
        }

        renderFlatColor(main) {
            if (!isVisible()) return;
            Shader[Enums.Shader.FLAT].getOrCreate(getGL()).draw(this, main);
        }

        /////////////////
        // UPDATE BUFFERS
        /////////////////
        getRenderNbVertices() {
            if (isUsingDrawArrays()) return getCount();
            return isUsingTexCoords() ? getNbTexCoords() : getNbVertices();
        }

        updateVertexBuffer() {
            var vertices = isUsingDrawArrays() ? getVerticesDrawArrays() : getVertices();
            getVertexBuffer().update(vertices, getRenderNbVertices() * 3);
        }

        updateNormalBuffer() {
            var normals = isUsingDrawArrays() ? getNormalsDrawArrays() : getNormals();
            getNormalBuffer().update(normals, getRenderNbVertices() * 3);
        }

        updateColorBuffer() {
            var colors = isUsingDrawArrays() ? getColorsDrawArrays() : getColors();
            getColorBuffer().update(colors, getRenderNbVertices() * 3);
        }

        updateMaterialBuffer() {
            var materials = isUsingDrawArrays() ? getMaterialsDrawArrays() : getMaterials();
            getMaterialBuffer().update(materials, getRenderNbVertices() * 3);
        }

        updateTexCoordBuffer() {
            if (isUsingTexCoords()) {
                var texCoords = isUsingDrawArrays() ? getTexCoordsDrawArrays() : getTexCoords();
                getTexCoordBuffer().update(texCoords, getRenderNbVertices() * 2);
            }
        }

        updateIndexBuffer() {
            if (!isUsingDrawArrays()) {
                var triangles = isUsingTexCoords() ? getTrianglesTexCoord() : getTriangles();
                getIndexBuffer().update(triangles, getNbTriangles() * 3);
            }
        }

        updateWireframeBuffer() {
            if (getShowWireframe())
                getWireframeBuffer().update(getWireframe(), getNbEdges() * 2);
        }

        updateGeometryBuffers() {
            updateVertexBuffer();
            updateNormalBuffer();
        }

        updateBuffers() {
            updateGeometryBuffers();
            updateColorBuffer();
            updateMaterialBuffer();
            updateTexCoordBuffer();
            updateIndexBuffer();
            updateWireframeBuffer();
        }

        release() {
            if (getTexture0())
                getGL().deleteTexture(getTexture0());
            getVertexBuffer().release();
            getNormalBuffer().release();
            getColorBuffer().release();
            getMaterialBuffer().release();
            getIndexBuffer().release();
            getWireframeBuffer().release();
        }

        copyRenderConfig(mesh) {
            setFlatShading(mesh.getFlatShading());
            setShowWireframe(mesh.getShowWireframe());
            setShaderType(mesh.getShaderType());
            setMatcap(mesh.getMatcap());
            setTexture0(mesh.getTexture0());
            setCurvature(mesh.getCurvature());
            setOpacity(mesh.getOpacity());
        }

        copyTransformData(mesh) {
            vec3.copy(getCenter(), mesh.getCenter());
            mat4.copy(getMatrix(), mesh.getMatrix());
            mat4.copy(getEditMatrix(), mesh.getEditMatrix());
            vec3.copy(getSymmetryNormal(), mesh.getSymmetryNormal());
        }

        copyData(mesh) {
            setVertices(mesh.getVertices().slice());
            setFaces(mesh.getFaces().slice());

            setColors(mesh.getColors().slice());
            setMaterials(mesh.getMaterials().slice());
            if (mesh.hasUV()) {
                initTexCoordsDataFromOBJData(mesh.getTexCoords(), mesh.getFacesTexCoord());
            }

            init();
            initRender();

            copyTransformData(mesh);
            copyRenderConfig(mesh);
        }

        optimize() {
            if (getNbFaces() == = 0 || !Mesh.OPTIMIZE)
                return;

            // lower is better :
            // ACTVR : ~1 is optimal (soup or not)
            // ACMR : ~0.5 optimal ratio, 3 for triangle soup
            // var data = computeCacheScore();

            optimizePostTransform();
            optimizePreTransform();
            initFaceRings();

            // console.log('\nbefore ACMR ' + data.ACMR);
            // console.log('before ACTVR ' + data.ACTVR);
            // data = computeCacheScore();
            // console.log('after ACMR ' + data.ACMR);
            // console.log('after ACTVR ' + data.ACTVR);
        }

        computeCacheScore() {
            var fAr = getFaces();
            var nbFaces = getNbFaces();

            var sizeCache = 32;
            var cache = [];
            cache.length = sizeCache;

            var cacheMiss = 0;
            var k = 0;
            for (var i = 0; i < nbFaces; ++i) {
                var id = i * 3;
                var nbPoly = fAr[id + 3] != = Utils.TRI_INDEX ? 4 : 3;
                // check in cache
                for (var j = 0; j < nbPoly; ++j) {

                    var idFace = fAr[id + j];
                    // check in cache
                    for (k = 0; k < sizeCache; ++k) {
                        if (cache[k] == = undefined) {
                            cache[k] = idFace;
                            cacheMiss++;
                            break;
                        }
                        else if (cache[k] == = idFace) {
                            break;
                        }
                    }

                    if (k == = sizeCache) {
                        cacheMiss++;
                        cache.shift();
                        cache.push(idFace);
                    }
                }
            }

            return {
              ACMR: cacheMiss / nbFaces,
              ACTVR : cacheMiss / getNbVertices()
            };
        }

        optimizePostTransform() {
            // post transform optimization (index buffer re-index), it implements tipsy
            // http://gfx.cs.princeton.edu/pubs/Sander_2007_%3ETR/tipsy.pdf

            var i = 0;
            var cacheSize = 32;
            var hasUV = hasUV();
            var fAr = getFaces();
            var fArUV = hasUV ? getFacesTexCoord() : fAr;

            var nbFaces = getNbFaces();
            var nbUniqueVertices = getNbVertices();
            var nbVertices = hasUV ? getNbTexCoords() : nbUniqueVertices;

            var dupUV = getVerticesDuplicateStartCount();
            var mapToUnique = new Uint32Array(nbVertices - nbUniqueVertices);
            if (hasUV) {
                for (i = 0; i < nbVertices; ++i) {
                    var dupStart = dupUV[i * 2];
                    var dupEnd = dupStart + dupUV[i * 2 + 1];
                    for (var j = dupStart; j < dupEnd; ++j) {
                        mapToUnique[j - nbUniqueVertices] = i;
                    }
                }
            }

            var fringsStartCount = getVerticesRingFaceStartCount();
            var frings = getVerticesRingFace();

            var livesTriangles = new Int32Array(nbVertices);
            for (i = 0; i < nbUniqueVertices; ++i) {
                livesTriangles[i] = fringsStartCount[i * 2 + 1];
            }

            for (i = nbUniqueVertices; i < nbVertices; ++i) {
                livesTriangles[i] = fringsStartCount[mapToUnique[i - nbUniqueVertices] * 2 + 1];
            }

            var vTimeStamp = new Uint32Array(nbVertices);
            var timeStamp = cacheSize + 1;
            var cursor = 0;

            var deadEndStack = new Uint32Array(Utils.getMemory(4 * nbFaces * 4), 0, nbFaces * 4);
            var deadEndCount = 0;
            var emittedFaces = new Uint8Array(nbFaces);

            var fArUVNew = new Uint32Array(nbFaces * 4);
            var fArNew = hasUV ? new Uint32Array(nbFaces * 4) : fArUVNew;
            var fcount = 0;

            var fanningVertex = 0;
            while (fanningVertex >= 0) {

                var ringCandidates = [];

                var idRing = fanningVertex >= nbUniqueVertices ? mapToUnique[fanningVertex - nbUniqueVertices] : fanningVertex;
                var start = fringsStartCount[idRing * 2];
                var end = start + fringsStartCount[idRing * 2 + 1];

                for (i = start; i < end; ++i) {
                    var idFace = frings[i];
                    if (emittedFaces[idFace]) continue;
                    emittedFaces[idFace] = 1;

                    var idf = idFace * 4;

                    var idv1 = deadEndStack[deadEndCount++] = fArUVNew[fcount++] = fArUV[idf];
                    var idv2 = deadEndStack[deadEndCount++] = fArUVNew[fcount++] = fArUV[idf + 1];
                    var idv3 = deadEndStack[deadEndCount++] = fArUVNew[fcount++] = fArUV[idf + 2];
                    var idv4 = fArUVNew[fcount++] = fArUV[idf + 3];
                    var isQuad = idv4 != = Utils.TRI_INDEX;

                    if (hasUV) {
                        fArNew[fcount - 4] = fAr[idf];
                        fArNew[fcount - 3] = fAr[idf + 1];
                        fArNew[fcount - 2] = fAr[idf + 2];
                        fArNew[fcount - 1] = fAr[idf + 3];
                    }

                    ringCandidates.push(idv1, idv2, idv3);

                    --livesTriangles[idv1];
                    --livesTriangles[idv2];
                    --livesTriangles[idv3];

                    if (timeStamp - vTimeStamp[idv1] > cacheSize) vTimeStamp[idv1] = timeStamp++;
                    if (timeStamp - vTimeStamp[idv2] > cacheSize) vTimeStamp[idv2] = timeStamp++;
                    if (timeStamp - vTimeStamp[idv3] > cacheSize) vTimeStamp[idv3] = timeStamp++;

                    if (isQuad) {
                        deadEndStack[deadEndCount++] = idv4;
                        ringCandidates.push(idv4);
                        --livesTriangles[idv4];
                        if (timeStamp - vTimeStamp[idv4] > cacheSize) vTimeStamp[idv4] = timeStamp++;
                    }

                }

                // get emitted next vertex
                fanningVertex = -1;
                var bestPriority = -1;
                var nbCandidates = ringCandidates.length;
                for (i = 0; i < nbCandidates; ++i) {
                    var idc = ringCandidates[i];
                    var liveCount = livesTriangles[idc];
                    if (liveCount <= 0) continue;

                    var priority = 0;
                    var posCache = timeStamp - vTimeStamp[idc];
                    if (posCache + 2 * liveCount <= cacheSize) {
                        priority = posCache;
                    }

                    if (priority > bestPriority) {
                        bestPriority = priority;
                        fanningVertex = idc;
                    }
                }

                if (fanningVertex != = -1) continue;

                while (deadEndCount > 0) {
                    var vEnd = deadEndStack[--deadEndCount];
                    if (livesTriangles[vEnd] > 0) {
                        fanningVertex = vEnd;
                        break;
                    }
                }

                if (fanningVertex != = -1) continue;

                while (cursor < nbVertices) {
                    if (livesTriangles[cursor++] > 0) {
                        fanningVertex = cursor - 1;
                        break;
                    }
                }

            }

            fArUV.set(fArUVNew.subarray(0, nbFaces * 4));
            if (hasUV) fAr.set(fArNew.subarray(0, nbFaces * 4));

        }

        optimizePreTransform() {
            // pre transform optimization (not as important as post transform though)
            // it also removes unused vertices

            var vArOld = getVertices();
            var cArOld = getColors();
            var mArOld = getMaterials();
            var nbVertices = getNbVertices();

            var fAr = getFaces();
            var fArCount = getNbFaces() * 4;

            var vArNew = new Float32Array(nbVertices * 3);
            var cArNew = new Float32Array(nbVertices * 3);
            var mArNew = new Float32Array(nbVertices * 3);

            var idvPos = new Uint32Array(nbVertices);
            var acc = 0;
            var i = 0;
            for (i = 0; i < fArCount; ++i) {
                var iv = fAr[i];
                if (iv == = Utils.TRI_INDEX) continue;

                var tag = idvPos[iv] - 1;
                if (tag == = -1) {
                    var idNew = acc * 3;
                    var idOld = iv * 3;
                    vArNew[idNew] = vArOld[idOld];
                    vArNew[idNew + 1] = vArOld[idOld + 1];
                    vArNew[idNew + 2] = vArOld[idOld + 2];

                    cArNew[idNew] = cArOld[idOld];
                    cArNew[idNew + 1] = cArOld[idOld + 1];
                    cArNew[idNew + 2] = cArOld[idOld + 2];

                    mArNew[idNew] = mArOld[idOld];
                    mArNew[idNew + 1] = mArOld[idOld + 1];
                    mArNew[idNew + 2] = mArOld[idOld + 2];

                    tag = acc++;
                    idvPos[iv] = tag + 1;
                }

                fAr[i] = tag;
            }

            var nbUnusedVertex = nbVertices - acc;
            if (nbUnusedVertex > 0)
                setNbVertices(acc);

            // Only the unique "positoned" vertices are pre transform, because sculptgl 
            // requires the duplicate vertices to be after the uniques positioned vertices
            if (hasUV()) {
                var fArUV = getFacesTexCoord();
                // remap unique vertex i
                for (i = 0; i < fArCount; ++i) {
                    var iduv = fArUV[i];
                    if (iduv < nbVertices) fArUV[i] = idvPos[iduv] - 1;
                    else if (iduv != = Utils.TRI_INDEX) fArUV[i] -= nbUnusedVertex;
                }

                var nbUV = getNbTexCoords();
                var nbUVNew = getNbTexCoords() - nbUnusedVertex;

                var tAr = getTexCoords();
                var tArNew = new Float32Array(nbUVNew * 2);
                var dupUVNew = new Uint32Array(acc * 2);
                var dupUV = getVerticesDuplicateStartCount();

                for (i = 0; i < nbVertices; ++i) {
                    var i2 = i * 2;
                    var start = dupUV[i2];
                    var newiv = (idvPos[i] - 1) * 2;
                    if (newiv < 0) continue;

                    if (start > 0) {
                        dupUVNew[newiv] = start - nbUnusedVertex;
                        dupUVNew[newiv + 1] = dupUV[i2 + 1];
                    }

                    tArNew[newiv] = tAr[i2];
                    tArNew[newiv + 1] = tAr[i2 + 1];
                }

                for (i = nbVertices; i < nbUV; ++i) {
                    var ivd = i * 2;
                    var ivdnew = (i - nbUnusedVertex) * 2;
                    tArNew[ivdnew] = tAr[ivd];
                    tArNew[ivdnew + 1] = tAr[ivd + 1];
                }

                setVerticesDuplicateStartCount(dupUVNew);
                setTexCoords(tArNew);
            }

            vArOld.set(vArNew);
            cArOld.set(cArNew);
            mArOld.set(mArNew);

        }
    }

    Mesh.OPTIMIZE = true;
    Mesh.ID = 0;


}

#endif // mesh_h__