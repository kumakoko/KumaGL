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
#include "particle_system_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_source_texture.h"

ParticleSystemApp::ParticleSystemApp()
{
    particle_generated_pos_[0] = 0.0f;
    particle_generated_pos_[1] = 0.0f;
    particle_generated_pos_[2] = 0.0f;
    particle_color_[0] = 0.234f;
    particle_color_[1] = 0.536f;
    particle_color_[2] = 0.336f;

    particle_min_velocity_[0] = -5.0f;
    particle_min_velocity_[1] =  0.0f;
    particle_min_velocity_[2] = -5.0f;

    particle_max_velocity_[0] = 5.0f;
    particle_max_velocity_[1] = 10.0f;
    particle_max_velocity_[2] = 5.0f;

    particle_gravity_[0] = 0.0f;
    particle_gravity_[1] = -5.0f;
    particle_gravity_[2] = 0.0f;

    particle_min_life_ = 1.5f;
    particle_max_life_ = 3.0f;
    particle_render_size_ = 0.2f;
    particle_spawn_time_interval_ = 0.05f;
    particle_spawn_num_once_ = 10;

    particle_number_ = 0;
}

ParticleSystemApp::~ParticleSystemApp()
{
    KGL_SAFE_DELETE(ps_);
}

void ParticleSystemApp::InitMainCamera()
{
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 30.0f);
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, pos);
}

void ParticleSystemApp::RenderGUI()
{
    const glm::vec3& camera_pos = main_camera_->GetPosition();
    ImGui::Begin("粒子系统");
    ImGui::Text("W键向前，S键向后移动摄像机");
    ImGui::Text("摄像机坐标: (%.1f,%.1f,%.1f)", camera_pos.x, camera_pos.y, camera_pos.z);
    ImGui::Text("当前粒子个数: %d", particle_number_);
    ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
    ImGui::SliderFloat3("颜色", particle_color_, 0.0f, 1.0f);
    ImGui::SliderFloat3("最小速度", particle_min_velocity_, -10.0f, 0.0f);
    ImGui::SliderFloat3("最大速度", particle_max_velocity_, 0.0f, 10.0f);
    ImGui::SliderFloat3("所受重力", particle_gravity_, -10.0f, 0.0f);
    ImGui::SliderFloat("最小生命期", &particle_min_life_, 1.0f, 3.0f);
    ImGui::SliderFloat("最大生命期", &particle_max_life_, 5.0f, 10.0f);
    ImGui::SliderFloat("大小尺寸", &particle_render_size_, 0.1f, 1.0f);
    ImGui::SliderFloat("生成时间间隔", &particle_spawn_time_interval_, 0.1f, 1.0f);
    ImGui::SliderInt("每批生成个数", &particle_spawn_num_once_, 3, 33);
    ImGui::Checkbox("线框模式显示", &wireframe_mode_);
    ImGui::End();
}

void ParticleSystemApp::PreRenderFrame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ParticleSystemApp::InitScene()
{
    App::InitScene();

    const char* update_vs_file_path = "resources/shader/050_particle_system_update_vs.glsl";
    const char* update_gs_file_path = "resources/shader/050_particle_system_update_gs.glsl";
    const char* render_vs_file_path = "resources/shader/050_particle_system_render_vs.glsl";
    const char* render_gs_file_path = "resources/shader/050_particle_system_render_gs.glsl";
    const char* render_fs_file_path = "resources/shader/050_particle_system_render_fs.glsl";

    std::vector<const char*> varings(6);
    varings =
    {
        "vPositionOut",
        "vVelocityOut",
        "vColorOut",
        "fLifeTimeOut",
        "fSizeOut",
        "iTypeOut",
    };

    kgl::TextureParams texture_param;
    texture_param.wrap_s_mode = GL_REPEAT;
    texture_param.wrap_t_mode = GL_REPEAT;
    texture_param.mag_filter_mode = GL_LINEAR;
    texture_param.min_filter_mode = GL_NEAREST;
    texture_param.internal_format = GL_RGB;
    texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
    texture_param.src_img_format = GL_BGR;//GL_RGB;
    texture_param.used_mipmap = false;

    kgl::SourceTextureSPtr particle_texture = std::make_shared<kgl::SourceTexture>();
    particle_texture->CreateFromFile("resources/image/particle.png", texture_param);

    ps_ = new kgl::GPUParticleSystem;
    ps_->InitalizeParticleSystem(update_vs_file_path, update_gs_file_path, render_vs_file_path, render_gs_file_path, render_fs_file_path, varings);
    ps_->SetParticleTexture(particle_texture);
    ps_->SetGeneratorProperties(
        glm::vec3(particle_generated_pos_[0], particle_generated_pos_[1], particle_generated_pos_[2]),  // 粒子发射器的生成位置点
        glm::vec3(particle_min_velocity_[0], particle_min_velocity_[1], particle_min_velocity_[2]),     // 最小发射速度
        glm::vec3(particle_max_velocity_[0], particle_max_velocity_[1], particle_max_velocity_[2]),     // 最大发射速度
        glm::vec3(particle_gravity_[0], particle_gravity_[1], particle_gravity_[2]),                    // 作用于粒子上的重力值
        glm::vec3(particle_color_[0], particle_color_[1], particle_color_[2]),                          // 粒子颜色，Color (light blue)
        particle_min_life_,                                                                             // 粒子的最小生存时间，单位秒
        particle_max_life_,                                                                             // 粒子的最大生存时间，单位秒
        particle_render_size_,                                                                          // Rendered size
        particle_spawn_time_interval_,                                                                  // Spawn every 0.05 seconds
        particle_spawn_num_once_);                                                                      // 生成30个粒子
}

void ParticleSystemApp::RenderScene()
{
    draw_mode_.SetCurrentDrawMode(wireframe_mode_ ? kgl::DM_LINE : kgl::DM_FILL);

    current_frame_time_ = glfwGetTime();
    main_camera_->Update();

    const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
    const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
    const glm::vec3& camera_forward_dir = main_camera_->GetForwardDirection();
    const glm::vec3& camera_up_dir = main_camera_->GetUpDirection();

    ps_->SetGeneratorProperties(
        glm::vec3(particle_generated_pos_[0], particle_generated_pos_[1], particle_generated_pos_[2]),  // 粒子发射器的生成位置点
        glm::vec3(particle_min_velocity_[0], particle_min_velocity_[1], particle_min_velocity_[2]),     // 最小发射速度
        glm::vec3(particle_max_velocity_[0], particle_max_velocity_[1], particle_max_velocity_[2]),     // 最大发射速度
        glm::vec3(particle_gravity_[0], particle_gravity_[1], particle_gravity_[2]),                    // 作用于粒子上的重力值
        glm::vec3(particle_color_[0], particle_color_[1], particle_color_[2]),                            // 粒子颜色，Color (light blue)
        particle_min_life_,                                                                             // 粒子的最小生存时间，单位秒
        particle_max_life_,                                                                             // 粒子的最大生存时间，单位秒
        particle_render_size_,                                                                          // Rendered size
        particle_spawn_time_interval_,                                                                  // Spawn every 0.05 seconds
        particle_spawn_num_once_);                                                                      // 生成30个粒子
    ps_->SetMatrices(view_matrix, projection_matrix, camera_forward_dir, camera_up_dir);
    ps_->UpdateParticles(static_cast<float>(current_frame_time_ - last_frame_time_));
    ps_->RenderParticles();
    particle_number_ = ps_->GetNumParticles();
    last_frame_time_ = current_frame_time_;
}

void ParticleSystemApp::ProcessInput()
{
    if (key_state_[GLFW_KEY_W])
    {
        main_camera_->Move(kgl::CameraDirection::FORWARD);
    }

    if (key_state_[GLFW_KEY_S])
    {
        main_camera_->Move(kgl::CameraDirection::BACK);
    }
}