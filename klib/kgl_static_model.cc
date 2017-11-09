// 模型类
#include "kgl_lib_pch.h"
#include "kgl_static_model.h"
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
    StaticModel::StaticModel(VertexType vt, const char* path)
    {
        vertex_type_ = vt;
        this->LoadModel(std::string(path));
    }

    void StaticModel::Draw()
    {
        for (GLuint i = 0; i < meshes_.size(); i++)
            meshes_[i]->Draw();
    }

    void StaticModel::ApplyShaderToMesh(std::size_t index, GPUProgramSPtr shader)
    {
        if (index >= meshes_.size())
        {
            throw Error(L"The index is out of range",__FILE__,__LINE__);
        }

        meshes_[index]->SetShader(shader);
    }

    void StaticModel::ApplyShaderToModel(GPUProgramSPtr shader)
    {
        for (GLuint i = 0; i < meshes_.size(); i++)
            meshes_[i]->SetShader(shader);
    }

    GPUProgramSPtr StaticModel::GetMeshShader(std::size_t index)
    {
        if (index >= meshes_.size())
        {
            throw Error(L"The index is out of range", __FILE__, __LINE__);
        }

        return meshes_[index]->GetShader();
    }

    void StaticModel::LoadModel(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs );


        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::wstringstream wss;
            wss << L"ERROR::ASSIMP:: " << StringConvertor::ANSItoUTF16LE(importer.GetErrorString());
            throw Error(wss.str(), __FILE__, __LINE__);
            return;
        }

        // this->directory = path.substr(0, path.find_last_of('/'));
        // 从最上面的根节点开始处理起
        this->ProcessNode(scene,scene->mRootNode, glm::mat4());
    }

    void StaticModel::ProcessNode(const aiScene* scene, const aiNode* node, const glm::mat4& parent_transform)
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
            this->meshes_.push_back(this->ProcessMesh(scene, mesh, global_transform));
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

    uint8_t* StaticModel::ReadVertexData(VertexType vt, const aiMesh* mesh, uint32_t* vertex_byte_count)
    {
        switch (vt)
        {
        case VERTEX_TYPE_P: return this->ReadVertexDataP( mesh, vertex_byte_count);
        case VERTEX_TYPE_PC: return this->ReadVertexDataPC(mesh, vertex_byte_count);
        case VERTEX_TYPE_PN: return this->ReadVertexDataPN(mesh, vertex_byte_count);
        case VERTEX_TYPE_PNTB: return this->ReadVertexDataPNTB( mesh, vertex_byte_count);
        case VERTEX_TYPE_PNTBT1: return this->ReadVertexDataPNTBT1(mesh, vertex_byte_count);
        case VERTEX_TYPE_PNTBT2: return this->ReadVertexDataPNTBT2(mesh, vertex_byte_count);
        case VERTEX_TYPE_PNTBT3: return this->ReadVertexDataPNTBT3(mesh, vertex_byte_count);
        case VERTEX_TYPE_PNTBT4: return this->ReadVertexDataPNTBT4(mesh, vertex_byte_count);
        case VERTEX_TYPE_PNT1: return this->ReadVertexDataPNT1(mesh, vertex_byte_count);
        case VERTEX_TYPE_PNT2: return this->ReadVertexDataPNT2(mesh, vertex_byte_count);
        case VERTEX_TYPE_PNT3: return this->ReadVertexDataPNT3(mesh, vertex_byte_count);
        case VERTEX_TYPE_PNT4: return this->ReadVertexDataPNT3(mesh, vertex_byte_count);
        default:return nullptr;
        }
    }

    uint8_t* StaticModel::ReadVertexDataP(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* StaticModel::ReadVertexDataPC(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    
    uint8_t* StaticModel::ReadVertexDataPN(const aiMesh* mesh, uint32_t* vertex_byte_count)
    { 
        this->CheckHasPositions(mesh, L"PN", __FILE__, __LINE__);
        this->CheckHasNormals(mesh, L"PN", __FILE__, __LINE__);
        
        *vertex_byte_count = mesh->mNumVertices * sizeof(VertexPN);
        VertexPN* v = new VertexPN[mesh->mNumVertices];

        for (GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            v[i].position.x = mesh->mVertices[i].x;
            v[i].position.y = mesh->mVertices[i].y;
            v[i].position.z = mesh->mVertices[i].z;

            v[i].normal.x = mesh->mNormals[i].x;
            v[i].normal.y = mesh->mNormals[i].y;
            v[i].normal.z = mesh->mNormals[i].z;
        }

        return reinterpret_cast<uint8_t*>(v);
    }
    
    uint8_t* StaticModel::ReadVertexDataPNTB(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* StaticModel::ReadVertexDataPNTBT1(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* StaticModel::ReadVertexDataPNTBT2(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* StaticModel::ReadVertexDataPNTBT3(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    uint8_t* StaticModel::ReadVertexDataPNTBT4(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }

    void StaticModel::CheckHasPositions(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line)
    {
        if (!mesh->HasPositions())
        {
            boost::wformat f(L"The desired vertex type is %s,but the source mesh does not contain position");
            f % vertex_type;
            throw Error(f.str(), file, line);
            return;
        }
    }

    void StaticModel::CheckHasNormals(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line)
    {
        if (!mesh->HasNormals())
        {
            boost::wformat f(L"The desired vertex type is %s,but the source mesh does not contain normal");
            f % vertex_type;
            throw Error(f.str(), file, line);
            return;
        }
    }

    void StaticModel::CheckTangentAndBinormals(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line)
    {
        if (!mesh->HasTangentsAndBitangents())
        {
            boost::wformat f(L"The desired vertex type is %s,but the source mesh does not contain tangent and binormal");
            f % vertex_type;
            throw Error(f.str(), file, line);
            return;
        }
    }

    void StaticModel::CheckTextureCoords(const aiMesh* mesh, const wchar_t* vertex_type, uint32_t texture_index, const char* file, uint32_t line)
    {
        if (!mesh->HasTextureCoords(texture_index))
        {
            boost::wformat f(L"The desired vertex type is %s,but the source mesh does not contain texture coordinate %d");
            f % vertex_type % texture_index;
            throw Error(f.str(), file, line);
            return;
        }
    }
    
    uint8_t* StaticModel::ReadVertexDataPNT1(const aiMesh* mesh, uint32_t* vertex_byte_count)
    { 
        this->CheckHasPositions(mesh, L"PNT1", __FILE__, __LINE__);
        this->CheckHasNormals(mesh, L"PNT1", __FILE__, __LINE__);
        this->CheckTextureCoords(mesh, L"PNT1", 0 ,__FILE__, __LINE__);

        *vertex_byte_count = mesh->mNumVertices * sizeof(VertexPNT1);

        VertexPNT1* v = new VertexPNT1[mesh->mNumVertices];

        for (GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            v[i].position.x = mesh->mVertices[i].x;
            v[i].position.y = mesh->mVertices[i].y;
            v[i].position.z = mesh->mVertices[i].z;
            
            v[i].normal.x = mesh->mNormals[i].x;
            v[i].normal.y = mesh->mNormals[i].y;
            v[i].normal.z = mesh->mNormals[i].z;
            
            v[i].tex_coord_1.x = mesh->mTextureCoords[0][i].x;
            v[i].tex_coord_1.y = mesh->mTextureCoords[0][i].y;
        }

        return reinterpret_cast<uint8_t*>(v);
    }
    
    unsigned char* StaticModel::ReadVertexDataPNT2(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    unsigned char* StaticModel::ReadVertexDataPNT3(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
    unsigned char* StaticModel::ReadVertexDataPNT4(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }

    StaticMesh* StaticModel::ProcessMesh(const aiScene* scene, aiMesh* mesh, const glm::mat4& model_transform)
    {
        uint32_t vertex_byte_count = 0;
        uint8_t* vertex_data = this->ReadVertexData(vertex_type_, mesh, &vertex_byte_count);

        std::vector<uint32_t> index_array;

        for (GLuint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (GLuint j = 0; j < face.mNumIndices; j++)
                index_array.push_back(face.mIndices[j]);
        }

        std::vector <TextureSPtr> texture_array;

        if (scene->mNumMaterials > 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            
            TextureParams texture_param;
            texture_param.wrap_s_mode = GL_REPEAT;
            texture_param.wrap_t_mode = GL_REPEAT;
            texture_param.mag_filter_mode = GL_LINEAR;
            texture_param.min_filter_mode = GL_LINEAR;
            texture_param.internal_format = GL_RGB;
            texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
            texture_param.src_img_format = GL_RGB;
            texture_param.used_mipmap = false;

            std::vector<TextureSPtr> diffuseMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", texture_param);
            texture_array.insert(texture_array.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<TextureSPtr>  specularMaps = this->LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", texture_param);
            texture_array.insert(texture_array.end(), specularMaps.begin(), specularMaps.end());
        }

        StaticMesh* custom_mesh = new StaticMesh();
        custom_mesh->Setup(
            vertex_type_,
            vertex_data, 
            vertex_byte_count,
            &(index_array[0]),
            index_array.size() * sizeof(uint32_t),
            texture_array, model_transform);

        KGL_SAFE_DELETE_ARRAY(vertex_data);
        return custom_mesh;
    }

    std::vector<TextureSPtr> StaticModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name, const TextureParams& params)
    {
        std::vector<TextureSPtr> textures;
        std::string std_string;
        TextureManager* tm = KTextureManager::GetInstance();

        // 取得待处理的材质中的的纹理数
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            std_string.clear();
            aiString str;
            mat->GetTexture(type, i, &str);
            std_string.append("resources/model/textures/").append(str.C_Str()).append("png");
            //std_string = "resources/model/textures/tux_texture.png";

            if (tm->IsTextureExist(std_string))
                continue;

            TextureSPtr t = tm->CreateTextureFromFile(std_string, SOURCE_2D_TEXTURE, params);
            textures.push_back(t);
        }

        return textures;
    }

    void StaticModel::ApplyTextureToMesh(std::size_t mesh_index, TextureSPtr texture, const char* uniform_var_name, GLuint slot_index)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyTexture(texture, uniform_var_name, slot_index);
    }

    void StaticModel::ApplyTextureToModel(TextureSPtr texture, const char* uniform_var_name, GLuint slot_index)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyTexture(texture, uniform_var_name, slot_index);
        }
    }

    void StaticModel::ApplyMatrixToMesh(std::size_t mesh_index, const GLfloat* matrix_data_pointer, const char* uniform_var_name, bool need_transpose)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyMatrix(matrix_data_pointer, uniform_var_name, need_transpose);
    }

    void StaticModel::ApplyMatrixToModel(const GLfloat* matrix_data_pointer, const char* uniform_var_name, bool need_transpose)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyMatrix(matrix_data_pointer, uniform_var_name, need_transpose);
        }
    }

    void StaticModel::ApplyFloatToMesh(std::size_t mesh_index, GLfloat float_data, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyFloat(float_data, uniform_var_name);
    }

    void StaticModel::ApplyFloatToModel(GLfloat float_data, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyFloat(float_data, uniform_var_name);
        }
    }

    void StaticModel::ApplyVector2ToMesh(std::size_t mesh_index, const GLfloat* vector2_data_pointer, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyVector2(vector2_data_pointer, uniform_var_name);
    }

    void StaticModel::ApplyVector2ToModel(const GLfloat* vector2_data_pointer, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyVector2(vector2_data_pointer, uniform_var_name);
        }
    }

    void StaticModel::ApplyVector3ToMesh(std::size_t mesh_index, const GLfloat* vector3_data_pointer, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyVector3(vector3_data_pointer, uniform_var_name);
    }

    void StaticModel::ApplyVector3ToModel(const GLfloat* vector3_data_pointer, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyVector3(vector3_data_pointer, uniform_var_name);
        }
    }

    void StaticModel::ApplyVector4ToMesh(std::size_t mesh_index, const GLfloat* vector4_data_pointer, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyVector4(vector4_data_pointer, uniform_var_name);
    }

    void StaticModel::ApplyVector4ToModel(const GLfloat* vector4_data_pointer, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyVector4(vector4_data_pointer, uniform_var_name);
        }
    }

    void StaticModel::ApplyIntToMesh(std::size_t mesh_index, GLint int_data, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyInt(int_data, uniform_var_name);
    }

    void StaticModel::ApplyIntToModel(GLint int_data, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyInt(int_data, uniform_var_name);
        }
    }

    void StaticModel::ApplyMaterialToMesh(std::size_t mesh_index, const Material* material, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyMaterial(material, uniform_var_name);
    }

    void StaticModel::ApplyMaterialToModel(const Material* material, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyMaterial(material, uniform_var_name);
        }
    }

    void StaticModel::ApplyDirectionalLightToMesh(std::size_t mesh_index, const DirectionalLight* light, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyDirectionalLight(light, uniform_var_name);
    }

    void StaticModel::ApplyDirectionalLightToModel(const DirectionalLight* light, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyDirectionalLight(light, uniform_var_name);
        }
    }

    void StaticModel::ApplyPointLightToMesh(std::size_t mesh_index, const PointLight* light, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyPointLight(light, uniform_var_name);
    }

    void StaticModel::ApplyPointLightToModel(const PointLight* light, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyPointLight(light, uniform_var_name);
        }
    }

    void StaticModel::ApplySpotLightToMesh(std::size_t mesh_index, const SpotLight* light, const char* uniform_var_name)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplySpotLight(light, uniform_var_name);
    }

    void StaticModel::ApplySpotLightToModel(const SpotLight* light, const char* uniform_var_name)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplySpotLight(light, uniform_var_name);
        }
    }

    void StaticModel::ApplyLocalTransformMatrixToMesh(std::size_t mesh_index, const char* uniform_var_name, bool need_transpose /*= false*/)
    {
        const GLfloat* matrix_data_pointer = glm::value_ptr(meshes_[mesh_index]->GetLocalTransformMatrix());
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->ApplyMatrix(matrix_data_pointer, uniform_var_name, need_transpose);
    }

    void StaticModel::ApplyLocalTransformMatrixToModel(const char* uniform_var_name, bool need_transpose)
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            const GLfloat* matrix_data_pointer = glm::value_ptr(meshes_[mesh_index]->GetLocalTransformMatrix());
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->ApplyMatrix(matrix_data_pointer, uniform_var_name, need_transpose);
        }
    }

    void StaticModel::UseShaderForMesh(std::size_t mesh_index)
    {
        GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
        shader->Use();
    }

    void StaticModel::UseShader()
    {
        for (size_t mesh_index = 0; mesh_index < meshes_.size(); ++mesh_index)
        {
            GPUProgramSPtr shader = meshes_[mesh_index]->GetShader();
            shader->Use();
        }
    }

    void StaticModel::DrawSubset(std::size_t mesh_index)
    {
        meshes_[mesh_index]->Draw();
    }
}
