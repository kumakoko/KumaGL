//import { vec3, mat3, mat4 } from 'gl-matrix';
#ifndef transform_data_h__
#define transform_data_h__

#include <memory>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace DigitalSculpt
{
    class TransformData
    {
    public:
        glm::vec3 _symmetryNormal;//:;// [1.0, 0.0, 0.0], // symmetry normal
        glm::vec3 _center;// : vec3.create(), // center of the mesh (local space, before transformation)
        glm::mat4 _matrix;//: mat4.create();//, // transformation matrix of the mesh
        glm::mat4 _editMatrix;//: mat4.create();//, // edit matrix
        // the model-view and model-view-projection and normal matrices 
        // are computed at the beginning of each frame (after camera update)
        glm::mat4 _lastComputedMV;//:;// mat4.create(), // MV matrix
        glm::mat4 _lastComputedMVP;//:;// mat4.create(), // MVP matrix
        glm::mat3 _lastComputedN;//:;// mat3.create(), // N matrix
        glm::mat3 _lastComputedEN;//: mat3.create(), // Editmatrix N matrix
        float _lastComputedDepth;//: 0.0, // depth of center
        float _symmetryOffset;//:;// 0.0, // offset
        float _lastWorldBound[6];//: [Infinity, Infinity, Infinity, -Infinity, -Infinity, -Infinity]
    public:
        TransformData()
        {

        }
    };

    typedef std::shared_ptr<TransformData> TransformDataSPtr;
}


#endif // transform_data_h__
