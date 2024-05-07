#ifndef mesh_h__
#define mesh_h__

#include "utils.h"
#include "mesh_data.h"

namespace DigitalSculpt
{
    class OctreeCell;

    typedef boost::intrusive_ptr<OctreeCell> OctreeCellPtr;

    class Mesh
    {
    public:
        Mesh();

        virtual ~Mesh();
        
        /******************************************************************************************************************
         * Desc: 总初始化入口函数
         * Method:    Init
         * Returns:   void
         ****************************************************************************************************************/
         void Init();
    private:
        /******************************************************************************************************************
         * Desc: 
         * Method:    InitTopology
         * Returns:   void
         ****************************************************************************************************************/
         void InitTopology();

         /******************************************************************************************************************
          * Desc: 
          * Method:    UpdateGeometry
          * Returns:   void
          * Parameter: Uint32Array * iFaces
          * Parameter: Uint32Array * iVerts
          ****************************************************************************************************************/
          void UpdateGeometry(Uint32Array* iFaces, Uint32Array* iVerts);

         /******************************************************************************************************************
          * Desc: 
          * Method:    updateFacesAabbAndNormal
          * Returns:   void
          * Parameter: Uint32Array * iFaces
          ****************************************************************************************************************/
          void updateFacesAabbAndNormal(Uint32Array* iFaces);

         /******************************************************************************************************************
          * Desc: 
          * Method:    updateVerticesNormal
          * Returns:   void
          * Parameter: const Uint32Array * iVerts
          ****************************************************************************************************************/
          void updateVerticesNormal(const Uint32Array* iVerts);

          void initFaceRings();
    public:
        Float32Array face_boxes_;
        Vec3Array face_centers_;
        Uint32Array face_pos_in_leaf_;
        std::vector<OctreeCellPtr> face_leaves_;


        MeshData* mesh_data_;
    };

    typedef boost::intrusive_ptr<Mesh> MeshPtr;
}

#endif // mesh_h__
