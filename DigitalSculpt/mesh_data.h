#ifndef mesh_data_h__
#define mesh_data_h__

#include "../klib/kgl_lib_pch.h"
#include "utils.h"

namespace DigitalSculpt
{
    class MeshData
    {
    private:
        struct BasicMeshEntry
        {
            BasicMeshEntry()
            {
                NumIndices = 0;
                BaseVertex = 0;
                BaseIndex = 0;
                MaterialIndex = INVALID_MATERIAL;
            }

            std::uint32_t NumIndices;
            std::uint32_t BaseVertex;
            std::uint32_t BaseIndex;
            std::uint32_t MaterialIndex;
        };
    public:
        std::vector<glm::vec3> positions_;
        std::vector<glm::vec3> normals_;
        std::vector<glm::vec2> texture_coords_;
        std::vector<std::uint32_t> indices_;
    public:
        bool LoadMesh(const std::string& file_name);
        void Clear();
        void Render();
    private:
        bool InitFromAssetScene(const aiScene* scene, const std::string& file_name);
        void InitMesh(const aiMesh* mesh);
    private:
        /// <summary>
        /// vertex attribute object
        /// </summary>
        GLuint  vao_;

        /// <summary>
        /// 用来存储vertex attribute描述字段的六个buffer
        /// </summary>
        GLuint  vertex_attribute_buffers_[6];

        std::vector<BasicMeshEntry> entries_;
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




    public:
        /// <summary>
        /// 存储着三角形面或者四边形面的数组
        /// </summary>
        Uint32Array _facesABCD;//: null, // faces tri or quad, tri will have D:Utils.TRI_INDEX (Uint32Array)

        /// <summary>
        /// 本mesh中的面的个数
        /// </summary>
        std::uint32_t _nbFaces;// : 0,

        // 每个面的中心点坐标列表
        Vec3Array _faceCentersXYZ;

        // 每个面的法线列表
        Vec3Array _faceNormalsXYZ;

        // 每个面的包围盒的数组
        Float32Array    _faceBoxes;
    };
}
#endif // mesh_data_h__
