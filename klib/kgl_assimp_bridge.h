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
/*!
* \file kgl_assimp_bridge.h
* \date 2017/12/31 13:34
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: assimp和glm之间的一些数据结构之间的转换，以及一些处理assimp相关的工具函数
*
* \note
*/

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

    void AiVector3DToGlmVec3(const aiVector3D& ai_vector, glm::vec3& glm_vector);

    glm::vec3 AiVector3DToGlmVec3(const aiVector3D& ai_vector);

    //************************************
    // Method:    CalculateAABB
    // FullName:  kgl::CalculateAABB
    // Access:    public 
    // Returns:   void
    // Qualifier: 根据给定的节点，计算该节点以及其子节点所占据的AABB包围盒
    // Parameter: const aiScene * scene assimp的场景节点
    // Parameter: const aiNode * node   待计算的根节点
    // Parameter: const aiMatrix4x4 & transform_matrix node节点相对其父节点的transform matrix
    // Parameter: aiVector3D & out_bound aiVector3D类型数组，该数组包含了两个aiVector3D，存储了AABB包围盒的左上角和右下角的点坐标，
    //************************************
    void CalculateAABB(const aiScene* scene, const aiNode* node, const aiMatrix4x4& transform_matrix, aiVector3D* out_bound);

    //************************************
    // Method:    ScaleAsset
    // FullName:  kgl::ScaleAsset
    // Access:    public 
    // Returns:   void
    // Qualifier: 根据当前场景下mesh的大小，选择一个合适的scale矩阵，使得这个mesh能刚好
    // Parameter: aiScene * scene
    // Parameter: aiMatrix4x4 & world_matrix
    //************************************
    void ScaleAsset(aiScene* scene, aiMatrix4x4& world_matrix);
}

#endif // kgl_assimp_bridge_h__