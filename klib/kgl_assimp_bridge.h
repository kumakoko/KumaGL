// assimp和glm之间的一些数据结构之间的转换
#ifndef kgl_assimp_bridge_h__
#define kgl_assimp_bridge_h__

namespace kgl
{
	// aiMatrix和glm matrix都是列优先存储的形式
	// 但aiMtrix不仅是逻辑上是列优先存储，同时在存储形式上也是列优先存储
	// 而glm matrix则是逻辑上是列优先存储，但存储形式上却是使用类似于行优先存储的形式
	// 详细见mesh文件中的注解
	void AiMatrix4x4ToGlmMat4(const aiMatrix4x4& ai_matrix, glm::mat4& glm_matrix);

	void AiMatrix3x3ToGlmMat4(const aiMatrix3x3& ai_matrix, glm::mat4& glm_matrix);

	void AiVector3DToGlmVec3(const aiVector3D& ai_vector, glm::vec3 glm_vector);

	glm::vec3  AiVector3DToGlmVec3(const aiVector3D& ai_vector);
}

#endif // kgl_assimp_bridge_h__