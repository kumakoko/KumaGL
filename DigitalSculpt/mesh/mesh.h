#ifndef mesh_h__
#define mesh_h__

#include <memory>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "mesh/mesh_data.h"
#include "mesh/transform_data.h"
#include "mesh/render_data.h"
#include "mesh/cache_store_info.h"
#include "misc/enums.h"
#include "math3d/camera.h"
#include "scene.h"

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
        static bool OPTIMIZE;
        static int ID;

    protected:
        std::int32_t _id;// = Mesh.ID++; // useful id to retrieve a mesh (dynamic mesh, multires mesh, voxel mesh)
        MeshData* _meshData = null;
        TransformData* _transformData = null;
        RenderData* _renderData = null;
        bool _isVisible = true;

    public:
        Mesh()
        {
            _id = Mesh::ID++; // useful id to retrieve a mesh (dynamic mesh, multires mesh, voxel mesh)
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

        inline void setMeshData(MeshData* mdata)
        {
            _meshData = mdata;
        }

        inline void setRenderData(RenderData* rdata)
        {
            _renderData = rdata;
        }

        inline void setTransformData(TransformData* tdata)
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

        inline OctreeCell* getOctree()
        {
            return _meshData->_octree;
        }

        inline const OctreeCell* getOctree() const
        {
            return _meshData->_octree;
        }

        inline glm::vec3& getCenter()
        {
            return _transformData->_center;
        }

        inline const glm::vec3& getCenter() const
        {
            return _transformData->_center;
        }

        inline glm::mat4& getMV() 
        {
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

        inline const glm::mat3& getEN() const 
        {
            return _transformData->_lastComputedEN;
        }

        inline float getDepth() const 
        {
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

        inline glm::mat4& getEditMatrix()
        {
            return _transformData->_editMatrix;
        }

        inline const glm::mat4& getEditMatrix() const 
        {
            return _transformData->_editMatrix;
        }

        inline glm::mat4& getEditMatrix()
        {
            return _transformData->_editMatrix;
        }

        inline const glm::mat4& getEditMatrix() const 
        {
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

        /******************************************************************************************************************
         * Desc: 
         * Method:    updateGeometry
         * Returns:   void
         * Parameter: Uint32Array * iFaces
         * Parameter: Uint32Array * iVerts
         ****************************************************************************************************************/
         void updateGeometry(Uint32Array* iFaces, Uint32Array* iVerts);

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
        /******************************************************************************************************************
         * Desc: 
         * Method:    updateCenter
         * Returns:   void
         ****************************************************************************************************************/
         void updateCenter();

        /******************************************************************************************************************
         * Desc: Pre compute mv and mvp matrices as well as the depth center
         * Method:    updateMatrices
         * Returns:   void
         * Parameter: const Camera & camera
         ****************************************************************************************************************/
         void updateMatrices(const Camera& camera);

        /******************************************************************************************************************
         * Desc: 
         * Method:    computeLocalRadius
         * Returns:   float
         ****************************************************************************************************************/
         float computeLocalRadius() const;

        /******************************************************************************************************************
         * Desc: 
         * Method:    normalizeSize
         * Returns:   void
         ****************************************************************************************************************/
         void normalizeSize();

        /******************************************************************************************************************
         * Desc: 
         * Method:    computeWorldBound
         * Returns:   DigitalSculpt::SixElemArray&
         ****************************************************************************************************************/
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
        inline void setFlatColor(const glm::vec3& val)
        {
            _renderData->_flatColor = val;// getFlatColor().set(val);
        }

        inline void setAlbedo(const glm::vec3& val)
        {
            _renderData->_albedo = val;//getAlbedo().set(val);
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

        /***************************** render相关的模块 begin *****************************/
        void render(Scene* main);

        void renderWireframe(Scene* main);

        void renderFlatColor(Scene* main);
        /***************************** render相关的模块 end *****************************/

        /***************************** update buffer相关的模块 begin *****************************/
        std::uint32_t getRenderNbVertices();

        void updateVertexBuffer();

        void updateNormalBuffer();

        void updateColorBuffer();

        void updateMaterialBuffer();

        void updateTexCoordBuffer();

        void updateIndexBuffer();

        void updateWireframeBuffer();

        void updateGeometryBuffers();

        void updateBuffers();

        void release();

        void copyRenderConfig(Mesh* mesh);

        void copyTransformData(Mesh* mesh);

        void copyData(Mesh* mesh);

        void optimize();

        CacheStoreInfo computeCacheScore();

        void optimizePostTransform();

        void optimizePreTransform();
        /***************************** update buffer相关的模块 begin *****************************/
    };

    typedef std::shared_ptr<Mesh> MeshSPtr;
}

#endif // mesh_h__