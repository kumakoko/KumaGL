// 顶点格式定义
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
        glm::vec3 position;
    };

    struct VertexPC : public VertexP //position color
    {
        glm::vec4 color;
    };

    struct VertexPN : public VertexP // position normal
    {
        glm::vec3 normal;
    };

    struct VertexPNTB : public VertexPN // position normal tangent binormal
    {
        glm::vec3 tangent;
        glm::vec3 binormal;
    };

    struct VertexPNTBT1 : public VertexPNTB // position normal tangent binormal texture_uv1
    {
        glm::vec2 tex_coord_1;
    };

    struct VertexPNTBT2 : public VertexPNTBT1
    {
        glm::vec2 tex_coord_2;
    };

    struct VertexPNTBT3 : public VertexPNTBT2
    {
        glm::vec2 tex_coord_3;
    };

    struct VertexPNTBT4 : public VertexPNTBT3
    {
        glm::vec2 tex_coord_4;
    };

    struct VertexPNT1 : public VertexPN // position normal texture1
    {
        glm::vec2 tex_coord_1;
    };

    struct VertexPNT2 : public VertexPNT1 // position normal texture2
    {
        glm::vec2 tex_coord_2;
    };

    struct VertexPNT3 : public VertexPNT2 // position normal texture2
    {
        glm::vec2 tex_coord_3;
    };

    struct VertexPNT4 : public VertexPNT3 // position normal texture2
    {
        glm::vec2 tex_coord_4;
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