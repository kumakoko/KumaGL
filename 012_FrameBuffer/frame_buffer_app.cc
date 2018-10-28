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
#include "frame_buffer_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_debug_tool.h"

// http://blog.csdn.net/dreamcs/article/details/7691690

FrameBufferApp::FrameBufferApp() :floor_shader_(nullptr), screen_shader_(nullptr)
{
    floor_translation_ = glm::vec3(0.0f, 0.3f, -0.3f);
}

FrameBufferApp::~FrameBufferApp()
{
    KGL_SAFE_DELETE(floor_shader_);
    KGL_SAFE_DELETE(screen_shader_);
    KGL_SAFE_DELETE(frame_buffer_);
    KGL_SAFE_DELETE(wood_box_shader_);
    box_.reset();
    floor_.reset();
    floor_texture_.reset();
    box_texture_.reset();
}

void FrameBufferApp::InitScene()
{
    App::InitScene();
    
    cull_mode_.SetEnable(GL_TRUE);
    cull_mode_.SetFrontFaceMode(GL_CW);
    cull_mode_.SetCullMode(GL_BACK);

    depth_test_mode_.SetEnable(GL_TRUE);
    depth_test_mode_.SetDepthTestFunc(GL_LESS);

    draw_fb_to_scr_cull_.SetEnable(GL_FALSE);
    draw_fb_to_scr_cull_.SetCullMode(GL_BACK); // 指定删除背面
    draw_fb_to_scr_cull_.SetFrontFaceMode(GL_CCW); // 待渲染的模型，以逆时针为正面

    draw_fb_to_scr_depth_.SetEnable(GL_FALSE); // 关闭深度测试
}

void FrameBufferApp::PreRenderFrame()
{
    App::PreRenderFrame();
}

void FrameBufferApp::RenderFrame()
{
    main_camera_->Update();
    GLfloat current_time = (GLfloat)glfwGetTime();

// ======================== 写入到离屏的frame buffer =============================

    frame_buffer_->StartWriting(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        
    const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
    const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();

    cull_mode_.Use();
    depth_test_mode_.Use();

    // 绘制太阳
    glm::mat4 matrix_sun;
    matrix_sun = glm::scale(matrix_sun, glm::vec3(5.f, 5.f, 5.f));
    matrix_sun = glm::rotate(matrix_sun, current_time, glm::vec3(0.0f, 1.0f, 0.0f));
    scene_shader_->Use();
    scene_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
    scene_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
    scene_shader_->ApplyMatrix(glm::value_ptr(matrix_sun), "world_matrix");
    scene_shader_->ApplyTexture(texture_sun_, "source_texture_1", 0);
    sun_->DrawIndexed();

    // 绘制地板
    glm::mat4 matrix_floor;
    matrix_floor = glm::rotate(matrix_floor, -glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix_floor = glm::scale(matrix_floor, glm::vec3(20.f, 20.f, 20.f));
    matrix_floor = glm::translate(matrix_floor, floor_translation_);
    scene_shader_->ApplyMatrix(glm::value_ptr(matrix_floor), "world_matrix");
    scene_shader_->ApplyTexture(floor_texture_, "source_texture_1", 0);
    floor_->DrawIndexed();

    // 结束向离屏frame buffer的写入
    frame_buffer_->EndWriting();

// ======================== 直接写入到屏幕对应的主frame buffer =============================
    scene_shader_->ApplyMatrix(glm::value_ptr(matrix_sun), "world_matrix");
    scene_shader_->ApplyTexture(texture_sun_, "source_texture_1", 0);
    sun_->DrawIndexed();

    scene_shader_->ApplyMatrix(glm::value_ptr(matrix_floor), "world_matrix");
    scene_shader_->ApplyTexture(floor_texture_, "source_texture_1", 0);
    floor_->DrawIndexed();
// ======================== 把离屏的frame buffer的内容绘制到屏幕 =============================

    draw_fb_to_scr_cull_.Use();
    draw_fb_to_scr_depth_.Use();
    screen_shader_->Use();
    screen_shader_->ApplyTexture(frame_buffer_->GetTexture(), "source_texture_1", 0);
    screen_->Draw();
}

void FrameBufferApp::InitMaterials()
{
    App::InitMaterials();
}

void FrameBufferApp::InitModel()
{
    kgl::TextureParams texture_param = 
        kgl::TextureManager::MakeTextureParamsRGB(GL_REPEAT, GL_LINEAR);

    texture_sun_ = std::make_shared<kgl::SourceTexture>();
    texture_sun_->CreateFromFile("resources/image/sun.jpg", texture_param);

    sun_ = kgl::PrimitiveTool::BuildSphere(10, true);

    // 初始化帧缓冲对象
    frame_buffer_ = new kgl::FrameBuffer;
    frame_buffer_->Create(800, 600);

    // 初始化图元
    screen_ = kgl::PrimitiveTool::BuildNDCTexturedRectange(0.5f,1.f,0.5f,1.f);

    // 地板图元和贴图
    floor_ = kgl::PrimitiveTool::BuildSquare(10, true);
    floor_texture_ = std::make_shared<kgl::SourceTexture>();
    floor_texture_->CreateFromFile("resources/image/wood_box.jpg", texture_param);
}

void FrameBufferApp::InitShaders()
{
    const char* vs_file = "resources/shader/003_transform_draw_vs.glsl";
    const char* fs_file = "resources/shader/003_transform_draw_fs.glsl";
    scene_shader_ = std::make_shared<kgl::GPUProgram>();
    scene_shader_->CreateFromFile(vs_file, fs_file, nullptr);

    vs_file = "resources/shader/012_framebuffer_textured_ndc_rectangle_vs.glsl";
    fs_file = "resources/shader/012_framebuffer_textured_ndc_rectangle_fs.glsl";
    screen_shader_ = new kgl::GPUProgram;
    screen_shader_->CreateFromFile(vs_file, fs_file, nullptr);
}

void FrameBufferApp::InitLights()
{
    App::InitLights();
}

void FrameBufferApp::InitMainCamera()
{
    glm::vec3 pos = glm::vec3(0.0f, 5.0f, 28.0f);
    glm::vec3 look_at = glm::vec3(0.0f, 0.0f, 0.0f);
    float pitch_angle =-10.0f;
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, pos, pitch_angle);
}

void FrameBufferApp::InitFont()
{
    App::InitFont();
}

void FrameBufferApp::ProcessInput()
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

    if (key_state_[GLFW_KEY_LEFT])
    {
        main_camera_->ChangeYaw(-0.1f);
    }

    if (key_state_[GLFW_KEY_RIGHT])
    {
        main_camera_->ChangeYaw(0.1f);
    }

    if (key_state_[GLFW_KEY_UP])
    {
        main_camera_->ChangePitch(0.1f);
    }

    if (key_state_[GLFW_KEY_DOWN])
    {
        main_camera_->ChangePitch(-0.1f);
    }
}

void FrameBufferApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    App::OnKeyAction(window, key, scancode, action, mode);
}

void FrameBufferApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    App::OnMouseAction(window, xpos, ypos);
}

void FrameBufferApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
    App::OnScrollAction(window, xoffset, yoffset);
}