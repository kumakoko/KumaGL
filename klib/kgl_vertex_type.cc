// 顶点格式定义
#include "kgl_lib_pch.h"
#include "kgl_vertex_type.h"

namespace kgl
{
    VertexBoneData::VertexBoneData()
    {
        Reset();
    };

    void VertexBoneData::Reset()
    {
        memset(affectd_bone_id, 0, sizeof(uint32_t)*VertexBoneData::NUM_BONES_PER_VEREX);
        memset(weights, 0, sizeof(float)*VertexBoneData::NUM_BONES_PER_VEREX);
    }

    void VertexBoneData::AddBoneData(uint32_t bone_id, float weight)
    {
        for (uint32_t i = 0; i < VertexBoneData::NUM_BONES_PER_VEREX; i++)
        {
            if (weights[i] == 0.0) {
                affectd_bone_id[i] = bone_id;
                weights[i] = weight;
                return;
            }
        }

    }

}