// 模型类
#ifndef kgl_model_h__
#define kgl_model_h__

#include "kgl_mesh.h"
#include "kgl_vertex_type.h"

namespace kgl
{
	class Model
	{
	public:
		//************************************
		// Method:    Model
		// FullName:  kgl::Model::Model
		// Access:    public 
		// Returns:   
		// Qualifier:
		// Parameter: VertexType vt
		// Parameter: const char * path
		//************************************
		Model(VertexType vt, const char* path);

		//************************************
		// Method:    Draw
		// FullName:  kgl::Model::Draw
		// Access:    public 
		// Returns:   void
		// Qualifier:
		//************************************
		void Draw();

		//************************************
		// Method:    ApplyShaderToMesh
		// FullName:  kgl::Model::ApplyShaderToMesh
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: std::size_t index
		// Parameter: GPUProgramSPtr shader
		//************************************
		void ApplyShaderToMesh(std::size_t index,GPUProgramSPtr shader);

		//************************************
		// Method:    GetMeshShader
		// FullName:  kgl::Model::GetMeshShader
		// Access:    public 
		// Returns:   kgl::GPUProgramSPtr
		// Qualifier:
		// Parameter: std::size_t index
		//************************************
		GPUProgramSPtr GetMeshShader(std::size_t index);

		//************************************
		// Method:    GetMeshCount
		// FullName:  kgl::Model::GetMeshCount
		// Access:    public 
		// Returns:   const std::size_t
		// Qualifier: const
		//************************************
		inline const std::size_t GetMeshCount() const
		{
			return meshes_.size();
		}

		//************************************
		// Method:    GetMeshCount
		// FullName:  kgl::Model::GetMeshCount
		// Access:    public 
		// Returns:   std::size_t
		// Qualifier:
		//************************************
		inline std::size_t GetMeshCount()
		{
			return meshes_.size();
		}

	protected:
		void LoadModel(const std::string& path);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<TextureSPtr> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name, const TextureParams& params);
		uint8_t* ReadVertexData(VertexType vt, const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataP(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPC(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPN(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPNTB(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPNTBT1(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPNTBT2(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPNTBT3(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPNTBT4(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPNT1(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPNT2(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPNT3(const aiMesh* mesh, uint32_t* vertex_byte_count);
		uint8_t* ReadVertexDataPNT4(const aiMesh* mesh, uint32_t* vertex_byte_count);
		void CheckHasPositions(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line);
		void CheckHasNormals(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line);
		void CheckTangentAndBinormals(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line);
		void CheckTextureCoords(const aiMesh* mesh, const wchar_t* vertex_type, uint32_t texture_index, const char* file, uint32_t line);
	protected:
		std::vector<Mesh*>			meshes_;
		VertexType					vertex_type_;
	};
}

#endif // kgl_model_h__