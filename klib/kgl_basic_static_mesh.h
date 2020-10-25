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
* \brief 静态网格类
*
* TODO: 静态网格类，提供从assimp库所支持的模型中载入一个
*       静态的mesh并渲染
*
* \note 静态的带有顶点索引值的mesh。mesh的属性需要包含有
*       position,texture coord1,normal
*       顶点属性的布局，在vertex shader中
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
        /// 无效的材质索引
        /// </summary>
        static const int INVALID_MATERIAL = 0xFFFFFFFF;
        
        /// <summary>
        /// vertex_attribute_buffers_数组中，顶点索引的attribute buffer排第0位
        /// </summary>
        static const int INDEX_BUFFER = 0;

        /// <summary>
        /// vertex_attribute_buffers_数组中，顶点位置坐标的attribute buffer排第1位
        /// </summary>
        static const int POS_VB = 1;

        /// <summary>
        /// vertex_attribute_buffers_数组中，顶点法线的attribute buffer排第2位
        /// </summary>
        static const int NORMAL_VB = 2;

        /// <summary>
        /// vertex_attribute_buffers_数组中，顶点第一层纹理坐标的attribute buffer排第3位
        /// </summary>
        static const int TEXCOORD_VB = 3;

        /// <summary>
        /// vertex_attribute_buffers_数组中，顶点的wvp变换矩阵的attribute buffer排第4位
        /// </summary>
        static const int WVP_MAT_VB = 4;

        /// <summary>
        /// vertex_attribute_buffers_数组中，顶点的world矩阵的attribute buffer排第4位
        /// </summary>
        static const int WORLD_MAT_VB = 5;

        /// <summary>
        /// 在vertex shader中，顶点中的position的layout值为0
        /// </summary>
        static const int POSITION_LOCATION = 0;
        
        /// <summary>
        /// 在vertex shader中，顶点中的第一层纹理映射坐标的layout值为1
        /// </summary>
        static const int TEX_COORD_LOCATION = 1;
        
        /// <summary>
        /// T在vertex shader中，顶点中的法线值的layout值为1
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
        /// <see cref="BasicStaticMesh"/>类的构造函数
        /// </summary>
        BasicStaticMesh();

        /// <summary>
        /// <see cref="BasicStaticMesh"/>类的析构函数
        /// </summary>
        ~BasicStaticMesh();

        /// <summary>
        /// Loads the mesh.
        /// </summary>
        /// <param name="file_name">The file_name.</param>
        /// <returns>bool.</returns>
        bool LoadMesh(const std::string& file_name);

		/*********************************************************
		渲染网格    
		*********************************************************/
		void Render();

        /// <summary>
        /// 以instance方式，渲染instance_count个本实例的实例
        /// </summary>
        /// <param name="instance_count">要渲染本mesh的实例的个数</param>
        /// <param name="wvp_matrix">存储着个待渲染的mesh实例的wvp矩阵的数组首指针</param>
        /// <param name="world_matrix">存储着个待渲染的mesh实例的world矩阵的数组首指针</param>
        void Render(uint32_t instance_count, const glm::mat4* wvp_matrix, const glm::mat4* world_matrix);

        /// <summary>
        /// 获取到本mesh的translation，rotation和scale
        /// </summary>
        /// <returns>kgl.Orientation &.</returns>
        inline Orientation& GetOrientation()
        {
            return transform_;
        }

    private:
        /// <summary>
        /// 从一个Assimp库提供的aiScene类型的场景中创建出本mesh
        /// </summary>
        /// <param name="scene">Assimp库提供的aiScene类型的场景</param>
        /// <param name="file_name">模型文件名</param>
        /// <returns>初始化成功返回true，否则返回false</returns>
        bool InitFromAssetScene(const aiScene* scene, const std::string& file_name);

        /// <summary>
        /// 根据import进来的aiMesh，解压出顶点的位置，法线，第一层纹理坐标属性，创建对应的缓冲区
        /// </summary>
        /// <param name="mesh">assimp从文件中读入的aiMesh对象</param>
        /// <param name="positions">待填充的顶点的position数组</param>
        /// <param name="normals">待填充的顶点的法线数组</param>
        /// <param name="texture_coords">待填充的顶点的texture coords 1数组</param>
        /// <param name="indices">待填充的顶点索引缓冲区数组</param>
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
        /// 释放本mesh用到的一些数据属性
        /// </summary>
        void Clear();
    private:
        /// <summary>
        /// vertex attribute object handle
        /// </summary>
        GLuint                      vao_;

        /// <summary>
        /// 用来存储vertex attribute描述字段的六个buffer
        /// </summary>
        GLuint                      vertex_attribute_buffers_[6];

        /// <summary>
        /// 一个模型文件中可能由若干个子mesh组成，每一个mesh都应该有一个入口
        /// 本数组存储着组成本mesh的子mesh的入口信息
        /// </summary>
        std::vector<BasicMeshEntry> entries_;

        /// <summary>
        /// 本mesh用到的texture集合
        /// </summary>
        std::vector<TextureSPtr>    textures_;

        /// <summary>
        /// 本mesh的TRS信息
        /// </summary>
        Orientation                 transform_;
    };
}

#endif // kgl_basic_static_mesh_h__