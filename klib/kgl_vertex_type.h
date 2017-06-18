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
        VERTEX_TYPE_PNT4
    };

    struct VertexP //position
    {
        glm::vec3 P;
    };

    struct VertexPC : public VertexP //position color
    {
        glm::vec4 C;
    };

    struct VertexPN : public VertexP // position normal
    {
        glm::vec3 N;
    };

    struct VertexPNTB : public VertexPN // position normal tangent binormal
    {
        glm::vec3 T;
        glm::vec3 B;
    };

    struct VertexPNTBT1 : public VertexPNTB
    {
        glm::vec2 T1;
    };

    struct VertexPNTBT2 : public VertexPNTBT1
    {
        glm::vec2 T2;
    };

    struct VertexPNTBT3 : public VertexPNTBT2
    {
        glm::vec2 T3;
    };

    struct VertexPNTBT4 : public VertexPNTBT3
    {
        glm::vec2 T4;
    };

    struct VertexPNT1 : public VertexPN // position normal texture1
    {
        glm::vec2 T1;
    };

    struct VertexPNT2 : public VertexPNT1 // position normal texture2
    {
        glm::vec2 T2;
    };

    struct VertexPNT3 : public VertexPNT2 // position normal texture2
    {
        glm::vec2 T3;
    };

    struct VertexPNT4 : public VertexPNT3 // position normal texture2
    {
        glm::vec2 T4;
    };
}

#endif // kgl_vertex_type_h__