// 网格类基类
#include "kgl_lib_pch.h"
#include "kgl_skinned_mesh.h"
#include "kgl_defines.h"
#include "kgl_debug_tool.h"

namespace kgl
{
    SkinnedMesh::SkinnedMesh()
    {
    }

    SkinnedMesh::~SkinnedMesh()
    {
        this->Clear();
    }

    void SkinnedMesh::Clear()
    {
        size_t sz = texture_array_.size();

        for (size_t t = 0; t < sz; ++t)
        {
            texture_array_[t].reset();
        }

        texture_array_.clear();

        KGL_SAFE_DELETE_ARRAY(vertex_data_);
        vertex_byte_count_ = 0;

        KGL_SAFE_DELETE_ARRAY(index_data_);
        index_byte_count_ = 0;

        vertex_byte_count_ = 0;
        index_byte_count_ = 0;

        vao_ = 0;
        vbo_ = 0;
        ebo_ = 0;

        shader_.reset();
    }

    void SkinnedMesh::Setup(VertexType vertex_type, const unsigned char* vertex_data, uint32_t vertex_byte_count, const uint32_t* index_data, uint32_t index_byte_count, const std::vector<TextureSPtr>& texture_array, const glm::mat4& local_transform_matrix)
    {
        this->Clear();

        vertex_data_ = new uint8_t[vertex_byte_count];
        memcpy(vertex_data_, vertex_data, vertex_byte_count);

        index_data_ = new uint32_t[index_byte_count / 4];
        memcpy(index_data_, index_data, index_byte_count);

        size_t sz = texture_array.size();

        for (size_t t = 0; t < sz; ++t)
        {
            texture_array_.push_back(texture_array[t]);
        }

        vertex_byte_count_ = vertex_byte_count;
        index_byte_count_ = index_byte_count;
        vertex_type_ = vertex_type;
        // ------------------------------------------------------------------------

        // 创建对应的buffer
        glGenVertexArrays(1, &this->vao_);
        glGenBuffers(1, &this->vbo_);
        glGenBuffers(1, &this->ebo_);

        // 装载数据到顶点缓冲区
        glBindVertexArray(this->vao_);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
        glBufferData(GL_ARRAY_BUFFER, vertex_byte_count_, vertex_data_, GL_STATIC_DRAW);

        // 装载数据到索引缓冲区
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_byte_count_, index_data_, GL_STATIC_DRAW);

        this->SetupVertexAttributeBuffer(vertex_type_);

        glBindVertexArray(0); // 解除vertex attribute array的绑定

        // 记录下本mesh用到的纹理贴图
        texture_array_ = texture_array;

        local_transform_matrix_ = local_transform_matrix;
#if defined(DEBUG) || defined(_DEBUG)
        DebugTool::PrintGLMMatrix("local transform matrix", local_transform_matrix_);
#endif
    }

    void SkinnedMesh::Draw()
    {
        shader_->Use();
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, index_byte_count_ / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void SkinnedMesh::SetupVertexAttributeBufferPNBoneT1()
    {
        size_t offset_sz = 0;
        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNBoneT1), (GLvoid*)(offset_sz));

        offset_sz += sizeof(glm::vec3);
        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNBoneT1), (GLvoid*)(offset_sz));

        offset_sz += sizeof(glm::vec3);
        glEnableVertexAttribArray(2); // bone data
        glVertexAttribPointer(2, sizeof(VertexBoneData), GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VertexPNBoneT1), (GLvoid*)(offset_sz));

        offset_sz += sizeof(VertexBoneData);
        glEnableVertexAttribArray(3); // texture coord 1
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNBoneT1), (GLvoid*)(offset_sz));
    }
}