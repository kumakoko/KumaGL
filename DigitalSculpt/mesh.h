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

         void UpdateGeometry(Uint32Array* iFaces, Uint32Array* iVerts);

         void updateFacesAabbAndNormal(Uint32Array* iFaces);
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
