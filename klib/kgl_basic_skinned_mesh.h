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
* \file kgl_basic_skinned_mesh.h
* \date 2017/12/21 10:29
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/
#ifndef kgl_basic_skinned_mesh_h__
#define kgl_basic_skinned_mesh_h__

#include "kgl_transform.h"
#include "kgl_texture.h"

namespace kgl
{
    class BasicSkinnedMesh
    {
    protected:
        static const int NUM_BONES_PER_VEREX = 4;
        static const int POSITION_LOCATION = 0;
        static const int TEX_COORD_LOCATION = 1;
        static const int NORMAL_LOCATION = 2;
        static const int BONE_ID_LOCATION = 3;
        static const int BONE_WEIGHT_LOCATION = 4;
        static const int INVALID_MATERIAL = 0xFFFFFFFF;
    public:
        BasicSkinnedMesh();

        ~BasicSkinnedMesh();

        bool LoadMesh(const std::string& file_name);

        void Render();

        inline uint32_t NumBones() const
        {
            return bones_count_;
        }

        void BoneTransform(float time_in_seconds, std::vector<glm::mat4>& transform);

    private:
        enum VB_TYPES
        {
            INDEX_BUFFER,
            POS_VB,
            NORMAL_VB,
            TEXCOORD_VB,
            BONE_VB,
            NUM_VBs
        };

        struct MeshEntry
        {
            MeshEntry()
            {
                NumIndices = 0;
                BaseVertex = 0;
                BaseIndex = 0;
                MaterialIndex = INVALID_MATERIAL;
            }

            unsigned int NumIndices;
            unsigned int BaseVertex;
            unsigned int BaseIndex;
            unsigned int MaterialIndex;
        };

        struct BoneInfo
        {
            glm::mat4 BoneOffset;
            glm::mat4 FinalTransformation;
        };

        struct VertexBoneData
        {
            uint32_t IDs[NUM_BONES_PER_VEREX];
            float Weights[NUM_BONES_PER_VEREX];

            VertexBoneData()
            {
                Reset();
            };

            void Reset()
            {
                memset(IDs, 0, sizeof(uint32_t)*NUM_BONES_PER_VEREX);
                memset(Weights, 0, sizeof(float)*NUM_BONES_PER_VEREX);
            }

            void AddBoneData(uint32_t bone_id, float weight);
        };

    protected:

        /// <summary>
        /// Calculates the interpolated scaling.
        /// </summary>
        /// <param name="out_scale">The out_scale.</param>
        /// <param name="animation_time">The animation_time.</param>
        /// <param name="anim_node">The anim_node.</param>
        void CalcInterpolatedScaling(aiVector3D& out_scale, float animation_time, const aiNodeAnim* anim_node);

        /// <summary>
        /// Calculates the interpolated rotation.
        /// </summary>
        /// <param name="out_rotation">The out_rotation.</param>
        /// <param name="animation_time">The animation_time.</param>
        /// <param name="anim_node">The anim_node.</param>
        void CalcInterpolatedRotation(aiQuaternion& out_rotation, float animation_time, const aiNodeAnim* anim_node);

        /// <summary>
        /// Calculates the interpolated position.
        /// </summary>
        /// <param name="out_position">The out_position.</param>
        /// <param name="animation_time">The animation_time.</param>
        /// <param name="anim_node">The anim_node.</param>
        void CalcInterpolatedPosition(aiVector3D& out_position, float animation_time, const aiNodeAnim* anim_node);

        /// <summary>
        /// Finds the scaling.
        /// </summary>
        /// <param name="animation_time">The animation_time.</param>
        /// <param name="anim_node">The anim_node.</param>
        /// <returns>uint32_t.</returns>
        uint32_t FindScaling(float animation_time, const aiNodeAnim* anim_node);

        /// <summary>
        /// Finds the rotation.
        /// </summary>
        /// <param name="animation_time">The animation_time.</param>
        /// <param name="anim_node">The anim_node.</param>
        /// <returns>uint32_t.</returns>
        uint32_t FindRotation(float animation_time, const aiNodeAnim* anim_node);

        /// <summary>
        /// Finds the position.
        /// </summary>
        /// <param name="animation_time">The animation_time.</param>
        /// <param name="anim_node">The anim_node.</param>
        /// <returns>uint32_t.</returns>
        uint32_t FindPosition(float animation_time, const aiNodeAnim* anim_node);

        /// <summary>
        /// Finds the node anim.
        /// </summary>
        /// <param name="animation">The animation.</param>
        /// <param name="node_name">The node_name.</param>
        /// <returns>const aiNodeAnim *.</returns>
        const aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const std::string& node_name);

        /// <summary>
        /// Reads the node heirarchy.
        /// </summary>
        /// <param name="animation_time">The animation_time.</param>
        /// <param name="node">The node.</param>
        /// <param name="parent_transform">The parent_transform.</param>
        void ReadNodeHeirarchy(float animation_time, const aiNode* node, const glm::mat4& parent_transform);

        /// <summary>
        /// Initializes from asset scene.
        /// </summary>
        /// <param name="scene">The scene.</param>
        /// <param name="file_name">The file_name.</param>
        /// <returns>bool.</returns>
        bool InitFromAssetScene(const aiScene* scene, const std::string& file_name);

        /// <summary>
        /// Initializes the mesh.
        /// </summary>
        /// <param name="mesh_index">The mesh_index.</param>
        /// <param name="mesh">The mesh.</param>
        /// <param name="positions">The positions.</param>
        /// <param name="normals">The normals.</param>
        /// <param name="texure_coords">The texure_coords.</param>
        /// <param name="bones">The bones.</param>
        /// <param name="indices">The indices.</param>
        void InitMesh(uint32_t mesh_index,
            const aiMesh* mesh,
            std::vector<glm::vec3>& positions,
            std::vector<glm::vec3>& normals,
            std::vector<glm::vec2>& texure_coords,
            std::vector<VertexBoneData>& bones,
            std::vector<unsigned int>& indices);

        /// <summary>
        /// Loads the bones.
        /// </summary>
        /// <param name="mesh_index">The mesh_index.</param>
        /// <param name="mesh">The mesh.</param>
        /// <param name="bones">The bones.</param>
        void LoadBones(uint32_t mesh_index, const aiMesh* mesh, std::vector<VertexBoneData>& bones);

        /// <summary>
        /// Initializes the materials.
        /// </summary>
        /// <param name="scene">The scene.</param>
        /// <param name="file_name">The file_name.</param>
        /// <returns>bool.</returns>
        bool InitMaterials(const aiScene* scene, const std::string& file_name);

        /// <summary>
        /// Clears this instance.
        /// </summary>
        void Clear();

    protected:
        /// <summary>
        /// The scene_
        /// </summary>
        const aiScene*                  scene_ = nullptr;

        /// <summary>
        /// The bones_count_
        /// </summary>
        uint32_t                        bones_count_ = 0;

        /// <summary>
        /// The vao_
        /// </summary>
        GLuint                          vao_;

        /// <summary>
        /// The buffers_
        /// </summary>
        GLuint                          buffers_[NUM_VBs];

        /// <summary>
        /// The buffers_
        /// </summary>
        glm::mat4                       global_inverse_transform_;

        /// <summary>
        /// The asset_importer_
        /// </summary>
        Assimp::Importer                asset_importer_;

        /// <summary>
        /// The mesh_entries_
        /// </summary>
        std::vector<MeshEntry>          mesh_entries_;

        /// <summary>
        /// The textures_
        /// </summary>
        std::vector<TextureSPtr>        textures_;

        /// <summary>
        /// The bone_info_
        /// </summary>
        std::vector<BoneInfo>           bone_info_;

        /// <summary>
        /// The bone_mapping_
        /// </summary>
        std::map<std::string, uint32_t> bone_mapping_; // maps a bone name to its index
    };
}
#endif // kgl_basic_skinned_mesh_h__