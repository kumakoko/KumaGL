/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

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
// 展示使用HDR的技术
#include "hdr_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_rendered_texture.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_string_convertor.h"
#include "../klib/kgl_texture.h"

HDRApp::HDRApp()
{
}

HDRApp::~HDRApp()
{
    KGL_SAFE_DELETE(hdr_buffer_);
    KGL_SAFE_DELETE(lighting_shader_);
    KGL_SAFE_DELETE(hdr_shader_);
    wood_texture_.reset();
    screen_rectangle_.reset();
    cube_.reset();
}

void HDRApp::InitScene()
{
    App::InitScene();
    hdr_buffer_ = new kgl::FrameBuffer;
    hdr_buffer_->Create(window_width_, window_height_, kgl::RenderedTexelType::RGBA16F);
    rs_depth_.SetEnable(GL_TRUE);
}

void HDRApp::PreRenderFrame()
{
    App::PreRenderFrame();
}

void HDRApp::RenderScene()
{
    main_camera_->Update();

    const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
    const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
    const glm::vec3& camera_position = main_camera_->GetPosition();

    glm::vec4 clear_color(0.0f, 0.0f, 0.0f, 1.0f);
    hdr_buffer_->StartWriting(clear_color);
    lighting_shader_->Use();
    lighting_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "u_projection_matrix");
    lighting_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "u_view_matrix");

    // 设置灯光的参数
    for (unsigned int i = 0; i < light_positions_.size(); i++)
    {
        lighting_shader_->ApplyVector3(glm::value_ptr(light_positions_[i]),("lights[" + std::to_string(i) + "].Position").c_str());
        lighting_shader_->ApplyVector3(glm::value_ptr(light_colors_[i]), ("lights[" + std::to_string(i) + "].Color").c_str());
    }

    // 绘制盒子内部
    glm::mat4 model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0f));
    model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
    lighting_shader_->ApplyMatrix(glm::value_ptr(model),"u_world_matrix");
    lighting_shader_->ApplyInt(1,"u_inverse_normals"); // 对盒子的法线进行取反，使得能看到盒子内部
    lighting_shader_->ApplyTexture(wood_texture_, "u_diffuse_texture", 0);
    cube_->Draw();
    hdr_buffer_->EndWriting();

    // 把G-buffer的内容绘制到和屏幕大小相等的图元上，并绘制到默认缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    hdr_shader_->Use();
    hdr_shader_->ApplyInt(use_hdr_ ? 1 : 0,"u_use_hdr");
    hdr_shader_->ApplyTexture(hdr_buffer_->GetTexture(), "u_hdr_buffer", 0);
    hdr_shader_->ApplyFloat(exposure_,"u_exposure");
    screen_rectangle_->Draw();
}

void HDRApp::InitMaterials()
{
    App::InitMaterials();
}

void HDRApp::InitModel()
{
    // 初始化NDC矩形图元
    screen_rectangle_ = kgl::PrimitiveTool::BuildNDCTexturedRectange();
    
    kgl::TextureParams texture_param;
    texture_param.wrap_s_mode = GL_REPEAT;
    texture_param.wrap_t_mode = GL_REPEAT;
    texture_param.mag_filter_mode = GL_LINEAR;
    texture_param.min_filter_mode = GL_NEAREST;
    texture_param.internal_format = GL_SRGB; // 要用sRGB
    texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
    texture_param.src_img_format = GL_BGR;//GL_RGB;
    texture_param.used_mipmap = true;
    // 初始化表示场景的盒子和纹理
    wood_texture_ = std::make_shared<kgl::SourceTexture>();
    wood_texture_->CreateFromFile("resources/image/wood.png", texture_param);
    cube_ = kgl::PrimitiveTool::BuildNDCTextureNormalCube();
}

void HDRApp::InitShaders()
{
    const GLchar* vs_file = "resources/shader/022_lighting_vs.glsl";
    const GLchar* fs_file = "resources/shader/022_lighting_fs.glsl";
    lighting_shader_ = new kgl::GPUProgram;
    lighting_shader_->CreateFromFile(vs_file, fs_file,nullptr);

    vs_file = "resources/shader/022_hdr_vs.glsl";
    fs_file = "resources/shader/022_hdr_fs.glsl";
    hdr_shader_ = new kgl::GPUProgram;
    hdr_shader_->CreateFromFile(vs_file, fs_file, nullptr);
}

void HDRApp::InitLights()
{
    light_positions_.push_back(glm::vec3(0.0f, 0.0f, 2.5f));
    light_positions_.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
    light_positions_.push_back(glm::vec3(0.0f, -1.8f, 14.0f));
    light_positions_.push_back(glm::vec3(0.8f, -1.7f, 26.0f));
    
    light_colors_.push_back(glm::vec3(100.0f, 100.0f, 100.0f)); // 用一个远远超出1的光
    light_colors_.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
    light_colors_.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
    light_colors_.push_back(glm::vec3(0.0f, 0.1f, 0.0f));
}

void HDRApp::InitMainCamera()
{
    glm::vec3 camera_pos(0.0f, 0.0f,25.0f);
    float pitch_angle = 0.0f;
    float move_speed = 0.1f;
    float max_yaw_degree_per_frame = 1.0f;
    float max_pitch_degree_per_frame = 1.0f;
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, camera_pos);
    main_camera_->SetCameraSpeed(move_speed);
    main_camera_->SetMaxYawDegreePerFrame(max_yaw_degree_per_frame);
    main_camera_->SetMaxPitchDegreePerFrame(max_pitch_degree_per_frame);
    main_camera_->SetPitchAngle(pitch_angle);
}

void HDRApp::RenderGUI()
{
	const glm::vec3& camera_pos = main_camera_->GetPosition();
	ImGui::Begin("022 HDR 高动态范围光效果演示");
	ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("摄像机坐标: (%.1f,%.1f,%.1f)", camera_pos.x, camera_pos.y, camera_pos.z);
    ImGui::Text("曝光度值：%.1f",exposure_);
	ImGui::Text("按WSADUJ键向前后左右上下移动摄像机,按下左右箭头键，增大或减小曝光度值");
    ImGui::Checkbox("打开HDR效果", &use_hdr_);
	ImGui::End();
}

void HDRApp::ProcessInput()
{
    if (key_state_[GLFW_KEY_W])
    {
        main_camera_->Move(kgl::CameraDirection::FORWARD);
        const glm::vec3& pos = main_camera_->GetPosition();

        if (pos.z <= 3.f)
        {
            main_camera_->SetPosition(glm::vec3(pos.x, pos.y, 3.f));
        }
    }

    if (key_state_[GLFW_KEY_S])
    {
        main_camera_->Move(kgl::CameraDirection::BACK);
        const glm::vec3& pos = main_camera_->GetPosition();

        if (pos.z >= 45.f)
        {
            main_camera_->SetPosition(glm::vec3(pos.x, pos.y, 45.f));
        }
    }

    if (key_state_[GLFW_KEY_U])
    {
        exposure_ += 0.01f;

        if (exposure_ >= 5.0f)
            exposure_ = 5.0f;
    }

    if (key_state_[GLFW_KEY_J])
    {
        exposure_ -= 0.01f;

        if (exposure_ < 0.0f)
            exposure_ = 0.0f;
    }
}

void HDRApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    App::OnKeyAction(window, key, scancode, action, mode);
}

void HDRApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    App::OnMouseAction(window, xpos, ypos);
}

void HDRApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
    App::OnScrollAction(window, xoffset, yoffset);
}