// 展示全屏模糊的渲染技术
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
#include "blur_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_debug_tool.h"

BlurApp::BlurApp()
{
}

BlurApp::~BlurApp()
{
    KGL_SAFE_DELETE(model_);
    KGL_SAFE_DELETE(frame_buffer_);
    model_shader_.reset();
    blur_shader_.reset();
}

void BlurApp::InitModel()
{
    const char* model_path = "resources/model/box2/box.obj";
    model_ = new kgl::BasicStaticMesh;
    model_->LoadMesh(std::string(model_path));
    rs_depth_.Use();

    // 初始化帧缓冲对象
    frame_buffer_ = new kgl::FrameBuffer;
    frame_buffer_->Create(1024, 768);

    // 初始化图元
    screen_ = kgl::PrimitiveTool::BuildNDCTexturedRectange();
}

void BlurApp::InitShaders()
{
    model_shader_ = std::make_shared<kgl::GPUProgram>();
    model_shader_->CreateFromFile("resources/shader/015_draw_model_vs.glsl", 
        "resources/shader/015_draw_model_fs.glsl", nullptr);
    blur_shader_ = std::make_shared<kgl::GPUProgram>();
    blur_shader_->CreateFromFile("resources/shader/015_blur_effect_vs.glsl",
        "resources/shader/015_blur_effect_fs.glsl", nullptr);
}

void BlurApp::InitScene()
{
    App::InitScene();

    draw_fb_to_scr_cull_.SetEnable(GL_FALSE);
    draw_fb_to_scr_cull_.SetCullMode(GL_BACK); // 指定删除背面
    draw_fb_to_scr_cull_.SetFrontFaceMode(GL_CCW); // 待渲染的模型，以逆时针为正面
    draw_fb_to_scr_depth_.SetEnable(GL_FALSE); // 关闭深度测试
}

void BlurApp::InitMainCamera()
{
    glm::vec3 camera_pos(0.0f, 0.000005f, -8.799932f);
    float pitch_angle = 0.0f;
    float yaw_angle = 0.f;// 180.0f;
    float fov = 120.f;
    float near_clip_distance = 0.1f;
    float far_clip_distance = 1000.0f;

    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, camera_pos,
        pitch_angle, yaw_angle, fov, near_clip_distance, far_clip_distance);
}

void BlurApp::InitLights()
{
    directional_light_.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    directional_light_.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    directional_light_.Direction = glm::vec3(0.0f, 0.0f, -1.0f);
    directional_light_.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void BlurApp::InitMaterials()
{
    // 白银材质
    material_.Ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f);
    material_.Diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
    material_.Specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);
    material_.Shininess = 0.4f * 128.f;
}

void BlurApp::RenderScene()
{
    main_camera_->Update();

    // ======================== 写入到离屏的frame buffer =============================
    frame_buffer_->StartWriting(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    glm::mat4 world_matrix;
    world_matrix = glm::rotate(world_matrix, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
    world_matrix = glm::rotate(world_matrix, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
    const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
    const glm::vec3& view_pos = main_camera_->GetPosition();

    rs_depth_.SetEnable(true);
    model_shader_->Use();
    model_shader_->ApplyMatrix(glm::value_ptr(world_matrix), "model_matrix");
    model_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
    model_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
    model_shader_->ApplyTexture("texture_diffuse_1", 0);
    model_->Render();

    // 结束向离屏frame buffer的写入
    frame_buffer_->EndWriting();

    // ======================== 把离屏的frame buffer的内容绘制到屏幕 =============================
    blur_shader_->Use();
    blur_shader_->ApplyTexture(frame_buffer_->GetTexture(), "image", 0);
    blur_shader_->ApplyInt(blur_horizontal_, "horizontal");
    blur_shader_->ApplyInt(use_blur_, "use_blur");
    screen_->Draw();
}

void BlurApp::ProcessInput()
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

    if (key_state_[GLFW_KEY_J])
    {
        main_camera_->Move(kgl::CameraDirection::UP);
    }

    if (key_state_[GLFW_KEY_U])
    {
        main_camera_->Move(kgl::CameraDirection::DOWN);
    }

    if (key_state_[GLFW_KEY_R])
    {
        main_camera_->ChangeYaw(0.01f);
    }

    if (key_state_[GLFW_KEY_UP])
    {
        main_camera_->ChangePitch(0.01f);
    }

    if (key_state_[GLFW_KEY_DOWN])
    {
        main_camera_->ChangePitch(-0.01f);
    }

    if (key_state_[GLFW_KEY_LEFT])
    {
        main_camera_->ChangeYaw(0.01f);
    }

    if (key_state_[GLFW_KEY_RIGHT])
    {
        main_camera_->ChangeYaw(-0.01f);
    }
}

void BlurApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_P  && action == GLFW_RELEASE)
    {
        use_blur_ = -use_blur_;
    }

    if (key == GLFW_KEY_H  && action == GLFW_RELEASE)
    {
        blur_horizontal_ = -blur_horizontal_;
    }

    App::OnKeyAction(window, key, scancode, action, mode);
}

void BlurApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    App::OnMouseAction(window, xpos, ypos);
}

void BlurApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
    App::OnScrollAction(window, xoffset, yoffset);
}