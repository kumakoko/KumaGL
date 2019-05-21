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
#include "kgl_lib_pch.h"
#include "kgl_defines.h"
#include "kgl_sky_box.h"
#include "kgl_vertex_attribute.h"
#include "kgl_vertex_type.h"
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
        std::string model_path("resources/model/sphere.obj");
        model_ = new kgl::BasicStaticMesh;
        model_->LoadMesh(model_path);
        cubemap_texture_ = std::make_shared<CubemapTexture>(
            positive_x_file,negative_x_file,
            positive_y_file,negative_y_file,
            positive_z_file,negative_z_file);

        cubemap_texture_->Load();
    }

    void SkyBox::Draw()
    {
        glm::mat4 world_matrix;
        world_matrix = glm::translate(world_matrix, camera_->GetPosition()); // 以摄像机在世界坐标中的位置为天空球的原点
        // world_matrix = glm::scale(world_matrix, glm::vec3(5.0f, 5.0f, 5.0f));

        const glm::mat4& view_matrix = camera_->GetViewMatrix();
        const glm::mat4& projection_matrix = camera_->GetProjectionMatrix();

        gpu_program_->Use();
        gpu_program_->ApplyMatrix(glm::value_ptr(world_matrix), "world_matrix");
        gpu_program_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
        gpu_program_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
        gpu_program_->ApplyTexture(cubemap_texture_, "skybox_cubemap_texture", 0);
        model_->Render();
    }
}