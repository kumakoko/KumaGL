#ifndef digital_sculpt_mesh_static_h__
#define digital_sculpt_mesh_static_h__

#include "mesh/mesh.h"
#include "mesh/transform_data.h"
#include "mesh/mesh_data.h"
#include "mesh/render_data.h"

namespace DigitalSculpt
{
    class MeshStatic : public Mesh 
    {
    public:
        MeshStatic()
        {
            this->_id = Mesh::ID++; // useful id to retrieve a mesh (dynamic mesh, multires mesh, voxel mesh)

            this->_renderData = new RenderData(this);
            this->_meshData = new MeshData();
            this->_transformData = new TransformData();
        }
    };
}
#endif // digital_sculpt_mesh_static_h__
