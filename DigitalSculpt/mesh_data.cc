#include "mesh_data.h"
#include "kgl_lib_pch.h"
#include "kgl_basic_static_mesh.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_texture_manager.h"

namespace DigitalSculpt
{
    bool MeshData::LoadMesh(const std::string& file_name)
    {
        // 清空上次的残留
        Clear();

        // 创建vertex attribute object和6个顶点属性buffer
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);
        glGenBuffers(6, vertex_attribute_buffers_);

        bool ret = false;
        Assimp::Importer asset_importer;

        const aiScene* asset_scene = asset_importer.ReadFile(file_name.c_str(),
            aiProcess_Triangulate | aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

        if (asset_scene)
        {
            ret = InitFromAssetScene(asset_scene, file_name);
        }
        else
        {
            std::wstringstream wss;
            wss << L"Error parsing: "
                << kgl::StringConvertor::ANSItoUTF16LE(file_name.c_str()) << std::endl
                << kgl::StringConvertor::ANSItoUTF16LE(asset_importer.GetErrorString());
            throw kgl::Error(wss.str(), __FILE__, __LINE__);
        }

        glBindVertexArray(0);
        return ret;
    }

    bool MeshData::InitFromAssetScene(const aiScene* scene, const std::string& file_name)
    {
        entries_.resize(scene->mNumMeshes);
        //textures_.resize(scene->mNumMaterials);

        unsigned int vertices_count = 0;
        unsigned int indices_count = 0;

        // 计算出本mesh中的顶点和顶点索引的数量
        for (unsigned int i = 0; i < entries_.size(); i++)
        {
            entries_[i].MaterialIndex = scene->mMeshes[i]->mMaterialIndex;
            entries_[i].NumIndices = scene->mMeshes[i]->mNumFaces * 3;
            entries_[i].BaseVertex = vertices_count;
            entries_[i].BaseIndex = indices_count;
            vertices_count += scene->mMeshes[i]->mNumVertices;
            indices_count += entries_[i].NumIndices;
        }

        // 根据各项属性的数目设置vector的实际size，一点都不浪费
        positions_.reserve(vertices_count);
        normals_.reserve(vertices_count);
        texture_coords_.reserve(vertices_count);
        indices_.reserve(indices_count);

        // 依次取出每个mesh然后初始化之
        for (unsigned int i = 0; i < entries_.size(); i++)
        {
            const aiMesh* paiMesh = scene->mMeshes[i];
            InitMesh(paiMesh);
        }

        /*if (!InitMaterials(scene, file_name))
        {
            return false;
        }*/

        // 顶点的position，texture coord1，normal分别在layout的0，1，2中
        
        glBindBuffer(GL_ARRAY_BUFFER, vertex_attribute_buffers_[POS_VB]);
        glBufferData(GL_ARRAY_BUFFER, /*sizeof(positions_[0])*/sizeof(glm::vec3) * positions_.size(), positions_.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(POSITION_LOCATION);
        glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_attribute_buffers_[TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texture_coords_.size(), texture_coords_.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(TEX_COORD_LOCATION);
        glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_attribute_buffers_[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals_[0]) * normals_.size(), normals_.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(NORMAL_LOCATION);
        glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_attribute_buffers_[INDEX_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_[0]) * indices_.size(), indices_.data(), GL_STATIC_DRAW);

        return true;// GLCheckError();
    }

    void MeshData::InitMesh(const aiMesh* mesh)
    {
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            const aiVector3D& pos = mesh->mVertices[i];
            const aiVector3D& normal = mesh->mNormals[i];
            const aiVector3D& texture_coord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : Zero3D;

            positions_.emplace_back(glm::vec3(pos.x, pos.y, pos.z));
            normals_.emplace_back(glm::vec3(normal.x, normal.y, normal.z));
            texture_coords_.emplace_back(glm::vec2(texture_coord.x, texture_coord.y));
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace& face = mesh->mFaces[i];
            assert(face.mNumIndices == 3);
            indices_.emplace_back(face.mIndices[0]);
            indices_.emplace_back(face.mIndices[1]);
            indices_.emplace_back(face.mIndices[2]);
        }
    }

    void MeshData::Clear()
    {/*
        for (unsigned int i = 0; i < textures_.size(); i++)
        {
            textures_[i].reset();
        }*/

        if (vertex_attribute_buffers_[0] != 0)
        {
            glDeleteBuffers(6, vertex_attribute_buffers_);
        }

        if (vao_ != 0)
        {
            glDeleteVertexArrays(1, &vao_);
            vao_ = 0;
        }
    }

    void MeshData::Render()
    {
        glBindVertexArray(vao_);

        for (size_t i = 0; i < entries_.size(); i++)
        {
        //    const size_t material_index = entries_[i].MaterialIndex;
        //    assert(material_index < textures_.size());

            /*
            if (textures_[material_index])
            {
                textures_[material_index]->ActiveBind(0); // 使用第一个纹理单元
            }
            */
            glDrawElementsBaseVertex(GL_TRIANGLES,
                entries_[i].NumIndices,
                GL_UNSIGNED_INT,
                (void*)(sizeof(unsigned int) * entries_[i].BaseIndex),
                entries_[i].BaseVertex);
        }

        glBindVertexArray(0);
    }

}