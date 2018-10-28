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
#include "kgl_lib_pch.h"
#include "kgl_basic_skinned_mesh.h"
#include "kgl_assimp_bridge.h"
#include "kgl_error.h"
#include "kgl_texture_manager.h"

namespace kgl
{


    void BasicSkinnedMesh::VertexBoneData::AddBoneData(uint32_t bone_id, float weight)
    {
        for (uint32_t i = 0; i < NUM_BONES_PER_VEREX; i++) {
            if (Weights[i] == 0.0) {
                IDs[i] = bone_id;
                Weights[i] = weight;
                return;
            }
        }

        // should never get here - more bones than we have space for
        assert(0);
    }

    BasicSkinnedMesh::BasicSkinnedMesh()
    {
        vao_ = 0;
        bones_count_ = 0;
        scene_ = NULL;
        memset(buffers_, 0, sizeof(GLuint)*NUM_VBs);
    }

    BasicSkinnedMesh::~BasicSkinnedMesh()
    {
        Clear();
    }

    void BasicSkinnedMesh::Clear()
    {
        for (uint32_t i = 0; i < textures_.size(); i++) 
        {
            textures_[i].reset();
        }

        if (buffers_[0] != 0)
        {
            glDeleteBuffers(NUM_VBs, buffers_);
        }

        if (vao_ != 0)
        {
            glDeleteVertexArrays(1, &vao_);
            vao_ = 0;
        }
    }


    bool BasicSkinnedMesh::LoadMesh(const std::string& file_name)
    {
        // Release the previously loaded mesh (if it exists)
        Clear();

        // Create the VAO
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        // Create the buffers for the vertices attributes
        glGenBuffers(NUM_VBs, buffers_);

        bool Ret = false;

        scene_ = asset_importer_.ReadFile(
            file_name.c_str(), 
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

        if (scene_)
        {
            AiMatrix4x4ToGlmMat4(scene_->mRootNode->mTransformation, global_inverse_transform_);
            global_inverse_transform_ = glm::inverse(global_inverse_transform_);
            Ret = InitFromAssetScene(scene_, file_name);
        }
        else 
        {
            printf("Error parsing '%s': '%s'\n", file_name.c_str(), asset_importer_.GetErrorString());
        }

        // Make sure the VAO is not changed from the outside
        glBindVertexArray(0);

        return Ret;
    }


    bool BasicSkinnedMesh::InitFromAssetScene(const aiScene* scene, const std::string& file_name)
    {
        mesh_entries_.resize(scene->mNumMeshes);
        textures_.resize(scene->mNumMaterials);

        std::vector<glm::vec3> Positions;
        std::vector<glm::vec3> Normals;
        std::vector<glm::vec2> TexCoords;
        std::vector<VertexBoneData> Bones;
        std::vector<uint32_t> Indices;

        uint32_t NumVertices = 0;
        uint32_t NumIndices = 0;

        // Count the number of vertices and indices
        for (uint32_t i = 0; i < mesh_entries_.size(); i++)
        {
            mesh_entries_[i].MaterialIndex = scene->mMeshes[i]->mMaterialIndex;
            mesh_entries_[i].NumIndices = scene->mMeshes[i]->mNumFaces * 3;
            mesh_entries_[i].BaseVertex = NumVertices;
            mesh_entries_[i].BaseIndex = NumIndices;

            NumVertices += scene->mMeshes[i]->mNumVertices;
            NumIndices += mesh_entries_[i].NumIndices;
        }

        // Reserve space in the vectors for the vertex attributes and indices
        Positions.reserve(NumVertices);
        Normals.reserve(NumVertices);
        TexCoords.reserve(NumVertices);
        Bones.resize(NumVertices);
        Indices.reserve(NumIndices);

        // Initialize the meshes in the scene one by one
        for (uint32_t i = 0; i < mesh_entries_.size(); i++) {
            const aiMesh* paiMesh = scene->mMeshes[i];
            InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
        }

        if (!InitMaterials(scene, file_name)) {
            return false;
        }

        // Generate and populate the buffers with vertex attributes and the indices
        glBindBuffer(GL_ARRAY_BUFFER, buffers_[POS_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(POSITION_LOCATION);
        glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, buffers_[TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(TEX_COORD_LOCATION);
        glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, buffers_[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(NORMAL_LOCATION);
        glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, buffers_[BONE_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(BONE_ID_LOCATION);
        glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
        glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
        glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[INDEX_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

        return true;
        //return GLCheckError();
    }

    void BasicSkinnedMesh::InitMesh(uint32_t mesh_index,
        const aiMesh* mesh,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texure_coords,
        std::vector<VertexBoneData>& bones,
        std::vector<unsigned int>& indices)
    {
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

        // Populate the vertex attribute vectors
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            const aiVector3D* pPos = &(mesh->mVertices[i]);
            const aiVector3D* pNormal = &(mesh->mNormals[i]);
            const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;

            positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
            normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
            texure_coords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
        }

        LoadBones(mesh_index, mesh, bones);

        // Populate the index buffer
        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            const aiFace& Face = mesh->mFaces[i];
            assert(Face.mNumIndices == 3);
            indices.push_back(Face.mIndices[0]);
            indices.push_back(Face.mIndices[1]);
            indices.push_back(Face.mIndices[2]);
        }
    }


    void BasicSkinnedMesh::LoadBones(uint32_t mesh_index, const aiMesh* mesh, std::vector<VertexBoneData>& bones)
    {
        for (uint32_t i = 0; i < mesh->mNumBones; i++)
        {
            uint32_t BoneIndex = 0;
            std::string BoneName(mesh->mBones[i]->mName.data);

            if (bone_mapping_.find(BoneName) == bone_mapping_.end())
            {
                // Allocate an index for a new bone
                BoneIndex = bones_count_;
                bones_count_++;
                BoneInfo bi;
                bone_info_.push_back(bi);
                AiMatrix4x4ToGlmMat4(mesh->mBones[i]->mOffsetMatrix, bone_info_[BoneIndex].BoneOffset);
                bone_mapping_[BoneName] = BoneIndex;
            }
            else
            {
                BoneIndex = bone_mapping_[BoneName];
            }

            for (uint32_t j = 0; j < mesh->mBones[i]->mNumWeights; j++) 
            {
                uint32_t VertexID = mesh_entries_[mesh_index].BaseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
                float Weight = mesh->mBones[i]->mWeights[j].mWeight;
                bones[VertexID].AddBoneData(BoneIndex, Weight);
            }
        }
    }

    bool BasicSkinnedMesh::InitMaterials(const aiScene* scene, const std::string& file_name)
    {
        // Extract the directory part from the file name
        std::string::size_type SlashIndex = file_name.find_last_of("/");
        std::string Dir;

        if (SlashIndex == std::string::npos)
        {
            Dir = ".";
        }
        else if (SlashIndex == 0) 
        {
            Dir = "/";
        }
        else 
        {
            Dir = file_name.substr(0, SlashIndex);
        }

        bool Ret = true;

        // Initialize the materials
        for (uint32_t i = 0; i < scene->mNumMaterials; i++)
        {
            const aiMaterial* pMaterial = scene->mMaterials[i];

            textures_[i] = NULL;

            if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
            {
                aiString Path;

                if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_DIFFUSE,
                    0, &Path, NULL, NULL, NULL, NULL, NULL)) 
                {
                    std::string p(Path.data);

                    if (p.substr(0, 2) == ".\\")
                    {
                        p = p.substr(2, p.size() - 2);
                    }

                    std::string FullPath = Dir + "/" + p;
                    kgl::TextureParams texture_param = kgl::TextureManager::MakeTextureParamsRGB(GL_REPEAT, GL_LINEAR);
                    textures_[i] = kgl::KTextureManager::GetInstance()->CreateTextureFromFile(FullPath, kgl::SOURCE_2D_TEXTURE, texture_param);
                    /*
                    m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                    if (!m_Textures[i]->Load()) {
                        printf("Error loading texture '%s'\n", FullPath.c_str());
                        delete m_Textures[i];
                        m_Textures[i] = NULL;
                        Ret = false;
                    }
                    else {
                        printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
                    }*/
                }
            }
        }

        return Ret;
    }


    void BasicSkinnedMesh::Render()
    {
        glBindVertexArray(vao_);

        for (uint32_t i = 0; i < mesh_entries_.size(); i++) 
        {
            const uint32_t material_index = mesh_entries_[i].MaterialIndex;
            assert(material_index < textures_.size());

            if (textures_[material_index].get() != nullptr)
            {
                textures_[material_index]->ActiveBind(0);
            }

            glDrawElementsBaseVertex(GL_TRIANGLES,
                mesh_entries_[i].NumIndices,
                GL_UNSIGNED_INT,
                (void*)(sizeof(uint32_t) * mesh_entries_[i].BaseIndex),
                mesh_entries_[i].BaseVertex);
        }

        glBindVertexArray(0);
    }


    uint32_t BasicSkinnedMesh::FindPosition(float animation_time, const aiNodeAnim* anim_node)
    {
        for (uint32_t i = 0; i < anim_node->mNumPositionKeys - 1; i++)
        {
            if (animation_time < (float)anim_node->mPositionKeys[i + 1].mTime)
            {
                return i;
            }
        }

        assert(0);
        return 0;
    }


    uint32_t BasicSkinnedMesh::FindRotation(float animation_time, const aiNodeAnim* anim_node)
    {
        assert(anim_node->mNumRotationKeys > 0);

        for (uint32_t i = 0; i < anim_node->mNumRotationKeys - 1; i++)
        {
            if (animation_time < (float)anim_node->mRotationKeys[i + 1].mTime)
            {
                return i;
            }
        }

        assert(0);
        return 0;
    }


    uint32_t BasicSkinnedMesh::FindScaling(float animation_time, const aiNodeAnim* anim_node)
    {
        assert(anim_node->mNumScalingKeys > 0);

        for (uint32_t i = 0; i < anim_node->mNumScalingKeys - 1; i++) {
            if (animation_time < (float)anim_node->mScalingKeys[i + 1].mTime) {
                return i;
            }
        }

        assert(0);

        return 0;
    }


    void BasicSkinnedMesh::CalcInterpolatedPosition(aiVector3D& out_position, float animation_time, const aiNodeAnim* anim_node)
    {
        if (anim_node->mNumPositionKeys == 1) 
        {
            out_position = anim_node->mPositionKeys[0].mValue;
            return;
        }

        uint32_t PositionIndex = FindPosition(animation_time, anim_node);
        uint32_t NextPositionIndex = (PositionIndex + 1);
        assert(NextPositionIndex < anim_node->mNumPositionKeys);
        float DeltaTime = (float)(anim_node->mPositionKeys[NextPositionIndex].mTime - anim_node->mPositionKeys[PositionIndex].mTime);
        float Factor = (animation_time - (float)anim_node->mPositionKeys[PositionIndex].mTime) / DeltaTime;
        assert(Factor >= 0.0f && Factor <= 1.0f);
        const aiVector3D& Start = anim_node->mPositionKeys[PositionIndex].mValue;
        const aiVector3D& End = anim_node->mPositionKeys[NextPositionIndex].mValue;
        aiVector3D Delta = End - Start;
        out_position = Start + Factor * Delta;
    }


    void BasicSkinnedMesh::CalcInterpolatedRotation(aiQuaternion& out_rotation, float animation_time, const aiNodeAnim* anim_node)
    {
        // we need at least two values to interpolate...
        if (anim_node->mNumRotationKeys == 1)
        {
            out_rotation = anim_node->mRotationKeys[0].mValue;
            return;
        }

        uint32_t RotationIndex = FindRotation(animation_time, anim_node);
        uint32_t NextRotationIndex = (RotationIndex + 1);
        assert(NextRotationIndex < anim_node->mNumRotationKeys);
        float DeltaTime = (float)(anim_node->mRotationKeys[NextRotationIndex].mTime - anim_node->mRotationKeys[RotationIndex].mTime);
        float Factor = (animation_time - (float)anim_node->mRotationKeys[RotationIndex].mTime) / DeltaTime;
        assert(Factor >= 0.0f && Factor <= 1.0f);
        const aiQuaternion& StartRotationQ = anim_node->mRotationKeys[RotationIndex].mValue;
        const aiQuaternion& EndRotationQ = anim_node->mRotationKeys[NextRotationIndex].mValue;
        aiQuaternion::Interpolate(out_rotation, StartRotationQ, EndRotationQ, Factor);
        out_rotation = out_rotation.Normalize();
    }


    void BasicSkinnedMesh::CalcInterpolatedScaling(aiVector3D& out_scale, float animation_time, const aiNodeAnim* anim_node)
    {
        if (anim_node->mNumScalingKeys == 1)
        {
            out_scale = anim_node->mScalingKeys[0].mValue;
            return;
        }

        uint32_t ScalingIndex = FindScaling(animation_time, anim_node);
        uint32_t NextScalingIndex = (ScalingIndex + 1);
        assert(NextScalingIndex < anim_node->mNumScalingKeys);
        float DeltaTime = (float)(anim_node->mScalingKeys[NextScalingIndex].mTime - anim_node->mScalingKeys[ScalingIndex].mTime);
        float Factor = (animation_time - (float)anim_node->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
        assert(Factor >= 0.0f && Factor <= 1.0f);
        const aiVector3D& Start = anim_node->mScalingKeys[ScalingIndex].mValue;
        const aiVector3D& End = anim_node->mScalingKeys[NextScalingIndex].mValue;
        aiVector3D Delta = End - Start;
        out_scale = Start + Factor * Delta;
    }

    void BasicSkinnedMesh::ReadNodeHeirarchy(float animation_time, const aiNode* node, const glm::mat4& parent_transform)
    {
        std::string NodeName(node->mName.data);

        const aiAnimation* pAnimation = scene_->mAnimations[0];

        glm::mat4 NodeTransformation;
        AiMatrix4x4ToGlmMat4(node->mTransformation, NodeTransformation);

        const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

        if (pNodeAnim)
        {
            // Interpolate scaling and generate scaling transformation matrix
            aiVector3D Scaling;
            CalcInterpolatedScaling(Scaling, animation_time, pNodeAnim);
            glm::mat4 ScalingM = glm::scale(AiVector3DToGlmVec3(Scaling));
    
            // Interpolate rotation and generate rotation transformation matrix
            aiQuaternion RotationQ;
            glm::mat4 RotationM;
            CalcInterpolatedRotation(RotationQ, animation_time, pNodeAnim);
            AiMatrix3x3ToGlmMat4(RotationQ.GetMatrix(), RotationM);

            // Interpolate translation and generate translation transformation matrix
            aiVector3D Translation;
            CalcInterpolatedPosition(Translation, animation_time, pNodeAnim);
            glm::mat4 TranslationM = glm::translate(AiVector3DToGlmVec3(Translation));

            // Combine the above transformations
            NodeTransformation = TranslationM * RotationM * ScalingM;
        }

        glm::mat4 GlobalTransformation = parent_transform * NodeTransformation;

        if (bone_mapping_.find(NodeName) != bone_mapping_.end())
        {
            uint32_t BoneIndex = bone_mapping_[NodeName];
            bone_info_[BoneIndex].FinalTransformation = global_inverse_transform_ * GlobalTransformation * bone_info_[BoneIndex].BoneOffset;
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++) 
        {
            ReadNodeHeirarchy(animation_time, node->mChildren[i], GlobalTransformation);
        }
    }


    void BasicSkinnedMesh::BoneTransform(float time_in_seconds, std::vector<glm::mat4>& transform)
    {
        glm::mat4 Identity;
        float TicksPerSecond = (float)(scene_->mAnimations[0]->mTicksPerSecond != 0 ? scene_->mAnimations[0]->mTicksPerSecond : 25.0f);
        float TimeInTicks = time_in_seconds * TicksPerSecond;
        float AnimationTime = fmod(TimeInTicks, (float)scene_->mAnimations[0]->mDuration);

        ReadNodeHeirarchy(AnimationTime, scene_->mRootNode, Identity);

        transform.resize(bones_count_);

        for (uint32_t i = 0; i < bones_count_; i++)
        {
            transform[i] = bone_info_[i].FinalTransformation;
        }
    }

    const aiNodeAnim* BasicSkinnedMesh::FindNodeAnim(const aiAnimation* animation, const std::string& node_name)
    {
        for (uint32_t i = 0; i < animation->mNumChannels; i++)
        {
            const aiNodeAnim* pNodeAnim = animation->mChannels[i];

            if (std::string(pNodeAnim->mNodeName.data) == node_name) 
            {
                return pNodeAnim;
            }
        }

        return nullptr;
    }
}
