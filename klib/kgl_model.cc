// 模型类
#include "kgl_lib_pch.h"
#include "kgl_model.h"
#include "kgl_string_convertor.h"
#include "kgl_error.h"
#include "kgl_texture_manager.h"
#include "kgl_defines.h"

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
			throw Error(L"The index is out of range",__FILE__,__LINE__);
		}

		meshes_[index]->SetShader(shader);
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
		this->ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(const aiNode* node, const aiScene* scene)
	{
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes_.push_back(this->ProcessMesh(mesh, scene));
		}

		// 遞歸處理
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessNode(node->mChildren[i], scene);
		}
	}

	uint8_t* Model::ReadVertexData(VertexType vt, const aiMesh* mesh, uint32_t* vertex_byte_count)
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

	uint8_t* Model::ReadVertexDataP(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
	uint8_t* Model::ReadVertexDataPC(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
	
	uint8_t* Model::ReadVertexDataPN(const aiMesh* mesh, uint32_t* vertex_byte_count)
	{ 
		this->CheckHasPositions(mesh, L"PNT1", __FILE__, __LINE__);
		this->CheckHasNormals(mesh, L"PNT1", __FILE__, __LINE__);
		
		*vertex_byte_count = mesh->mNumVertices * sizeof(VertexPN);
		VertexPN* v = new VertexPN[mesh->mNumVertices];

		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			v[i].P.x = mesh->mVertices[i].x;
			v[i].P.y = mesh->mVertices[i].y;
			v[i].P.z = mesh->mVertices[i].z;

			v[i].N.x = mesh->mNormals[i].x;
			v[i].N.y = mesh->mNormals[i].y;
			v[i].N.z = mesh->mNormals[i].z;
		}

		return reinterpret_cast<uint8_t*>(v);
	}
	
	uint8_t* Model::ReadVertexDataPNTB(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
	uint8_t* Model::ReadVertexDataPNTBT1(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
	uint8_t* Model::ReadVertexDataPNTBT2(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
	uint8_t* Model::ReadVertexDataPNTBT3(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
	uint8_t* Model::ReadVertexDataPNTBT4(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }

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
	
	uint8_t* Model::ReadVertexDataPNT1(const aiMesh* mesh, uint32_t* vertex_byte_count)
	{ 
		this->CheckHasPositions(mesh, L"PNT1", __FILE__, __LINE__);
		this->CheckHasNormals(mesh, L"PNT1", __FILE__, __LINE__);
		this->CheckTextureCoords(mesh, L"PNT1", 0 ,__FILE__, __LINE__);

		*vertex_byte_count = mesh->mNumVertices * sizeof(VertexPNT1);

		VertexPNT1* v = new VertexPNT1[mesh->mNumVertices];

		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			v[i].P.x = mesh->mVertices[i].x;
			v[i].P.y = mesh->mVertices[i].y;
			v[i].P.z = mesh->mVertices[i].z;
			
			v[i].N.x = mesh->mNormals[i].x;
			v[i].N.y = mesh->mNormals[i].y;
			v[i].N.z = mesh->mNormals[i].z;
			
			v[i].T1.x = mesh->mTextureCoords[0][i].x;
			v[i].T1.y = mesh->mTextureCoords[0][i].y;
		}

		return reinterpret_cast<uint8_t*>(v);
	}
	
	unsigned char* Model::ReadVertexDataPNT2(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
	unsigned char* Model::ReadVertexDataPNT3(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }
	unsigned char* Model::ReadVertexDataPNT4(const aiMesh* mesh, uint32_t* vertex_byte_count){ return nullptr; }

	Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
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
			texture_param.WrapSMode = GL_REPEAT;
			texture_param.WrapTMode = GL_REPEAT;
			texture_param.MagFilterMode = GL_LINEAR;
			texture_param.MinFilterMode = GL_LINEAR;
			texture_param.InternalFormat = GL_RGB;
			texture_param.SrcImgPixelComponentType = GL_UNSIGNED_BYTE;
			texture_param.SrcImgFormat = GL_RGB;
			texture_param.LoadChannel = SOIL_LOAD_RGB;
			texture_param.UseMipmap = false;

			std::vector<TextureSPtr> diffuseMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", texture_param);
			texture_array.insert(texture_array.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<TextureSPtr>  specularMaps = this->LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", texture_param);
			texture_array.insert(texture_array.end(), specularMaps.begin(), specularMaps.end());
		}

		Mesh* custom_mesh = new Mesh();
		custom_mesh->Setup(vertex_type_, vertex_data, vertex_byte_count,&(index_array[0]),index_array.size() * sizeof(uint32_t), texture_array);

		KGL_SAFE_DELETE_ARRAY(vertex_data);
		return custom_mesh;
	}

	std::vector<TextureSPtr> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name, const TextureParams& params)
	{
		std::vector<TextureSPtr> textures;
		std::string std_string;
		TextureManager* tm = TextureMgr::Instance();

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
}