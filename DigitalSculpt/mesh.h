#ifndef mesh_h__
#define mesh_h__

#include "utils.h"
#include "mesh_data.h"

namespace DigitalSculpt
{
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

          //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> octree >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
          Uint32Array intersectRay(const glm::vec3& vNear, const glm::vec3& eyeDir, bool collectLeaves);

          /** Return faces inside a sphere */
          Uint32Array intersectSphere(const glm::vec3& vert, float radiusSquared, bool collectLeaves);


          /**
           * Update Octree
           * For each faces we check if its position inside the octree has changed
           * if so... we mark this face and we remove it from its former cells
           * We push back the marked faces into the octree
           */
          void updateOctree(const Uint32Array* iFaces);

          SixElemArray computeAabb();

          /** Compute the octree */
          void computeOctree();

          Uint32Array updateOctreeRemove(const Uint32Array* iFaces);

          void updateOctreeAdd(const Uint32Array* facesToMove);

          /** balance octree (cut empty leaves or go deeper if needed) */
          void balanceOctree();
          //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    public:


        MeshData* mesh_data_;
    };

    typedef boost::intrusive_ptr<Mesh> MeshPtr;
}

#endif // mesh_h__
