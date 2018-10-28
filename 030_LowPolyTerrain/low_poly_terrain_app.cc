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
#include "low_poly_terrain_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_string_convertor.h"
#include "../klib/kgl_standard_index_generator.h"

LowPolyTerrainApp::LowPolyTerrainApp()
{
}

LowPolyTerrainApp::~LowPolyTerrainApp()
{
    terrain_.reset();
    perlin_noise_.reset();
    color_generator_.reset();
    indices_generator_.reset();
    KGL_SAFE_DELETE(flat_terrain_generator_);
}

void LowPolyTerrainApp::PreRenderFrame()
{
    glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LowPolyTerrainApp::InitModel()
{
    std::vector<glm::vec4> terrain_color_array;
    terrain_color_array.resize(7);
    terrain_color_array[0] = glm::vec4(201.0f / 255.0f, 178.0f / 255.0f, 99.0f / 255.0f, 1.0f);
    terrain_color_array[1] = glm::vec4(164.0f / 255.0f, 155.0f / 255.0f, 98.0f / 255.0f, 1.0f);
    terrain_color_array[2] = glm::vec4(164.0f / 255.0f, 155.0f / 255.0f, 98.0f / 255.0f, 1.0f);
    terrain_color_array[3] = glm::vec4(229.0f / 255.0f, 219.0f / 255.0f, 164.0f / 255.0f, 1.0f);
    terrain_color_array[4] = glm::vec4(135.0f / 255.0f, 184.0f / 255.0f, 82.0f / 255.0f, 1.0f);
    terrain_color_array[5] = glm::vec4(120.0f / 255.0f, 120.0f / 255.0f, 120.0f / 255.0f, 1.0f);
    terrain_color_array[6] = glm::vec4(200.0f / 255.0f, 200.0f / 255.0f, 210.0f / 255.0f, 1.0f);
    float color_spread = 0.45f;
    color_generator_ = std::make_shared<kgl::low_poly_terrain::ColorGenerator>(
        terrain_color_array, color_spread);

    // ====================================================

    const float AMPLITUDE = 30;
    const float ROUGHNESS = 0.4f;
    const int OCTAVES = 5;
    perlin_noise_ = std::make_shared<kgl::low_poly_terrain::PerlinNoise>(OCTAVES, AMPLITUDE,ROUGHNESS);
    
    // ====================================================

    indices_generator_ = std::make_shared<kgl::low_poly_terrain::StandardIndexGenerator>();
    
    // ====================================================

    flat_terrain_generator_ = new kgl::low_poly_terrain::FlatTerrainGenerator(
        perlin_noise_, color_generator_, main_camera_, indices_generator_);

    const int WORLD_SIZE = 100;
    terrain_ = flat_terrain_generator_->GenerateTerrain(WORLD_SIZE);

    // ====================================================

    rs_depth_.SetEnable(GL_TRUE);
}


void LowPolyTerrainApp::InitShaders()
{
}

void LowPolyTerrainApp::InitLights()
{
    light_.Bias = glm::vec2(0.3f, 0.8f);
    // light_.Color = glm::vec4(1.0f, 0.95f, 0.95f,1.0f);
    light_.Color = glm::vec4(1.0f, 0.15f, 0.15f, 1.0f);
    light_.Direction = glm::normalize(glm::vec3(0.3f, -1.0f, 0.5f));
}

void LowPolyTerrainApp::InitMainCamera()
{
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(30.0f, 40.0f, 35.0f), 0.0f);
    main_camera_->SetMaxYawDegreePerFrame(1.0f);
    main_camera_->SetMaxPitchDegreePerFrame(1.0f);
}

void LowPolyTerrainApp::InitFont()
{
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    font_renderer->Initialize();
    font_renderer->CreateFontTexture("resources/font/fzkt_sim.ttf", "fzktsim24", 24, 512, 512);
    font_renderer->SetCurrentFont("fzktsim24");
    toggle_help_on_text_ = kgl::StringConvertor::ANSItoUTF16LE("按下H键显示帮助");
    toggle_help_off_text_ = kgl::StringConvertor::ANSItoUTF16LE("按下H键关闭帮助");
    camera_ctrl_text_ = kgl::StringConvertor::ANSItoUTF16LE("持续按下W、S、A、D、U、J键，使得摄像机向前、后、左、右、上、下方向移动");
}

void LowPolyTerrainApp::RenderFrame()
{
    main_camera_->Update();
    rs_depth_.Use();
    draw_mode_.SetCurrentDrawMode(draw_mode_line_ ? kgl::DM_LINE : kgl::DM_FILL);
    terrain_->Render(light_);
    this->DrawHelpText(main_camera_->GetPosition());
}

void LowPolyTerrainApp::DrawHelpText(const glm::vec3& view_pos)
{
    const std::wstring& help_toggle = is_help_on_ ? toggle_help_off_text_ : toggle_help_on_text_;
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();

    glm::vec4 text_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    font_renderer->AddToRendered(help_toggle, 0, 0, text_color, 1.0f);

    if (is_help_on_)
    {
        boost::format fmt("摄像机位置坐标： x = %f , y = %f , z = %f");
        fmt % view_pos.x % view_pos.y % view_pos.z;
        font_renderer->AddToRendered(camera_ctrl_text_, 0, 25, text_color, 1.0f);
        font_renderer->AddToRendered(kgl::StringConvertor::ANSItoUTF16LE(fmt.str().c_str()), 0, 100, text_color, 1.0f);
    }

    font_renderer->Draw();
}

void LowPolyTerrainApp::ProcessInput()
{
    main_camera_->SetCameraSpeed(0.1f);

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
        main_camera_->ChangeYaw(1.0f);
    }

    if (key_state_[GLFW_KEY_RIGHT])
    {
        main_camera_->ChangeYaw(-1.0f);
    }
}

void LowPolyTerrainApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    App::OnKeyAction(window, key, scancode, action, mode);

    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    {
        
    }

    if (GLFW_KEY_H == key && action == GLFW_RELEASE)
    {
        is_help_on_ = !is_help_on_;
    }

    if (GLFW_KEY_L == key && action == GLFW_RELEASE)
    {
        draw_mode_line_ = !draw_mode_line_;
    }
}

void LowPolyTerrainApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    App::OnMouseAction(window, xpos, ypos);
}

void LowPolyTerrainApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
    main_camera_->SetCameraSpeed(fabsf(static_cast<float>(yoffset)));

    if (yoffset > 0.0)
    {
        main_camera_->Move(kgl::CameraDirection::FORWARD);
    }
    else
    {
        main_camera_->Move(kgl::CameraDirection::BACK);
    }
}