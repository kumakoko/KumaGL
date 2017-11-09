// 描述了顶点属性的详细信息的数据结构体
#include "kgl_lib_pch.h"
#include "kgl_vertex_attribute.h"

namespace kgl
{
    VertexAttribute::VertexAttribute()
    {
    }

    VertexAttribute::VertexAttribute(GLuint index, GLboolean normalized, GLenum type, GLint size, GLsizei stride, const GLvoid* pointer)
    {
        this->size = size;
        this->index = index;
        this->normalized = normalized;
        this->type = type;
        this->stride = stride;
        this->pointer = pointer;
    }
}