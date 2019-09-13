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
/*!
 * \file shadow_mapping_app.cc
 * \date 2018/10/29 20:18
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: 展示使用HDR的技术
 *
 * \note
*/
#include "shadow_mapping_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_rendered_texture.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_string_convertor.h"
#include "../klib/kgl_texture.h"

ShadowMappingApp::ShadowMappingApp()
{
}

ShadowMappingApp::~ShadowMappingApp()
{
    KGL_SAFE_DELETE(shadow_depth_buffer_);
    KGL_SAFE_DELETE(shadow_mapping_depth_pass_);
    KGL_SAFE_DELETE(shadow_mapping_pass_);
    KGL_SAFE_DELETE(debug_screen_shader_);
    floor_.reset();
    box_.reset();
    wood_texture_.reset();
}

void ShadowMappingApp::InitScene()
{
    App::InitScene();

    this->CreateFrameBufferForDepth();

    const GLchar* vs_file = "resources/shader/025_shadow_mapping_vs.glsl";
    const GLchar* fs_file = "resources/shader/025_shadow_mapping_fs.glsl";
    shadow_mapping_pass_ = new RenderPassShadowMapping;
    shadow_mapping_pass_->CreateAndAttachGPUProgram(vs_file, fs_file);
    shadow_mapping_pass_->Initialize();

    vs_file = "resources/shader/025_shadow_mapping_depth_vs.glsl";
    fs_file = "resources/shader/025_shadow_mapping_depth_fs.glsl";
    shadow_mapping_depth_pass_ = new RenderPassShadowMappingDepth(window_width_,window_height_);
    shadow_mapping_depth_pass_->CreateAndAttachGPUProgram(vs_file, fs_file);
    shadow_mapping_depth_pass_->Initialize();

    rs_depth_.SetEnable(GL_TRUE);

    cull_mode_draw_box_.SetEnable(GL_TRUE);
    cull_mode_draw_box_.SetFrontFaceMode(GL_CCW);
    cull_mode_draw_box_.SetCullMode(GL_BACK);

    cull_mode_draw_floor_.SetEnable(GL_TRUE);
    cull_mode_draw_floor_.SetFrontFaceMode(GL_CW);
    cull_mode_draw_floor_.SetCullMode(GL_BACK);

    draw_fb_to_scr_cull_.SetEnable(GL_FALSE);
    draw_fb_to_scr_cull_.SetCullMode(GL_BACK); // 指定删除背面
    draw_fb_to_scr_cull_.SetFrontFaceMode(GL_CCW); // 待渲染的模型，以逆时针为正面

    draw_fb_to_scr_depth_.SetEnable(GL_FALSE); // 关闭深度测试
}

void ShadowMappingApp::CreateFrameBufferForDepth()
{
    const int32_t BUFFER_WIDTH = 1024;
    const int32_t BUFFER_HEIGHT = 1024;
    shadow_depth_buffer_ = new kgl::FrameBuffer;
    shadow_depth_buffer_->Create(BUFFER_WIDTH, BUFFER_HEIGHT, kgl::RenderedTexelType::DEPTH_COMPONENT, false);
}

void ShadowMappingApp::RenderScene()
{
    main_camera_->Update();
    this->RenderSceneDepthToBuffer(); // 从灯光角度来观察，记录下深度信息
    this->RenderSceneWithShadow();
    this->RenderDebugScreen();
    this->RenderHelpText(main_camera_->GetPosition());
}

void ShadowMappingApp::RenderDebugScreen()
{
    float near_plane = 1.0f, far_plane = 7.5f;
    draw_fb_to_scr_cull_.Use();
    draw_fb_to_scr_depth_.Use();
    debug_screen_shader_->Use();
    debug_screen_shader_->ApplyFloat(near_plane,"u_near_plane");
    debug_screen_shader_->ApplyFloat(far_plane,"u_far_plane");
    debug_screen_shader_->ApplyTexture(shadow_depth_buffer_->GetTexture(), "u_depth_map", 0);
    debug_screen_->Draw();
}

void ShadowMappingApp::PreRenderFrame()
{
    
}

void ShadowMappingApp::RenderSceneWithShadow()
{
    glViewport(0, 0, window_width_, window_height_);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
    const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
    const glm::vec3& camera_position = main_camera_->GetPosition();

    shadow_mapping_pass_->PrepareRendering();
    shadow_mapping_pass_->SetProjectionMatrix(projection_matrix);
    shadow_mapping_pass_->SetViewMatrix(view_matrix);
    shadow_mapping_pass_->SetCameraWorldPosition(main_camera_->GetPosition());
    shadow_mapping_pass_->SetLightWorldPosition(shadow_mapping_depth_pass_->GetLightPosition());
    shadow_mapping_pass_->SetLightSpaceMatrix(shadow_mapping_depth_pass_->GetLightSpaceMatrix());
    shadow_mapping_pass_->SetDiffuseTexture(wood_texture_, 0);
    shadow_mapping_pass_->SetShadowMap(shadow_depth_buffer_->GetTexture(), 1);
    shadow_mapping_pass_->SetBaisAndSampleForShadowLocation(use_bias_sample_shadow_);

    // 绘制地板
    cull_mode_draw_floor_.Use();
    glm::mat4 model;
    shadow_mapping_pass_->SetWorldMatrix(model);
    floor_->Draw();

    // 绘制盒子
    cull_mode_draw_box_.Use();
    model = glm::scale(glm::translate(glm::mat4(), glm::vec3(0.0f, 1.5f, 0.0f)), glm::vec3(0.5f));
    shadow_mapping_pass_->SetWorldMatrix(model);
    shadow_mapping_pass_->SetDiffuseTexture(stone_texture_, 0);
    box_->Draw();

    model = glm::scale(glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, 1.0f)), glm::vec3(0.5f));
    shadow_mapping_pass_->SetWorldMatrix(model);
    box_->Draw();

    model = glm::scale(glm::rotate(glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f, 2.0f)),
        glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))), glm::vec3(0.25f));
    shadow_mapping_pass_->SetWorldMatrix(model);
    box_->Draw();
}

void ShadowMappingApp::RenderSceneDepthToBuffer()
{
    // --> 渲染场景图元的深度到纹理中去
    kgl::IRect view_port_rect(0, 0, 1024, 1024);
    glm::vec4 clear_color(0.0f, 0.0f, 0.0f, 1.0f);
    rs_depth_.Use();
    shadow_depth_buffer_->StartWriting(clear_color, GL_DEPTH_BUFFER_BIT, &view_port_rect);

    shadow_mapping_depth_pass_->Update(0.0f);
    shadow_mapping_depth_pass_->PrepareRendering();
    shadow_mapping_depth_pass_->SetLightSpaceMatrix();

    // 绘制地板
    glm::mat4 model;
    shadow_mapping_depth_pass_->SetWorldMatrix(model);
    floor_->Draw();

    // 绘制盒子
    model = glm::scale(glm::translate(glm::mat4(), glm::vec3(0.0f, 1.5f, 0.0f)), glm::vec3(0.5f));
    shadow_mapping_depth_pass_->SetWorldMatrix(model);
    box_->Draw();

    model = glm::scale(glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, 1.0f)), glm::vec3(0.5f));
    shadow_mapping_depth_pass_->SetWorldMatrix(model);
    box_->Draw();

    model = glm::scale(glm::rotate(glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f, 2.0f)),
        glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))), glm::vec3(0.25f));
    shadow_mapping_depth_pass_->SetWorldMatrix(model);
    box_->Draw();

    view_port_rect.Reset(0, 0, window_width_, window_height_);
    shadow_depth_buffer_->EndWriting(&view_port_rect);
}

void ShadowMappingApp::RenderHelpText(const glm::vec3& view_pos)
{
    const std::wstring& help_toggle = is_help_on_ ? toggle_help_off_text_ : toggle_help_on_text_;
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    glm::vec4 text_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    font_renderer->AddToRendered(help_toggle, 0, 0, text_color, 1.0f);

    if (is_help_on_)
    {
        boost::format fmt("摄像机位置坐标：(%-8.3f,%-8.3f,%-8.3f)");
        fmt % view_pos.x % view_pos.y % view_pos.z;
        font_renderer->AddToRendered(camera_ctrl_text_, 0, 30, text_color, 1.0f);
        font_renderer->AddToRendered(kgl::StringConvertor::UTF8toUTF16LE(fmt.str().c_str()), 0, 60, text_color, 1.0f);
    }

    font_renderer->Draw();
}

void ShadowMappingApp::InitModel()
{
    floor_ = kgl::PrimitiveTool::BuildNormalTexturedXZPlane(5.0f,-0.5f);
    box_ = kgl::PrimitiveTool::BuildNDCTextureNormalCube();

    kgl::TextureParams texture_param;
    texture_param.wrap_s_mode = GL_REPEAT;
    texture_param.wrap_t_mode = GL_REPEAT;
    texture_param.mag_filter_mode = GL_LINEAR;
    texture_param.min_filter_mode = GL_LINEAR_MIPMAP_LINEAR;
    texture_param.internal_format = GL_RGB;
    texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
    texture_param.src_img_format = GL_BGR;//GL_RGB;
    texture_param.used_mipmap = true;
    wood_texture_ = std::make_shared<kgl::SourceTexture>();
    wood_texture_->CreateFromFile("resources/image/wood.png", texture_param);

    stone_texture_ = std::make_shared<kgl::SourceTexture>();
    stone_texture_->CreateFromFile("resources/image/stone_wall.png", texture_param);

    // 初始化图元
    debug_screen_ = kgl::PrimitiveTool::BuildNDCTexturedRectange(0.5f, 1.f, 0.5f, 1.f);
}

void ShadowMappingApp::InitShaders()
{
    const GLchar* vs_file = "resources/shader/025_debug_screen_vs.glsl";
    const GLchar* fs_file = "resources/shader/025_debug_screen_fs.glsl";
    debug_screen_shader_ = new kgl::GPUProgram;
    debug_screen_shader_->CreateFromFile(vs_file, fs_file, nullptr);
}

void ShadowMappingApp::InitLights()
{
}

void ShadowMappingApp::InitMainCamera()
{
    glm::vec3 camera_pos(-0.207f, 4.528f, 9.536f);
    float pitch_angle = -28.0f;
    float yaw_angle = 174.5f;
    float move_speed = 0.1f;
    float max_yaw_degree_per_frame = 1.0f;
    float max_pitch_degree_per_frame = 1.0f;
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, camera_pos);
    main_camera_->SetCameraSpeed(move_speed);
    main_camera_->SetMaxYawDegreePerFrame(max_yaw_degree_per_frame);
    main_camera_->SetMaxPitchDegreePerFrame(max_pitch_degree_per_frame);
    main_camera_->SetPitchAngle(pitch_angle);
    main_camera_->SetYawAngle(yaw_angle);
}

void ShadowMappingApp::InitFont()
{
    const char* font_name = "resources/font/wqy_wmh.ttf";
    std::string font_texture_name("wqyht24");
    int32_t font_size = 24;
    uint32_t font_texture_width = 512;
    uint32_t font_texture_height = 512;
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    font_renderer->Initialize();
    font_renderer->CreateFontTexture(font_name, font_texture_name.c_str(), font_size, font_texture_width, font_texture_height);
    font_renderer->SetCurrentFont(font_texture_name);

    toggle_help_on_text_ = kgl::StringConvertor::UTF8toUTF16LE("按下H键显示帮助");
    toggle_help_off_text_ = kgl::StringConvertor::UTF8toUTF16LE("按下H键关闭帮助");
    camera_ctrl_text_ = kgl::StringConvertor::UTF8toUTF16LE("持续按下W，S，U，J键，使得摄像机向前、后方向移动，增大或减小曝光度值");
}

void ShadowMappingApp::ProcessInput()
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
        main_camera_->Move(kgl::CameraDirection::DOWN);
    }

    if (key_state_[GLFW_KEY_U])
    {
        main_camera_->Move(kgl::CameraDirection::UP);
    }

    if (key_state_[GLFW_KEY_UP])
    {
        main_camera_->ChangePitch(0.5f);
    }

    if (key_state_[GLFW_KEY_DOWN])
    {
        main_camera_->ChangePitch(-0.5f);
    }

    if (key_state_[GLFW_KEY_LEFT])
    {
        main_camera_->ChangeYaw(0.5f);
    }

    if (key_state_[GLFW_KEY_RIGHT])
    {
        main_camera_->ChangeYaw(-0.5f);
    }
}

void ShadowMappingApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    App::OnKeyAction(window, key, scancode, action, mode);

    if (GLFW_KEY_H == key && action == GLFW_RELEASE)
    {
        is_help_on_ = !is_help_on_;
    }

    if (GLFW_KEY_B == key && action == GLFW_RELEASE)
    {
        use_bias_sample_shadow_ = !use_bias_sample_shadow_;
    }
}

void ShadowMappingApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    App::OnMouseAction(window, xpos, ypos);
}

void ShadowMappingApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
    App::OnScrollAction(window, xoffset, yoffset);
}