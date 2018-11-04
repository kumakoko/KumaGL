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
* \file kgl_basic_static_mesh.h
* \date 2017/12/21 10:22
*
* \author soft_koko
* Contact: sun_of_lover@sina.com
*
* \brief ��̬������
*
* TODO: ��̬�����࣬�ṩ��assimp����֧�ֵ�ģ��������һ��
*       ��̬��mesh����Ⱦ
*
* \note ��̬�Ĵ��ж�������ֵ��mesh��mesh��������Ҫ������
*       position,texture coord1,normal
*       �������ԵĲ��֣���vertex shader��
*/
#ifndef kgl_basic_static_mesh_h__
#define kgl_basic_static_mesh_h__

#include "kgl_transform.h"
#include "kgl_texture.h"

namespace kgl
{
    struct BasicStaticMeshVertex
    {
        glm::vec3 m_pos;
        glm::vec2 m_tex;
        glm::vec3 m_normal;

        BasicStaticMeshVertex() {}

        BasicStaticMeshVertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
        {
            m_pos = pos;
            m_tex = tex;
            m_normal = normal;
        }
    };

    class BasicStaticMesh
    {
    private:
        /// <summary>
        /// ��Ч�Ĳ�������
        /// </summary>
        static const int INVALID_MATERIAL = 0xFFFFFFFF;
        
        /// <summary>
        /// vertex_attribute_buffers_�����У�����������attribute buffer�ŵ�0λ
        /// </summary>
        static const int INDEX_BUFFER = 0;

        /// <summary>
        /// vertex_attribute_buffers_�����У�����λ�������attribute buffer�ŵ�1λ
        /// </summary>
        static const int POS_VB = 1;

        /// <summary>
        /// vertex_attribute_buffers_�����У����㷨�ߵ�attribute buffer�ŵ�2λ
        /// </summary>
        static const int NORMAL_VB = 2;

        /// <summary>
        /// vertex_attribute_buffers_�����У������һ�����������attribute buffer�ŵ�3λ
        /// </summary>
        static const int TEXCOORD_VB = 3;

        /// <summary>
        /// vertex_attribute_buffers_�����У������wvp�任�����attribute buffer�ŵ�4λ
        /// </summary>
        static const int WVP_MAT_VB = 4;

        /// <summary>
        /// vertex_attribute_buffers_�����У������world�����attribute buffer�ŵ�4λ
        /// </summary>
        static const int WORLD_MAT_VB = 5;

        /// <summary>
        /// ��vertex shader�У������е�position��layoutֵΪ0
        /// </summary>
        static const int POSITION_LOCATION = 0;
        
        /// <summary>
        /// ��vertex shader�У������еĵ�һ������ӳ�������layoutֵΪ1
        /// </summary>
        static const int TEX_COORD_LOCATION = 1;
        
        /// <summary>
        /// T��vertex shader�У������еķ���ֵ��layoutֵΪ1
        /// </summary>
        static const int NORMAL_LOCATION = 2;

        struct BasicMeshEntry
        {
            BasicMeshEntry()
            {
                NumIndices = 0;
                BaseVertex = 0;
                BaseIndex = 0;
                MaterialIndex = INVALID_MATERIAL;
            }

            uint32_t NumIndices;
            uint32_t BaseVertex;
            uint32_t BaseIndex;
            uint32_t MaterialIndex;
        };
    public:
        /// <summary>
        /// <see cref="BasicStaticMesh"/>��Ĺ��캯��
        /// </summary>
        BasicStaticMesh();

        /// <summary>
        /// <see cref="BasicStaticMesh"/>�����������
        /// </summary>
        ~BasicStaticMesh();

        /// <summary>
        /// Loads the mesh.
        /// </summary>
        /// <param name="file_name">The file_name.</param>
        /// <returns>bool.</returns>
        bool LoadMesh(const std::string& file_name);

        /// <summary>
        /// ��Ⱦ����
        /// </summary>
        void Render();

        /// <summary>
        /// ��instance��ʽ����Ⱦinstance_count����ʵ����ʵ��
        /// </summary>
        /// <param name="instance_count">Ҫ��Ⱦ��mesh��ʵ���ĸ���</param>
        /// <param name="wvp_matrix">�洢�Ÿ�����Ⱦ��meshʵ����wvp�����������ָ��</param>
        /// <param name="world_matrix">�洢�Ÿ�����Ⱦ��meshʵ����world�����������ָ��</param>
        void Render(uint32_t instance_count, const glm::mat4* wvp_matrix, const glm::mat4* world_matrix);

        /// <summary>
        /// ��ȡ����mesh��translation��rotation��scale
        /// </summary>
        /// <returns>kgl.Orientation &.</returns>
        inline Orientation& GetOrientation()
        {
            return transform_;
        }

    private:
        /// <summary>
        /// ��һ��Assimp���ṩ��aiScene���͵ĳ����д�������mesh
        /// </summary>
        /// <param name="scene">Assimp���ṩ��aiScene���͵ĳ���</param>
        /// <param name="file_name">ģ���ļ���</param>
        /// <returns>��ʼ���ɹ�����true�����򷵻�false</returns>
        bool InitFromAssetScene(const aiScene* scene, const std::string& file_name);

        /// <summary>
        /// ����import������aiMesh����ѹ�������λ�ã����ߣ���һ�������������ԣ�������Ӧ�Ļ�����
        /// </summary>
        /// <param name="mesh">assimp���ļ��ж����aiMesh����</param>
        /// <param name="positions">�����Ķ����position����</param>
        /// <param name="normals">�����Ķ���ķ�������</param>
        /// <param name="texture_coords">�����Ķ����texture coords 1����</param>
        /// <param name="indices">�����Ķ�����������������</param>
        void InitMesh(const aiMesh* mesh,
            std::vector<glm::vec3>& positions,
            std::vector<glm::vec3>& normals,
            std::vector<glm::vec2>& texture_coords,
            std::vector<unsigned int>& indices);

        /// <summary>
        /// Initializes the materials.
        /// </summary>
        /// <param name="scene">The p scene.</param>
        /// <param name="file_name">The filename.</param>
        /// <returns>bool.</returns>
        bool InitMaterials(const aiScene* scene, const std::string& file_name);

        /// <summary>
        /// �ͷű�mesh�õ���һЩ��������
        /// </summary>
        void Clear();
    private:
        /// <summary>
        /// vertex attribute object handle
        /// </summary>
        GLuint                      vao_;

        /// <summary>
        /// �����洢vertex attribute�����ֶε�����buffer
        /// </summary>
        GLuint                      vertex_attribute_buffers_[6];

        /// <summary>
        /// һ��ģ���ļ��п��������ɸ���mesh��ɣ�ÿһ��mesh��Ӧ����һ�����
        /// ������洢����ɱ�mesh����mesh�������Ϣ
        /// </summary>
        std::vector<BasicMeshEntry> entries_;

        /// <summary>
        /// ��mesh�õ���texture����
        /// </summary>
        std::vector<TextureSPtr>    textures_;

        /// <summary>
        /// ��mesh��TRS��Ϣ
        /// </summary>
        Orientation                 transform_;
    };
}

#endif // kgl_basic_static_mesh_h__