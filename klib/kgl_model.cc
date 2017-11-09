// 模型类的基类
#include "kgl_lib_pch.h"
#include "kgl_model.h"
#include "kgl_string_convertor.h"
#include "kgl_error.h"
#include "kgl_texture_manager.h"
#include "kgl_defines.h"
#include "kgl_debug_tool.h"
#include "kgl_assimp_bridge.h"

#if defined(__APPLE__) && defined(__MACH__)
#include "SOIL.h"
#endif

namespace kgl
{
    Model::Model(VertexType vt, const char* path)
    {
        vertex_type_ = vt;
        this->LoadModel(std::string(path));
    }

    void Model::Draw()
    {
        for (GLuint i = 0; i < meshes_.size(); i++)
            meshes_[i]->Draw();
    }

    void Model::ApplyShaderToMesh(std::size_t index, GPUProgramSPtr shader)
    {
        if (index >= meshes_.size())
        {
            throw Error(L"The index is out of range", __FILE__, __LINE__);
        }

        meshes_[index]->SetShader(shader);
    }

    void Model::ApplyShaderToModel(GPUProgramSPtr shader)
    {
        for (GLuint i = 0; i < meshes_.size(); i++)
            meshes_[i]->SetShader(shader);
    }

    GPUProgramSPtr Model::GetMeshShader(std::size_t index)
    {
        if (index >= meshes_.size())
        {
            throw Error(L"The index is out of range", __FILE__, __LINE__);
        }

        return meshes_[index]->GetShader();
    }

    void Model::LoadModel(const std::string& path)
    {
    }

    void Model::ProcessNode(const aiScene* scene, const aiNode* node, const glm::mat4& parent_transform)
    {
        // 得到本节点相对于父空间的transform matrix
        glm::mat4 node_transform;
        AiMatrix4x4ToGlmMat4(node->mTransformation, node_transform);

        // 递归算出本节点相对于世界坐标的transform matrix
        glm::mat4 global_transform = parent_transform * node_transform;

#if defined(DEBUG) || defined(_DEBUG)
        DebugTool::PrintGLMMatrix("Node Transform ", node_transform);
        DebugTool::PrintGLMMatrix("Parents Transform ", parent_transform);
        DebugTool::PrintGLMMatrix("Global Transform ", global_transform);
#endif

        for (GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
#if defined(DEBUG) || defined(_DEBUG)
            boost::format f("=====>>> Process Mesh -- Mesh name: %s\n");
            f % mesh->mName.C_Str();
            DebugTool::OutputDebugMessage(f.str().c_str());
#endif
            this->meshes_.push_back(this->ProcessMesh(i,scene, mesh, global_transform));
        }

        // 遞歸處理
        for (GLuint i = 0; i < node->mNumChildren; i++)
        {
#if defined(DEBUG) || defined(_DEBUG)
            boost::format f("=====>>> Process Node -- Node name: %s\n");
            f % node->mName.C_Str();
            DebugTool::OutputDebugMessage(f.str().c_str());
#endif
            this->ProcessNode(scene, node->mChildren[i], global_transform);
        }
    }

    uint8_t* Model::ReadVertexData(VertexType vt, uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count)
    {
        switch (vt)
        {
        case VERTEX_TYPE_P: return this->ReadVertexDataP(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PC: return this->ReadVertexDataPC(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PN: return this->ReadVertexDataPN(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PNTB: return this->ReadVertexDataPNTB(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PNTBT1: return this->ReadVertexDataPNTBT1(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PNTBT2: return this->ReadVertexDataPNTBT2(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PNTBT3: return this->ReadVertexDataPNTBT3(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PNTBT4: return this->ReadVertexDataPNTBT4(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PNT1: return this->ReadVertexDataPNT1(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PNT2: return this->ReadVertexDataPNT2(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PNT3: return this->ReadVertexDataPNT3(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PNT4: return this->ReadVertexDataPNT3(mesh_index, mesh, vertex_byte_count);
        case VERTEX_TYPE_PNBONET1:return this->ReadVertexDataPNBoneT1(mesh_index, mesh, vertex_byte_count);
        default:return nullptr;
        }
    }
    uint8_t* Model::ReadVertexDataPNT1(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataPNT2(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataPNT3(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataPNT4(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataPNBoneT1(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataP(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataPC(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataPN(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){return nullptr;}
    uint8_t* Model::ReadVertexDataPNTB(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataPNTBT1(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataPNTBT2(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataPNTBT3(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* Model::ReadVertexDataPNTBT4(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }

    void Model::CheckHasPositions(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line)
    {
        if (!mesh->HasPositions())
        {
            boost::wformat f(L"The desired vertex type is %s,but the source mesh does not contain position");
            f % vertex_type;
            throw Error(f.str(), file, line);
            return;
        }
    }

    void Model::CheckHasNormals(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line)
    {
        if (!mesh->HasNormals())
        {
            boost::wformat f(L"The desired vertex type is %s,but the source mesh does not contain normal");
            f % vertex_type;
            throw Error(f.str(), file, line);
            return;
        }
    }

    void Model::CheckTangentAndBinormals(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line)
    {
        if (!mesh->HasTangentsAndBitangents())
        {
            boost::wformat f(L"The desired vertex type is %s,but the source mesh does not contain tangent and binormal");
            f % vertex_type;
            throw Error(f.str(), file, line);
            return;
        }
    }

    void Model::CheckTextureCoords(const aiMesh* mesh, const wchar_t* vertex_type, uint32_t texture_index, const char* file, uint32_t line)
    {
        if (!mesh->HasTextureCoords(texture_index))
        {
            boost::wformat f(L"The desired vertex type is %s,but the source mesh does not contain texture coordinate %d");
            f % vertex_type % texture_index;
            throw Error(f.str(), file, line);
            return;
        }
    }

    std::vector<TextureSPtr> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name, const TextureParams& params)
    {
        std::vector<TextureSPtr> textures;
        std::string std_string;
        TextureManager* tm = KTextureManager::GetInstance();

        // 取得待处理的材质中的的纹理数
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            std_string.clear();

            aiString str;
            aiTextureMapping mapping;
            unsigned int uvindex = 0;
            float blend = 0.0f;
            aiTextureOp op;
            aiTextureMapMode mapmode;
            TextureParams texture_param;

            mat->GetTexture(type, i, &str, &mapping, &uvindex, &blend, &op, &mapmode);

            switch (mapping)
            {
            case aiTextureMapping_UV:
                break;
            case aiTextureMapping_SPHERE:
            case aiTextureMapping_CYLINDER:
            case aiTextureMapping_BOX:
            case aiTextureMapping_PLANE:
            case aiTextureMapping_OTHER:
            default:throw Error(L"Not support without aiTextureMapping_UV", __FILE__, __LINE__);
                break;
            }

            // 纹理混合方式
            switch (op)
            {
            case aiTextureOp_Multiply:break; /** T = T1 * T2 */
            case aiTextureOp_Add:break; /** T = T1 + T2 */
            case aiTextureOp_Subtract:break; /** T = T1 - T2 */
            case aiTextureOp_Divide:break;  /** T = T1 / T2 */
            case aiTextureOp_SmoothAdd:break; /** T = (T1 + T2) - (T1 * T2) */
            case aiTextureOp_SignedAdd:break; /** T = T1 + (T2-0.5) */
            }

            switch (mapmode)
            {
            case aiTextureMapMode_Wrap:
                texture_param.wrap_s_mode = GL_TEXTURE_WRAP_S;
                texture_param.wrap_t_mode = GL_TEXTURE_WRAP_T;
                break;
            case aiTextureMapMode_Clamp:
                texture_param.wrap_s_mode = GL_CLAMP;
                texture_param.wrap_t_mode = GL_CLAMP;
                break;
            case aiTextureMapMode_Decal:break;
            case aiTextureMapMode_Mirror:break;
            }

            texture_param.mag_filter_mode = GL_LINEAR;
            texture_param.min_filter_mode = GL_LINEAR;
            texture_param.internal_format = GL_RGBA;
            texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
            texture_param.src_img_format = GL_RGBA;
            texture_param.used_mipmap = false;

            std_string.append("resources/model/textures/").append(str.C_Str()).append("png");

            if (tm->IsTextureExist(std_string))
                continue;

            TextureSPtr t = tm->CreateTextureFromFile(std_string, SOURCE_2D_TEXTURE, params);
            textures.push_back(t);
        }

        return textures;
    }

    void Model::ApplyTextureToMesh(std::size_t mesh_index, TextureSPtr texture, const char* uniform_var_name, GLuint slot_index)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyTexture(texture, uniform_var_name, slot_index);
    }

    void Model::ApplyTextureToModel(TextureSPtr texture, const char* uniform_var_name, GLuint slot_index)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyTexture(texture, uniform_var_name, slot_index);
        }
    }

    void Model::ApplyMatrixToMesh(std::size_t mesh_index, const GLfloat* matrix_data_pointer, const char* uniform_var_name, bool need_transpose)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyMatrix(matrix_data_pointer, uniform_var_name, need_transpose);
    }

    void Model::ApplyMatrixToModel(const GLfloat* matrix_data_pointer, const char* uniform_var_name, bool need_transpose)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyMatrix(matrix_data_pointer, uniform_var_name, need_transpose);
        }
    }

    void Model::ApplyFloatToMesh(std::size_t mesh_index, GLfloat float_data, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyFloat(float_data, uniform_var_name);
    }

    void Model::ApplyFloatToModel(GLfloat float_data, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyFloat(float_data, uniform_var_name);
        }
    }

    void Model::ApplyVector2ToMesh(std::size_t mesh_index, const GLfloat* vector2_data_pointer, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyVector2(vector2_data_pointer, uniform_var_name);
    }

    void Model::ApplyVector2ToModel(const GLfloat* vector2_data_pointer, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyVector2(vector2_data_pointer, uniform_var_name);
        }
    }

    void Model::ApplyVector3ToMesh(std::size_t mesh_index, const GLfloat* vector3_data_pointer, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyVector3(vector3_data_pointer, uniform_var_name);
    }

    void Model::ApplyVector3ToModel(const GLfloat* vector3_data_pointer, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyVector3(vector3_data_pointer, uniform_var_name);
        }
    }

    void Model::ApplyVector4ToMesh(std::size_t mesh_index, const GLfloat* vector4_data_pointer, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyVector4(vector4_data_pointer, uniform_var_name);
    }

    void Model::ApplyVector4ToModel(const GLfloat* vector4_data_pointer, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyVector4(vector4_data_pointer, uniform_var_name);
        }
    }

    void Model::ApplyIntToMesh(std::size_t mesh_index, GLint int_data, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyInt(int_data, uniform_var_name);
    }

    void Model::ApplyIntToModel(GLint int_data, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyInt(int_data, uniform_var_name);
        }
    }

    void Model::ApplyMaterialToMesh(std::size_t mesh_index, const Material* material, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyMaterial(material, uniform_var_name);
    }

    void Model::ApplyMaterialToModel(const Material* material, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyMaterial(material, uniform_var_name);
        }
    }

    void Model::ApplyDirectionalLightToMesh(std::size_t mesh_index, const DirectionalLight* light, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyDirectionalLight(light, uniform_var_name);
    }

    void Model::ApplyDirectionalLightToModel(const DirectionalLight* light, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyDirectionalLight(light, uniform_var_name);
        }
    }

    void Model::ApplyPointLightToMesh(std::size_t mesh_index, const PointLight* light, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyPointLight(light, uniform_var_name);
    }

    void Model::ApplyPointLightToModel(const PointLight* light, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyPointLight(light, uniform_var_name);
        }
    }

    void Model::ApplySpotLightToMesh(std::size_t mesh_index, const SpotLight* light, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplySpotLight(light, uniform_var_name);
    }

    void Model::ApplySpotLightToModel(const SpotLight* light, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplySpotLight(light, uniform_var_name);
        }
    }

    void Model::ApplyLocalTransformMatrixToMesh(std::size_t mesh_index, const char* uniform_var_name, bool need_transpose /*= false*/)
    {
        const GLfloat* matrix_data_pointer = glm::value_ptr(meshes_[mesh_index]->GetLocalTransformMatrix());
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyMatrix(matrix_data_pointer, uniform_var_name, need_transpose);
    }

    void Model::ApplyLocalTransformMatrixToModel(const char* uniform_var_name, bool need_transpose)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            const GLfloat* matrix_data_pointer = glm::value_ptr(meshes_[mesh_index]->GetLocalTransformMatrix());
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyMatrix(matrix_data_pointer, uniform_var_name, need_transpose);
        }
    }

    void Model::UseShaderForMesh(std::size_t mesh_index)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->Use();
    }

    void Model::UseShader()
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->Use();
        }
    }

    void Model::DrawSubset(std::size_t mesh_index)
    {
        meshes_[mesh_index]->Draw();
    }
}
