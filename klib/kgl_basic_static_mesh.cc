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
#include "kgl_basic_static_mesh.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_texture_manager.h"

namespace kgl
{
	BasicStaticMesh::BasicStaticMesh()
	{
		vao_ = 0;
		memset(vertex_attribute_buffers_, 0, sizeof(GLuint)*6);
	}

	BasicStaticMesh::~BasicStaticMesh()
	{
		Clear();
	}

	void BasicStaticMesh::Clear()
	{
		for (unsigned int i = 0; i < textures_.size(); i++)
		{
			textures_[i].reset();
		}

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

	bool BasicStaticMesh::LoadMesh(const std::string& file_name)
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
				<< StringConvertor::ANSItoUTF16LE(file_name.c_str()) << std::endl
				<< StringConvertor::ANSItoUTF16LE(asset_importer.GetErrorString());
			throw Error(wss.str(), __FILE__, __LINE__);
		}

		glBindVertexArray(0);
		return ret;
	}

	bool BasicStaticMesh::InitFromAssetScene(const aiScene* scene, const std::string& file_name)
	{
		entries_.resize(scene->mNumMeshes);
		textures_.resize(scene->mNumMaterials);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texture_coords;
		std::vector<unsigned int> indices;

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
		positions.reserve(vertices_count);
		normals.reserve(vertices_count);
		texture_coords.reserve(vertices_count);
		indices.reserve(indices_count);

		// 依次取出每个mesh然后初始化之
		for (unsigned int i = 0; i < entries_.size(); i++)
		{
			const aiMesh* paiMesh = scene->mMeshes[i];
			InitMesh(paiMesh, positions, normals, texture_coords, indices);
		}

		if (!InitMaterials(scene, file_name))
		{
			return false;
		}

		// 顶点的position，texture coord1，normal分别在layout的0，1，2中
		glBindBuffer(GL_ARRAY_BUFFER, vertex_attribute_buffers_[POS_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(POSITION_LOCATION);
		glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_attribute_buffers_[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords[0]) * texture_coords.size(), &texture_coords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(TEX_COORD_LOCATION);
		glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_attribute_buffers_[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMAL_LOCATION);
		glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_attribute_buffers_[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

		return true;// GLCheckError();
	}

	void BasicStaticMesh::InitMesh(const aiMesh* mesh,
		std::vector<glm::vec3>& positions,
		std::vector<glm::vec3>& normals,
		std::vector<glm::vec2>& texture_coords,
		std::vector<unsigned int>& indices)
	{
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
		{
			const aiVector3D& pos = mesh->mVertices[i];
			const aiVector3D& normal = mesh->mNormals[i];
			const aiVector3D& texture_coord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : Zero3D;

			positions.push_back(glm::vec3(pos.x, pos.y, pos.z));
			normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
			texture_coords.push_back(glm::vec2(texture_coord.x, texture_coord.y));
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
		{
			const aiFace& Face = mesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			indices.push_back(Face.mIndices[0]);
			indices.push_back(Face.mIndices[1]);
			indices.push_back(Face.mIndices[2]);
		}
	}

	bool BasicStaticMesh::InitMaterials(const aiScene* scene, const std::string& file_name)
	{
		bfs::path full_path(file_name);
		std::string dir = full_path.parent_path().string();
		bool Ret = true;

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			const aiMaterial* material = scene->mMaterials[i];
			unsigned int diffuse_texture_count = material->GetTextureCount(aiTextureType_DIFFUSE);

			if (diffuse_texture_count > 0)
			{
				aiString texture_name;

				if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0,
					&texture_name, nullptr, nullptr, nullptr, nullptr, nullptr))
				{
					dir.append("/").append(texture_name.data);
					kgl::TextureParams texture_param = kgl::TextureManager::MakeTextureParamsRGB(GL_REPEAT, GL_LINEAR);
					texture_param.used_mipmap = true;
					textures_[i] = kgl::KTextureManager::GetInstance()->CreateTextureFromFile(dir, kgl::SOURCE_2D_TEXTURE, texture_param);
				}
			}
		}

		return Ret;
	}


	void BasicStaticMesh::Render()
	{
		glBindVertexArray(vao_);

		for (size_t i = 0; i < entries_.size(); i++) 
		{
			const size_t MaterialIndex = entries_[i].MaterialIndex;

			assert(MaterialIndex < textures_.size());

			if (textures_[MaterialIndex]) 
			{
				// 使用第一个纹理单元
				textures_[MaterialIndex]->ActiveBind(0);
			}

			glDrawElementsBaseVertex(GL_TRIANGLES,
				entries_[i].NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * entries_[i].BaseIndex),
				entries_[i].BaseVertex);
		}

		// Make sure the VAO is not changed from the outside    
		glBindVertexArray(0);
	}

	void BasicStaticMesh::Render(uint32_t instance_count, const glm::mat4* wvp_matrix, const glm::mat4* world_matrix)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertex_attribute_buffers_[WVP_MAT_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instance_count, wvp_matrix, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_attribute_buffers_[WORLD_MAT_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instance_count, world_matrix, GL_DYNAMIC_DRAW);

		glBindVertexArray(vao_);

		for (unsigned int i = 0; i < entries_.size(); i++) 
		{
			const unsigned int MaterialIndex = entries_[i].MaterialIndex;

			assert(MaterialIndex < textures_.size());

			if (textures_[MaterialIndex]) 
			{
				textures_[MaterialIndex]->ActiveBind(0);
			}

			glDrawElementsInstancedBaseVertex(
				GL_TRIANGLES,
				entries_[i].NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * entries_[i].BaseIndex),
				instance_count,
				entries_[i].BaseVertex);
		}

		glBindVertexArray(0);
	}
}