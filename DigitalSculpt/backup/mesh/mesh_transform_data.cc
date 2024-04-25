#include <utility>
#include <iterator>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" // 引入变换函数
#include "glm/gtc/type_ptr.hpp" // 引入变换函数
#include "mesh.h"
#include "misc/array_1d.h"
#include "misc/array_2d.h"


namespace DigitalSculpt
{
    void Mesh::updateCenter()
    {
        auto box = getLocalBound();
        _transformData->_center.x = (box[0] + box[3]) * 0.5f;
        _transformData->_center.y = (box[1] + box[4]) * 0.5f;
        _transformData->_center.z = (box[2] + box[5]) * 0.5f;
        //vec3.set(_transformData->_center, (box[0] + box[3]) * 0.5, (box[1] + box[4]) * 0.5, (box[2] + box[5]) * 0.5);
    }

    /** Pre compute mv and mvp matrices as well as the depth center */
    void Mesh::updateMatrices(const Camera& camera)
    {
        Utils::normalFromMat4(_transformData->_lastComputedEN, _transformData->_editMatrix);
        _transformData->_lastComputedMV = camera.getView() * _transformData->_matrix;//mat4.mul(_transformData->_lastComputedMV, camera.getView(), _transformData->_matrix);
        Utils::normalFromMat4(_transformData->_lastComputedN, _transformData->_lastComputedMV);
        _transformData->_lastComputedMVP = camera.getProjection() * _transformData->_lastComputedMV;//mat4.mul(_transformData->_lastComputedMVP, camera.getProjection(), _transformData->_lastComputedMV);
        const float* cen = glm::value_ptr(_transformData->_center);
        const float* m = glm::value_ptr(_transformData->_lastComputedMVP);
        _transformData->_lastComputedDepth = m[2] * cen[0] + m[6] * cen[1] + m[10] * cen[2] + m[14];
    }

    float Mesh::computeLocalRadius() const
    {
        auto box = getLocalBound();
        //return 0.5 * vec3.dist([box[0], box[1], box[2]], [box[3], box[4], box[5]]);
        return 0.5f * glm::distance(glm::vec3(box[0], box[1], box[2]), glm::vec3(box[3], box[4], box[5]));
    }

    void Mesh::normalizeSize()
    {
        float scale = Utils::SCALE / (2.0f * computeLocalRadius());
        //mat4.scale(_transformData->_matrix, _transformData->_matrix, [scale, scale, scale]);
        _transformData->_matrix = glm::scale(_transformData->_matrix, glm::vec3(scale));
    }

    std::array<float, 6>& Mesh::computeWorldBound()
    {
        //var worldb = _meshData->_worldBound;
        const std::array<float, 6>& localb = getLocalBound();
        const float* mat = glm::value_ptr(getMatrix());

        // trans
        _meshData->_worldBound[0] = _meshData->_worldBound[3] = mat[12];
        _meshData->_worldBound[1] = _meshData->_worldBound[4] = mat[13];
        _meshData->_worldBound[2] = _meshData->_worldBound[5] = mat[14];

        // rotate per component
        for (int i = 0; i < 3; ++i)
        {
            int i4 = i * 4;
            float mini = localb[i];
            float maxi = localb[i + 3];

            for (int j = 0; j < 3; ++j)
            {
                float cm = mat[i4 + j];
                float a = cm * maxi;
                float b = cm * mini;

                if (a < b)
                {
                    _meshData->_worldBound[j + 0] += a;
                    _meshData->_worldBound[j + 3] += b;
                }
                else
                {
                    _meshData->_worldBound[j + 0] += b;
                    _meshData->_worldBound[j + 3] += a;
                }
            }
        }

        return _meshData->_worldBound;
    }
}