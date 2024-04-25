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

#include "digital_sculpt_app.h"

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_vertex_type.h"
#include "../klib/kgl_texture_manager.h"

DigitalSculptApp::DigitalSculptApp() :model_(nullptr)
{

}

DigitalSculptApp::~DigitalSculptApp()
{
    KGL_SAFE_DELETE(model_);
    model_shader_.reset();
}

void DigitalSculptApp::InitScene()
{
    App::InitScene();
    rs_depth_.SetEnable(GL_TRUE);
    rs_blend_.SetBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void DigitalSculptApp::InitMainCamera()
{
    glm::vec3 camera_pos(0.0f, 1.78f, 6.11f);
    float pitch_angle = -18.5f;
    float move_speed = 0.1f;
    float max_yaw_degree_per_frame = 1.0f;
    float max_pitch_degree_per_frame = 1.0f;
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, camera_pos, pitch_angle, 180.0f, 120.f, 0.1f, 1000.0f);
    main_camera_->SetCameraSpeed(move_speed);
    main_camera_->SetMaxYawDegreePerFrame(max_yaw_degree_per_frame);
    main_camera_->SetMaxPitchDegreePerFrame(max_pitch_degree_per_frame);
    main_camera_->SetPitchAngle(pitch_angle);
}

void DigitalSculptApp::InitModel()
{
    const char* model_path = "resources/model/box2/box.obj";
    model_ = new kgl::BasicStaticMesh;
    model_->LoadMesh(std::string(model_path));
}

void DigitalSculptApp::InitShaders()
{
    const GLchar* vs_file_path = "resources/shader/009_assimp_vs.glsl";
    const GLchar* fs_file_path = "resources/shader/009_assimp_fs.glsl";
    const GLchar* gs_file_path = nullptr;
    model_shader_ = std::make_shared<kgl::GPUProgram>();
    model_shader_->CreateFromFile(vs_file_path, fs_file_path, gs_file_path);
}

void DigitalSculptApp::RenderScene()
{
    main_camera_->Update();

    glm::mat4 model_matrix = glm::identity<glm::mat4>();
    model_matrix = glm::scale(model_matrix, glm::vec3(box_scale_factor_));
    model_matrix = glm::rotate(model_matrix, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
    const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();

    rs_blend_.Use();
    rs_depth_.Use();

    model_shader_->Use();
    model_shader_->ApplyMatrix(glm::value_ptr(model_matrix), "model_matrix");
    model_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
    model_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
    model_->Render();
}

void DigitalSculptApp::RenderGUI()
{
    const glm::vec3& camera_pos = main_camera_->GetPosition();
    ImGui::Begin("009 Assimp -- 使用Assimp库装载模型");
    ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("按WSADUJ键向前后左右上下移动摄像机");
    ImGui::Text("摄像机坐标: (%.1f,%.1f,%.1f)", camera_pos.x, camera_pos.y, camera_pos.z);
    ImGui::SliderFloat("缩放系数值", &box_scale_factor_, 0.1f, 2.0f);
    ImGui::End();
}

void DigitalSculptApp::ProcessInput()
{
    if (key_state_[GLFW_KEY_W])
    {
        main_camera_->Move(kgl::CameraDirection::FORWARD);
    }

    if (key_state_[GLFW_KEY_S])
    {
        main_camera_->Move(kgl::CameraDirection::BACK);
    }

    if (key_state_[GLFW_KEY_A])
    {
        main_camera_->Move(kgl::CameraDirection::LEFT);
    }

    if (key_state_[GLFW_KEY_D])
    {
        main_camera_->Move(kgl::CameraDirection::RIGHT);
    }

    if (key_state_[GLFW_KEY_U])
    {
        main_camera_->Move(kgl::CameraDirection::UP);
    }

    if (key_state_[GLFW_KEY_J])
    {
        main_camera_->Move(kgl::CameraDirection::DOWN);
    }
}