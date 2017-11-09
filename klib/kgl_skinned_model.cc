// 模型类
#include "kgl_lib_pch.h"
#include "kgl_skinned_model.h"
#include "kgl_skinned_mesh.h"
#include "kgl_vertex_type.h"
#include "kgl_debug_tool.h"
#include "kgl_string_convertor.h"
#include "kgl_assimp_bridge.h"
#include "kgl_error.h"
#include "kgl_defines.h"
#include "kgl_texture_manager.h"

#if defined(__APPLE__) && defined(__MACH__)
#include "SOIL.h"
#endif

namespace kgl
{
    SkinnedModel::BoneInfo::BoneInfo()
    {
        BoneOffset = glm::mat4();
        FinalTransformation = glm::mat4();
    }

    SkinnedModel::MeshEntry::MeshEntry()
    {
        NumIndices = 0;
        BaseVertex = 0;
        BaseIndex = 0;
    }

    SkinnedModel::SkinnedModel(VertexType vt, const char* path):
    Model(vt,path)
    {
    }

    SkinnedModel::~SkinnedModel()
    {
        Clear();
    }

	void SkinnedModel::Clear()
	{

	}

    void SkinnedModel::LoadModel(const std::string& path)
    {
        uint32_t load_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
                              aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, load_flags);

        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::wstringstream wss;
            wss << L"ERROR::ASSIMP:: " << StringConvertor::ANSItoUTF16LE(importer.GetErrorString());
            throw Error(wss.str(), __FILE__, __LINE__);
            return;
        }

        m_Entries.resize(scene->mNumMeshes); // model有多少个mesh就有多少个入口

        uint32_t NumVertices = 0;
        uint32_t NumIndices = 0;

        for (uint32_t i = 0; i < m_Entries.size(); i++)
        {
            m_Entries[i].MaterialIndex = scene->mMeshes[i]->mMaterialIndex; // 每一个mesh所使用材质的索引值
            m_Entries[i].NumIndices = scene->mMeshes[i]->mNumFaces * 3; // 每个mesh的顶点索引数就是三角形面数乘以3
            m_Entries[i].BaseVertex = NumVertices; // 每一个mesh的起始顶点编号，就是用它在整个模型中的每个mesh的顶点数，逐个叠加之和
            m_Entries[i].BaseIndex = NumIndices; // 每一个mesh的起始顶点编号，就是用它在整个模型中的每个mesh的顶点索引数，逐个叠加之和
            NumVertices += scene->mMeshes[i]->mNumVertices;
            NumIndices += m_Entries[i].NumIndices;
        }

        AiMatrix4x4ToGlmMat4(scene->mRootNode->mTransformation, m_GlobalInverseTransform);
        m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);
        
        // 从最上面的根节点开始处理起
        this->ProcessNode(scene, scene->mRootNode, glm::mat4());
    }

    void SkinnedModel::ProcessNode(const aiScene* scene, const aiNode* node, const glm::mat4& parent_transform)
    {
        // 得到本节点相对于父空间的transform matrix
        glm::mat4 node_transform;
        AiMatrix4x4ToGlmMat4(node->mTransformation, node_transform);

        // 递归算出本节点相对于世界坐标的transform matrix
        glm::mat4 global_transform = parent_transform * node_transform;
        for (GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes_.push_back(this->ProcessMesh(i,scene, mesh, global_transform));
        }

        // 遞歸處理
        for (GLuint i = 0; i < node->mNumChildren; i++)
        {
            this->ProcessNode(scene, node->mChildren[i], global_transform);
        }
    }

    uint8_t* SkinnedModel::ReadVertexDataPNBoneT1(uint32_t mesh_index,const aiMesh* mesh, uint32_t* vertex_byte_count)
    {
//      this->CheckHasPositions(mesh, L"PN", __FILE__, __LINE__);
//      this->CheckHasNormals(mesh, L"PN", __FILE__, __LINE__);
        *vertex_byte_count = mesh->mNumVertices * sizeof(VertexPNBoneT1);
        VertexPNBoneT1* v = new VertexPNBoneT1[mesh->mNumVertices];

        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            v[i].position.x = mesh->mVertices[i].x;
            v[i].position.y = mesh->mVertices[i].y;
            v[i].position.z = mesh->mVertices[i].z;

            v[i].normal.x = mesh->mNormals[i].x;
            v[i].normal.y = mesh->mNormals[i].y;
            v[i].normal.z = mesh->mNormals[i].z;

            v[i].tex_coord_1.x = mesh->mTextureCoords[0][i].x;
            v[i].tex_coord_1.y = mesh->mTextureCoords[0][i].y;

            for (int j = 0; j < VertexBoneData::NUM_BONES_PER_VEREX; ++j)
            {
                v[i].bone_data.affectd_bone_id[j] = 0;
                v[i].bone_data.weights[j] = 0;
            }

            uint32_t vertex_id = m_Entries[mesh_index].BaseVertex + i;

            // 遍历本mesh中的每一个骨块
            for (uint32_t m = 0; m < mesh->mNumBones; m++)
            {
                uint32_t bone_index = 0;
                std::string BoneName(mesh->mBones[i]->mName.data);

                // 本骨块是不是已经在列表中了，不是的话就新建一个索引
                if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
                {
                    bone_index = m_NumBones; // 索引就是当前的骨块数
                    m_NumBones++;
                    BoneInfo bi;
                    m_BoneInfo.push_back(bi); // 记录下新的骨块信息
                    // m_BoneInfo[BoneIndex].BoneOffset = mesh->mBones[i]->mOffsetMatrix;
                    AiMatrix4x4ToGlmMat4(mesh->mBones[i]->mOffsetMatrix, m_BoneInfo[bone_index].BoneOffset);
                    m_BoneMapping[BoneName] = bone_index;
                }
                else
                {
                    bone_index = m_BoneMapping[BoneName];
                }

                // 每个骨块有几个权重值，就表示本骨块将会影响多少个顶点
                for (uint32_t n = 0; n < mesh->mBones[m]->mNumWeights; n++)
                {
                    // 如果当前遍历的骨块的某个权重属性中不影响本顶点，就跳过，否则就写入到本顶点
                    if ( m_Entries[mesh_index].BaseVertex + 
                         mesh->mBones[m]->mWeights[n].mVertexId != vertex_id)
                         continue;

                    v[i].bone_data.AddBoneData(bone_index, mesh->mBones[m]->mWeights[n].mWeight);
                }
            }
        }

        return reinterpret_cast<uint8_t*>(v);
    }

    Mesh* SkinnedModel::ProcessMesh(uint32_t mesh_index, const aiScene* scene, aiMesh* mesh, const glm::mat4& model_transform)
    {
        uint32_t vertex_byte_count = 0;
        uint8_t* vertex_data = this->ReadVertexData(vertex_type_, mesh_index, mesh, &vertex_byte_count);

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

        SkinnedMesh* custom_mesh = new SkinnedMesh();
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

    void SkinnedModel::LoadBones(uint32_t MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
    {
        for (uint32_t i = 0; i < pMesh->mNumBones; i++) 
        {
            uint32_t BoneIndex = 0;
            std::string BoneName(pMesh->mBones[i]->mName.data);

            // 如果骨块映射表中，没有当前遍历到的这块骨块的名字的话，就创建一个新的骨块信息
            if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
            {
                // 给骨块分配一个新的索引值
                BoneIndex = m_NumBones;
                m_NumBones++;
                m_BoneMapping[BoneName] = BoneIndex;
                BoneInfo bi;
                AiMatrix4x4ToGlmMat4(pMesh->mBones[i]->mOffsetMatrix, bi.BoneOffset);
                m_BoneInfo.push_back(bi);
            }
            else
            {
                BoneIndex = m_BoneMapping[BoneName]; //如果本身已经存在的，就把那个存在的index取出来
            }

            for (uint32_t j = 0; j < pMesh->mBones[i]->mNumWeights; j++)
            {
                // 拿到本网格中，第i块骨块中，第j个权重值结构中，权重值结构所作用到的那个顶点的ID
                // 然后加上本mesh中的起始顶点值的编号，作为骨块信息组的索引值
                uint32_t VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
                float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
                Bones[VertexID].AddBoneData(BoneIndex, Weight);
            }
        }
    }

	void SkinnedModel::Draw()
	{

	}

	uint32_t SkinnedModel::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (uint32_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) 
		{
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) 
			{
				return i;
			}
		}

		return 0;
	}

	uint32_t SkinnedModel::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (uint32_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) 
		{
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) 
			{
				return i;
			}
		}

		return 0;
	}


	uint32_t SkinnedModel::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKeys > 0);

		for (uint32_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);

		return 0;
	}


	void SkinnedModel::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1) {
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}

		uint32_t PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		uint32_t NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}


	void SkinnedModel::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) {
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		uint32_t RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		uint32_t NextRotationIndex = (RotationIndex + 1);
		// assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}


	void SkinnedModel::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1) {
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		uint32_t ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		uint32_t NextScalingIndex = (ScalingIndex + 1);
		//assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}


	void SkinnedModel::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
	{
		std::string NodeName(pNode->mName.data);

		const aiAnimation* pAnimation = m_pScene->mAnimations[0];

		glm::mat4 NodeTransformation;
		AiMatrix4x4ToGlmMat4(pNode->mTransformation, NodeTransformation);// Matrix4f NodeTransformation(pNode->mTransformation);

		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

		// 如果存在着动画节点，就用插值计算得来的TRS矩阵，
		// 得到本节点相对于其父节点的变换矩阵
		if (pNodeAnim)
		{
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			
			/*
			Matrix4f ScalingM;
			ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);
			*/
			glm::mat4 ScalingM = glm::scale(AiVector3DToGlmVec3(Scaling));

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
			// Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());
			glm::mat4 RotationM;
			AiMatrix3x3ToGlmMat4(RotationQ.GetMatrix(),RotationM);

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);

			//Matrix4f TranslationM;
			//TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);
			glm::mat4 TranslationM = glm::translate(AiVector3DToGlmVec3(Translation));

			// Combine the above transformations
			NodeTransformation = TranslationM * RotationM * ScalingM;
		}

		// 递归算出本节点相对于世界坐标的transform matrix
		glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) 
		{
			uint32_t BoneIndex = m_BoneMapping[NodeName];
			m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
		}

		for (uint32_t i = 0; i < pNode->mNumChildren; i++) {
			ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
		}
	}


	void SkinnedModel::BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms)
	{
		glm::mat4 Identity(1.0f);
		// Identity.InitIdentity();

		float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ?
			m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

		ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

		Transforms.resize(m_NumBones);

		for (uint32_t i = 0; i < m_NumBones; i++) {
			Transforms[i] = m_BoneInfo[i].FinalTransformation;
		}
	}


	const aiNodeAnim* SkinnedModel::FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName)
	{
		for (uint32_t i = 0; i < pAnimation->mNumChannels; i++) {
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
				return pNodeAnim;
			}
		}

		return NULL;
	}
}
