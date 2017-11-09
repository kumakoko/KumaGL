// 天空盒
#include "kgl_lib_pch.h"
#include "kgl_defines.h"
#include "kgl_sky_box.h"
#include "kgl_vertex_attribute.h"
#include "kgl_camera.h"
#include "kgl_texture_manager.h"

namespace kgl
{
    SkyBox::SkyBox(CameraSPtr camera)
    {
        camera_ = camera;
    }

    SkyBox::~SkyBox()
    {
        KGL_SAFE_DELETE(model_);
        cubemap_texture_.reset();
        camera_.reset();
        gpu_program_.reset();
    }

    void SkyBox::LoadSkyBox(const std::string& positive_x_file,
        const std::string& negative_x_file,
        const std::string& positive_y_file,
        const std::string& negative_y_file,
        const std::string& positive_z_file,
        const std::string& negative_z_file)
    {
        // 初始化shader
        gpu_program_ = std::make_shared<kgl::GPUProgram>();
        gpu_program_->CreateFromFile("resources/shader/framework/sky_box_vs.glsl", "resources/shader/framework/sky_box_fs.glsl", nullptr);

        // 初始化模型。并把shader作用到模型中的每一个mesh上
        const char* model_path = "resources/model/sphere.obj";
        model_ = new kgl::StaticModel(kgl::VERTEX_TYPE_PN, model_path);

        cubemap_texture_ = std::make_shared<CubemapTexture>(
            positive_x_file,negative_x_file,
            positive_y_file,negative_y_file,
            positive_z_file,negative_z_file);

        cubemap_texture_->Load();
        
        std::size_t sz = model_->GetMeshCount();

        for (std::size_t s = 0; s < sz; ++s)
        {
            model_->ApplyShaderToMesh(s, gpu_program_);
        }
    }

    void SkyBox::Draw()
    {
        glm::mat4 world_matrix;
        world_matrix = glm::translate(world_matrix, camera_->GetPosition()); // 以摄像机在世界坐标中的位置为天空球的原点
        world_matrix = glm::scale(world_matrix, glm::vec3(5.0f, 5.0f, 5.0f));

        const glm::mat4& view_matrix = camera_->GetViewMatrix();
        const glm::mat4& projection_matrix = camera_->GetProjectionMatrix();

        gpu_program_->Use();
        gpu_program_->ApplyMatrix(glm::value_ptr(world_matrix), "world_matrix");
        gpu_program_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
        gpu_program_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
        gpu_program_->ApplyTexture(cubemap_texture_, "skybox_cubemap_texture", 0);
        model_->Draw();
    }
}