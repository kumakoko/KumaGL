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
/*!
* \file kgl_vertex_type.h
* \date 2017/12/31 14:02
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: 顶点格式定义
*
* \note
*/
#ifndef kgl_vertex_type_h__
#define kgl_vertex_type_h__

namespace kgl
{
    enum VertexType
    {
        VERTEX_TYPE_P,
        VERTEX_TYPE_PC,
        VERTEX_TYPE_PN,
        VERTEX_TYPE_PNTB,
        VERTEX_TYPE_PNTBT1,
        VERTEX_TYPE_PNTBT2,
        VERTEX_TYPE_PNTBT3,
        VERTEX_TYPE_PNTBT4,
        VERTEX_TYPE_PNT1,
        VERTEX_TYPE_PNT2,
        VERTEX_TYPE_PNT3,
        VERTEX_TYPE_PNT4,
        VERTEX_TYPE_PNBONET1
    };

    struct VertexP //position
    {
        glm::vec3 Position;
    };

    struct VertexPC : public VertexP //position color
    {
        glm::vec4 Color;
    };

    struct VertexPN : public VertexP // position normal
    {
        glm::vec3 Normal;
    };

    struct VertexPNTB : public VertexPN // position normal tangent binormal
    {
        glm::vec3 Tangent;
        glm::vec3 Binormal;
    };

    struct VertexPNTBT1 : public VertexPNTB // position normal tangent binormal texture_uv1
    {
        glm::vec2 TextureCoord1;
    };

    struct VertexPNTBT2 : public VertexPNTBT1
    {
        glm::vec2 TextureCoord2;
    };

    struct VertexPNTBT3 : public VertexPNTBT2
    {
        glm::vec2 TextureCoord3;
    };

    struct VertexPNTBT4 : public VertexPNTBT3
    {
        glm::vec2 TextureCoord4;
    };

    struct VertexPNT1 : public VertexPN // position normal texture1
    {
        glm::vec2 TextureCoord1;
    };

    struct VertexPNT2 : public VertexPNT1 // position normal texture2
    {
        glm::vec2 TextureCoord2;
    };

    struct VertexPNT3 : public VertexPNT2 // position normal texture2
    {
        glm::vec2 TextureCoord3;
    };

    struct VertexPNT4 : public VertexPNT3 // position normal texture2
    {
        glm::vec2 TextureCoord4;
    };

    struct VertexBoneData
    {
        /// <summary>
        /// 每个顶点受多少块骨骼控制
        /// </summary>
        static const int NUM_BONES_PER_VEREX = 4;
    
        /// <summary>
        /// 作用在本顶点上的骨块的ID
        /// </summary>
        uint32_t affectd_bone_id[NUM_BONES_PER_VEREX];

        /// <summary>
        /// 作用在本顶点的各个骨块所影响的权重比例
        /// </summary>
        float weights[NUM_BONES_PER_VEREX];

        /// <summary>
        /// 初始化一个<see cref="VertexBoneData"/>结构体实例对象.
        /// </summary>
        VertexBoneData();

        /// <summary>
        /// 清空本结构体的数据属性
        /// </summary>
        void Reset();

        /// <summary>
        /// 把ID为bone_id的骨块所携带的权重值，以及它本身的ID，存进本结构中
        /// </summary>
        /// <param name="BoneID">骨块的ID</param>
        /// <param name="Weight">骨块的权重</param>
        void AddBoneData(uint32_t bone_id, float weight);
    };

    struct VertexPNBoneT1
    {
        glm::vec3 position;
        glm::vec3 normal;
        VertexBoneData bone_data;
        glm::vec2 tex_coord_1;
    };
}

#endif // kgl_vertex_type_h__