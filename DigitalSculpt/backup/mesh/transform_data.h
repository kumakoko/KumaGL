//import { vec3, mat3, mat4 } from 'gl-matrix';
#ifndef transform_data_h__
#define transform_data_h__

#include <limits>
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
            _symmetryNormal = glm::vec3(1.0f, 0.0f, 0.0f);
            _center = glm::vec3(0.0f);
            _matrix = glm::identity<glm::mat4>();
            _editMatrix = glm::identity<glm::mat4>();
            _lastComputedMV = glm::identity<glm::mat4>();
            _lastComputedMVP = glm::identity<glm::mat4>();
            _lastComputedN = glm::identity<glm::mat3>();
            _lastComputedEN = glm::identity<glm::mat3>();
            _lastComputedDepth = 0.0f;
            _symmetryOffset = 0.0f;
            float infinity = std::numeric_limits<float>::infinity();

            _lastWorldBound[0] = _lastWorldBound[1] = _lastWorldBound[2] = infinity;
            _lastWorldBound[4] = _lastWorldBound[5] = _lastWorldBound[6] = -infinity;
        }
    };

    typedef std::shared_ptr<TransformData> TransformDataSPtr;
}


#endif // transform_data_h__
