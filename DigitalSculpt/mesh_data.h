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
        /// �����洢vertex attribute�����ֶε�����buffer
        /// </summary>
        GLuint  vertex_attribute_buffers_[6];

        std::vector<BasicMeshEntry> entries_;
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




    public:
        /// <summary>
        /// �洢��������������ı����������
        /// </summary>
        Uint32Array _facesABCD;//: null, // faces tri or quad, tri will have D:Utils.TRI_INDEX (Uint32Array)

        /// <summary>
        /// ��mesh�е���ĸ���
        /// </summary>
        std::uint32_t _nbFaces;// : 0,

        // ÿ��������ĵ������б�
        Vec3Array _faceCentersXYZ;

        // ÿ����ķ����б�
        Vec3Array _faceNormalsXYZ;

        // ÿ����İ�Χ�е�����
        Float32Array    _faceBoxes;
    };
}
#endif // mesh_data_h__
