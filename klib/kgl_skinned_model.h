// 模型类
#ifndef kgl_skinned_model_h__
#define kgl_skinned_model_h__

#include "kgl_model.h"
#include "kgl_mesh.h"
// #include "kgl_skinned_mesh.h"
#include "kgl_vertex_type.h"
#include "kgl_primitive.h"

namespace kgl
{
	class SkinnedModel : public Model
	{
	private:
		struct BoneInfo
		{
			glm::mat4 BoneOffset;
			glm::mat4 FinalTransformation;
			BoneInfo();
		};
	public:
		/// <summary>
		/// 初始化一个<see cref="SkinnedModel"/>类的实例对象
		/// </summary>
		SkinnedModel(VertexType vt, const char* path);

		/// <summary>
		/// 初始化一个<see cref="SkinnedModel"/>类的实例对象
		/// </summary>
		~SkinnedModel();

		/// <summary>
		/// 返回本模型的骨块数量
		/// </summary>
		/// <returns>本模型的骨块数量</returns>
		inline uint32_t NumBones() const
		{
			return m_NumBones;
		}

		/// <summary>
		/// 绘制本模型
		/// </summary>
		virtual void Draw() override;

		/// <summary>
		/// Calculates the interpolated scaling.
		/// </summary>
		/// <param name="Out">The out.</param>
		/// <param name="AnimationTime">The animation time.</param>
		/// <param name="pNodeAnim">The p node anim.</param>
		void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

		/// <summary>
		/// Calculates the interpolated rotation.
		/// </summary>
		/// <param name="Out">The out.</param>
		/// <param name="AnimationTime">The animation time.</param>
		/// <param name="pNodeAnim">The p node anim.</param>
		void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

		/// <summary>
		/// Calculates the interpolated position.
		/// </summary>
		/// <param name="Out">The out.</param>
		/// <param name="AnimationTime">The animation time.</param>
		/// <param name="pNodeAnim">The p node anim.</param>
		void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

		/// <summary>
		/// Finds the scaling.
		/// </summary>
		/// <param name="AnimationTime">The animation time.</param>
		/// <param name="pNodeAnim">The p node anim.</param>
		/// <returns>uint32_t.</returns>
		uint32_t FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

		/// <summary>
		/// Finds the rotation.
		/// </summary>
		/// <param name="AnimationTime">The animation time.</param>
		/// <param name="pNodeAnim">The p node anim.</param>
		/// <returns>uint32_t.</returns>
		uint32_t FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);

		/// <summary>
		/// Finds the position.
		/// </summary>
		/// <param name="AnimationTime">The animation time.</param>
		/// <param name="pNodeAnim">The p node anim.</param>
		/// <returns>uint32_t.</returns>
		uint32_t FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

		/// <summary>
		/// Finds the node anim.
		/// </summary>
		/// <param name="pAnimation">The p animation.</param>
		/// <param name="NodeName">Name of the node.</param>
		/// <returns>const aiNodeAnim *.</returns>
		const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName);

		/// <summary>
		/// Reads the node heirarchy.
		/// </summary>
		/// <param name="AnimationTime">The animation time.</param>
		/// <param name="pNode">The p node.</param>
		/// <param name="ParentTransform">The parent transform.</param>
		void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);

		/// <summary>
		/// Initializes from scene.
		/// </summary>
		/// <param name="pScene">The p scene.</param>
		/// <param name="Filename">The filename.</param>
		/// <returns>bool.</returns>
		bool InitFromScene(const aiScene* pScene, const std::string& Filename);

		/// <summary>
		/// Loads the bones.
		/// </summary>
		/// <param name="MeshIndex">Index of the mesh.</param>
		/// <param name="paiMesh">The pai mesh.</param>
		/// <param name="Bones">The bones.</param>
		void LoadBones(uint32_t MeshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);

		void BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms);

		/// <summary>
		/// Clears this instance.
		/// </summary>
		void Clear();

		struct MeshEntry
		{
			MeshEntry();

			/// <summary>
			/// 本mesh有几个顶点索引
			/// </summary>
			unsigned int NumIndices;

			/// <summary>
			/// The base vertex
			/// </summary>
			unsigned int BaseVertex;

			/// <summary>
			/// 本mesh的起始索引值在整个model中的编号
			/// </summary>
			unsigned int BaseIndex;

			/// <summary>
			/// The material index
			/// </summary>
			unsigned int MaterialIndex;
		};

		std::vector<MeshEntry> m_Entries;

		/// <summary>
		///　骨块的名字和骨块的ID之间的映射关系表
		/// </summary>
		std::map<std::string, uint32_t> m_BoneMapping;

		/// <summary>
		/// 骨块的数量
		/// </summary>
		uint32_t m_NumBones;

		/// <summary>
		/// 所有骨块的信息
		/// </summary>
		std::vector<BoneInfo> m_BoneInfo;

		/// <summary>
		/// The m_ global inverse transform
		/// </summary>
		glm::mat4 m_GlobalInverseTransform;

	protected:
		/// <summary>
		/// Reads the vertex data pn bone t1.
		/// </summary>
		/// <param name="mesh_index">The mesh_index.</param>
		/// <param name="mesh">The mesh.</param>
		/// <param name="vertex_byte_count">The vertex_byte_count.</param>
		/// <returns>uint8_t *.</returns>
		virtual uint8_t* ReadVertexDataPNBoneT1(uint32_t mesh_index, const aiMesh* mesh, uint32_t* vertex_byte_count) override;

		/// <summary>
		/// Loads the model.
		/// </summary>
		/// <param name="path">The path.</param>
		virtual void LoadModel(const std::string& path) override;

		/// <summary>
		/// Processes the node.
		/// </summary>
		/// <param name="scene">The scene.</param>
		/// <param name="node">The node.</param>
		/// <param name="parent_transform">The parent_transform.</param>
		virtual void ProcessNode(const aiScene* scene, const aiNode* node, const glm::mat4& parent_transform) override;

		/// <summary>
		/// Processes the mesh.
		/// </summary>
		/// <param name="mesh_index">The mesh_index.</param>
		/// <param name="scene">The scene.</param>
		/// <param name="mesh">The mesh.</param>
		/// <param name="model_transform">The model_transform.</param>
		/// <returns>kgl.Mesh *.</returns>
		virtual Mesh* ProcessMesh(uint32_t mesh_index, const aiScene* scene, aiMesh* mesh, const glm::mat4& model_transform) override;

	protected:
		const aiScene* m_pScene = nullptr;
	};
}

#endif // kgl_static_model_h__