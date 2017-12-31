/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
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