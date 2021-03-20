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
    glm::mat4 model = glm::mat4();
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
  //  this->RenderHelpText(camera_position);
}

void HDRApp::RenderHelpText(const glm::vec3& view_pos)
{
    const std::wstring& help_toggle = is_help_on_ ? toggle_help_off_text_ : toggle_help_on_text_;
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    glm::vec4 text_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    font_renderer->AddToRendered(help_toggle, 0, 0, text_color, 1.0f);

    if (is_help_on_)
    {
        boost::format fmt("摄像机位置坐标：(%-8.3f,%-8.3f,%-8.3f)，曝光度值： %-8.3f, %s");
        fmt % view_pos.x % view_pos.y % view_pos.z
            % exposure_ 
            % (use_hdr_ ? "已启用HDR效果" : "已关闭HDR效果");
        font_renderer->AddToRendered(camera_ctrl_text_, 0, 30, text_color, 1.0f);
        font_renderer->AddToRendered(kgl::StringConvertor::UTF8toUTF16LE(fmt.str().c_str()), 0, 60, text_color, 1.0f);
        
        font_renderer->AddToRendered(use_hdr_ ?
            kgl::StringConvertor::UTF8toUTF16LE("按下R键关闭HDR效果") :
            kgl::StringConvertor::UTF8toUTF16LE("按下R键启用HDR效果"), 0, 90, text_color, 1.0f);
    }

    font_renderer->Draw();
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
    texture_param.min_filter_mode = GL_LINEAR;
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

void HDRApp::InitFont()
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

    if (GLFW_KEY_H == key && action == GLFW_RELEASE)
    {
        is_help_on_ = !is_help_on_;
    }

    if (GLFW_KEY_R == key && action == GLFW_RELEASE)
    {
        use_hdr_ = !use_hdr_;
    }
}

void HDRApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    App::OnMouseAction(window, xpos, ypos);
}

void HDRApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
    App::OnScrollAction(window, xoffset, yoffset);
}