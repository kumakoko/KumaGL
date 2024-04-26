#ifndef mesh_data_h__
#define mesh_data_h__

#include "../klib/kgl_lib_pch.h"

namespace DigitalSculpt
{
    class MeshData
    {
    public:
        std::vector<glm::vec3> positions_;
        std::vector<glm::vec3> normals_;
        std::vector<glm::vec2> texture_coords_;
        std::vector<std::uint32_t> indices_;
    public:
        bool LoadMesh(const std::string& file_name);
    private:
        bool InitFromAssetScene(const aiScene* scene, const std::string& file_name);
        void InitMesh(const aiMesh* mesh)
    };
}
#endif // mesh_data_h__
