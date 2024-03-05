#ifndef mesh_h__
#define mesh_h__

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "mesh/mesh_data.h"
#include "mesh/transform_data.h"
#include "mesh/render_data.h"
#include "misc/enums.h"
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

        inline RenderData* getRenderData()
        {
            return _renderData;
        }

        inline MeshData* getMeshData()
        {
            return _meshData;
        }

        inline TransformData* getTransformData()
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

        inline IUint32ArrayND* getVerticesRingVert()
        {
            return _meshData->_vertRingVert;
        }

        inline const IUint32ArrayND* getVerticesRingVert() const
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

        inline IUint32ArrayND* getVerticesRingFace()
        {
            return _meshData->_vertRingFace;
        }

        inline const IUint32ArrayND* getVerticesRingFace() const
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

        inline const Float32Array& getColorsDrawArrays() const {
            return _meshData->_DAcolorsRGB;
        }

        inline Float32Array& getMaterialsDrawArrays() {
            return _meshData->_DAmaterialsPBR;
        }

        inline const Float32Array& getMaterialsDrawArrays() const {
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

        inline const glm::mat4& getMV() const {
            return _transformData->_lastComputedMV;
        }

        inline glm::mat4& getMVP() {
            return _transformData->_lastComputedMVP;
        }

        inline glm::mat3& getN() {
            return _transformData->_lastComputedN;
        }

        inline const glm::mat3& getN() const {
            return _transformData->_lastComputedN;
        }

        inline glm::mat3& getEN() {
            return _transformData->_lastComputedEN;
        }

        inline const glm::mat3& getEN() const {
            return _transformData->_lastComputedEN;
        }

        inline float getDepth() const {
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

        inline const glm::mat4& getEditMatrix() const {
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

        inline std::vector<OctreeCell*>& getFaceLeaf()
        {
            return _meshData->_faceLeaf;
        }

        inline const std::vector<OctreeCell*>& getFaceLeaf() const
        {
            return _meshData->_faceLeaf;
        }

        inline std::vector<OctreeCell*>& getLeavesToUpdate()
        {
            return _meshData->_leavesToUpdate;
        }

        inline const std::vector<OctreeCell*>& getLeavesToUpdate() const
        {
            return _meshData->_leavesToUpdate;
        }

        inline SixElemArray& getLocalBound()
        {
            return _meshData->_octree->_aabbLoose;
        }

        inline const SixElemArray& getLocalBound() const
        {
            return _meshData->_octree->_aabbLoose;
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
        void updateVerticesNormal(const Uint32Array& iVerts);

        /** Computes vertex ring around vertices */
        void initVertexRings();

        /** Get more vertices (n-ring) */
        Uint32Array expandsVertices(const Uint32Array& iVerts, int nRing);

        /** Return all the vertices linked to a group of faces */
        Uint32Array getVerticesFromFaces(const Uint32Array& iFaces);

        void updateFacesAabbAndNormal(const Uint32Array& iFaces);

        /** Get more faces (n-ring) */
        Uint32Array expandsFaces(const Uint32Array& iFaces, std::uint32_t nRing);

        /** Return all the faces linked to a group of vertices */
        Uint32Array getFacesFromVertices(const Uint32Array& iVerts);

        /** Computes triangles */
        void initRenderTriangles();

        /** Computes triangles from faces */
        Uint32Array computeTrianglesFromFaces(const Uint32Array& faces);

        void initEdges();

        /** Return wireframe array (or compute it if not up to date) */
        Uint32Array& getWireframe();

        void updateDuplicateGeometry(const Uint32Array& iVerts);

        void updateDuplicateColorsAndMaterials(const Uint32Array& iVerts);

        void initTexCoordsDataFromOBJData(const Float32Array& uvAr, Uint32Array& uvfArOrig);

        void setAlreadyDrawArrays();

        /** Updates the arrays that are going to be used by webgl */
        void updateDrawArrays(Uint32Array* iFaces);

        /** Updates the UV array data for drawArrays */
        void updateDrawArraysTexCoord(Uint32Array* iFaces);

        /////////////////
        // TRANSFORM DATA
        /////////////////
        void updateCenter();

        /** Pre compute mv and mvp matrices as well as the depth center */
        void updateMatrices(const Camera& camera);

        float computeLocalRadius() const;

        void normalizeSize();

        SixElemArray& computeWorldBound();

        /////////
        // OCTREE
        /////////

        /******************************************************************************************************************
         * Desc: Return faces intersected by a ray
         * Method:    intersectRay
         * Returns:   DigitalSculpt::Uint32Array
         * Parameter: const glm::vec3 & vNear
         * Parameter: const glm::vec3 & eyeDir
         * Parameter: bool collectLeaves
         ****************************************************************************************************************/
        Uint32Array intersectRay(const glm::vec3& vNear, const glm::vec3& eyeDir, bool collectLeaves = false);

        /******************************************************************************************************************
         * Desc: Return faces inside a sphere
         * Method:    intersectSphere
         * Returns:   DigitalSculpt::Uint32Array
         * Parameter: const glm::vec3 & vert
         * Parameter: float radiusSquared
         * Parameter: bool collectLeaves
         ****************************************************************************************************************/
        Uint32Array intersectSphere(const glm::vec3& vert, float radiusSquared, bool collectLeaves = false);

        /******************************************************************************************************************
         * Desc: Update Octree For each faces we check if its position inside the octree has changed
         *       if so... we mark this face and we remove it from its former cells
         *       We push back the marked faces into the octree
         * Method:    updateOctree
         * Returns:   void
         * Parameter: const Uint32Array & iFaces
         ****************************************************************************************************************/
        void updateOctree(const Uint32Array& iFaces);

        /******************************************************************************************************************
         * Desc:
         * Method:    computeAabb
         * Returns:   DigitalSculpt::SixElemArray
         ****************************************************************************************************************/
        SixElemArray computeAabb();

        /******************************************************************************************************************
         * Desc: Compute the octree
         * Method:    computeOctree
         * Returns:   void
         ****************************************************************************************************************/
        void computeOctree();

        /******************************************************************************************************************
         * Desc:
         * Method:    updateOctreeRemove
         * Returns:   DigitalSculpt::Uint32Array
         * Parameter: const Uint32Array & iFaces
         ****************************************************************************************************************/
        Uint32Array Mesh::updateOctreeRemove(const Uint32Array& iFaces);

        /******************************************************************************************************************
         * Desc:
         * Method:    updateOctreeAdd
         * Returns:   void
         * Parameter: const Uint32Array & facesToMove
         ****************************************************************************************************************/
        void updateOctreeAdd(const Uint32Array& facesToMove);

        /******************************************************************************************************************
         * Desc: balance octree (cut empty leaves or go deeper if needed)
         * Method:    balanceOctree
         * Returns:   void
         ****************************************************************************************************************/
         void balanceOctree();

        //////////////
        // RENDER DATA
        //////////////
        inline void setFlatColor(val)
        {
            getFlatColor().set(val);
        }

        inline void setAlbedo(val)
        {
            getAlbedo().set(val);
        }

        inline void setMode(GLenum mode) 
        {
            _renderData->_mode = mode;
        }

        inline void setRoughness(float val)
        {
            _renderData->_roughness = val;
        }

        inline void setMetallic(float val)
        {
            _renderData->_metallic = val;
        }

        inline void setOpacity(float alpha)
        {
            _renderData->_alpha = alpha;
        }

        inline void setTexture0(kgl::TextureSPtr tex)
        {
            _renderData->_texture0 = tex;
        }

        inline void setMatcap(int idMat)
        {
            _renderData->_matcap = idMat;
        }

        inline void setCurvature(int cur) 
        {
            _renderData->_curvature = cur;
        }

        inline void setFlatShading(bool flatShading)
        {
            _renderData->_flatShading = flatShading;
        }

        void setShowWireframe(bool showWireframe);

        inline void setUseDrawArrays(bool b) 
        {
            _renderData->_useDrawArrays = b;
        }

        std::uint32_t getCount() const;

        inline Buffer* getVertexBuffer()
        {
            return _renderData->_vertexBuffer;
        }

        inline const Buffer* getVertexBuffer() const
        {
            return _renderData->_vertexBuffer;
        }

        inline Buffer* getNormalBuffer() 
        {
            return _renderData->_normalBuffer;
        }

        inline Buffer* getColorBuffer() 
        {
            return _renderData->_colorBuffer;
        }

        inline Buffer* getMaterialBuffer() 
        {
            return _renderData->_materialBuffer;
        }

        inline Buffer* getTexCoordBuffer() {
            return _renderData->_texCoordBuffer;
        }

        inline Buffer* getIndexBuffer() 
        {
            return _renderData->_indexBuffer;
        }

        inline Buffer* getWireframeBuffer()
        {
            return _renderData->_wireframeBuffer;
        }

        inline const Buffer* getNormalBuffer() const
        {
            return _renderData->_normalBuffer;
        }

        inline const Buffer* getColorBuffer() const
        {
            return _renderData->_colorBuffer;
        }

        inline const Buffer* getMaterialBuffer() const
        {
            return _renderData->_materialBuffer;
        }

        inline const Buffer* getTexCoordBuffer() const
        {
            return _renderData->_texCoordBuffer;
        }

        inline const Buffer* getIndexBuffer() const
        {
            return _renderData->_indexBuffer;
        }

        inline const Buffer* getWireframeBuffer() const 
        {
            return _renderData->_wireframeBuffer;
        }

        inline float getRoughness() const {
            return _renderData->_roughness;
        }

        inline float getMetallic() const {
            return _renderData->_metallic;
        }

        inline float getOpacity() const {
            return _renderData->_alpha;
        }

        inline glm::vec3& getFlatColor() 
        {
            return _renderData->_flatColor;
        }

        inline const glm::vec3& getFlatColor() const 
        {
            return _renderData->_flatColor;
        }

        inline GLenum getMode() const 
        {
            return _renderData->_mode;
        }

        inline glm::vec3& getAlbedo()
        {
            return _renderData->_albedo;
        }

        inline const glm::vec3& getAlbedo() const
        {
            return _renderData->_albedo;
        }

        inline kgl::TextureSPtr getTexture0()
        {
            return _renderData->_texture0;
        }

        inline int getMatcap() const
        {
            return _renderData->_matcap;
        }

        inline int getCurvature() const
        {
            return _renderData->_curvature;
        }

        inline bool getFlatShading() const
        {
            return _renderData->_flatShading;
        }

        inline bool getShowWireframe() const
        {
            return _renderData->_showWireframe;
        }

        inline bool isUsingDrawArrays() const{
            return _renderData->_useDrawArrays || RenderData::ONLY_DRAW_ARRAYS;
        }

        inline bool isUsingTexCoords() const
        {
            return _renderData->_shaderType == Enums::Shader::UV || _renderData->_shaderType == Enums::Shader::PAINTUV;
        }

        inline bool isTransparent() const {
            return _renderData->_alpha < 0.99;
        }

        inline int getShaderType() const
        {
            return _renderData->_shaderType;
        }

        void setShaderType(int shaderName);

        void initRender();

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
    };

    Mesh.OPTIMIZE = true;
    Mesh.ID = 0;


}

#endif // mesh_h__