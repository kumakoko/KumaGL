﻿/**************************************************************************************************************************
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
// 各种图像滤镜操作
#include "image_effect_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_string_convertor.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"FreeImage.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glfw3dll.lib")
#pragma comment(lib,"assimp-vc142-mtd.lib")
#pragma comment(lib,"vld.lib")
#pragma comment(lib,"libiconv.lib")
#pragma comment(lib,"freetype.lib")
#pragma comment(lib,"imgui_d.lib")
#pragma comment(lib,"klib_d.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")
#else
#pragma comment(lib,"FreeImage.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glfw3dll.lib")
#pragma comment(lib,"assimp-vc142-mt.lib")
#pragma comment(lib,"vld.lib")
#pragma comment(lib,"libiconv.lib")
#pragma comment(lib,"freetype.lib")
#pragma comment(lib,"imgui.lib")
#pragma comment(lib,"klib.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")
#endif

ImageEffectApp::ImageEffectApp()
{
}

ImageEffectApp::~ImageEffectApp()
{
    KGL_SAFE_DELETE(color_to_gray_shader_);
    KGL_SAFE_DELETE(emboss_shader_);
    main_texture_.reset();
}

void ImageEffectApp::InitShaders()
{
    color_to_gray_shader_ = new kgl::GPUProgram;
    color_to_gray_shader_->CreateFromFile(
        "resources/shader/img_fx_vs.glsl",
        "resources/shader/img_fx_color_to_gray_fs.glsl", nullptr);

    emboss_shader_ = new kgl::GPUProgram;
    emboss_shader_->CreateFromFile(
        "resources/shader/img_fx_vs.glsl",
        "resources/shader/img_fx_emboss_fs.glsl", nullptr);

    saturation_shader_ = new kgl::GPUProgram;
    saturation_shader_->CreateFromFile(
        "resources/shader/img_fx_vs.glsl",
        "resources/shader/img_fx_saturation_fs.glsl", nullptr);
}

void ImageEffectApp::InitModel()
{
    kgl::TextureParams texture_param;
    texture_param.wrap_s_mode = GL_REPEAT;
    texture_param.wrap_t_mode = GL_REPEAT;
    texture_param.mag_filter_mode = GL_LINEAR;
    texture_param.min_filter_mode = GL_NEAREST;
    texture_param.internal_format = GL_RGB;
    texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
    texture_param.src_img_format = GL_RGB;
    texture_param.used_mipmap = false;

    main_texture_ = std::make_shared<kgl::SourceTexture>();
    main_texture_->CreateFromFile("resources/image/fruit.jpg", texture_param);
    
    rectangle_primitive_ = kgl::PrimitiveTool::BuildNDCTexturedRectange();
}

void ImageEffectApp::InitFont()
{
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    font_renderer->Initialize();
    font_renderer->CreateFontTexture("resources/font/fzkt_sim.ttf", "fzktsim36", 36, 512, 512);
    font_renderer->SetCurrentFont("fzktsim36");
}

void ImageEffectApp::InitScene()
{
    screen_resolution_ = glm::vec2(800.0f, 600.0f);
    App::InitScene();

    rs_blend_.SetEnable(true);
    rs_blend_.SetBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ImageEffectApp::RenderScene()
{
    switch (current_effect_index_)
    {
    case 0:this->RenderColorToGray(); break;
    case 1:this->RenderEmboss(); break;
    case 2:this->RenderSaturation(); break;
    }
}

void ImageEffectApp::RenderColorToGray()
{
    color_to_gray_shader_->Use();
    color_to_gray_shader_->ApplyTexture(main_texture_, "main_texture", 0);
    rectangle_primitive_->Draw();

    if (effect_changed_)
    {
        effect_changed_ = false;
        info_message_ = "彩色图转灰度图";
    }
}

void ImageEffectApp::RenderEmboss()
{
    emboss_shader_->Use();
    emboss_shader_->ApplyTexture(main_texture_, "main_texture", 0);
    rectangle_primitive_->Draw();

	if (effect_changed_)
	{
		effect_changed_ = false;
		info_message_ = "浮雕效果";
	}
}

void ImageEffectApp::RenderSaturation()
{
    saturation_shader_->Use();
    saturation_shader_->ApplyTexture(main_texture_, "main_texture", 0);
    saturation_shader_->ApplyFloat(saturation_factor_, "saturation_factor");
    rectangle_primitive_->Draw();

	if (effect_changed_)
	{
        effect_changed_ = false;
		boost::format fmt("Image Effect : 色彩饱和度调整，按上下键调整饱和度，当前的饱和度控制值为 %1.3f");
		fmt% saturation_factor_;
        info_message_ = fmt.str();
	}
}

void ImageEffectApp::RenderGUI()
{
	const glm::vec3& camera_pos = main_camera_->GetPosition();
	ImGui::Begin("040 Image Effect 图像处理效果");
	ImGui::Text("按左右键切换各种图像处理效果");
	ImGui::Text(info_message_.c_str());
	ImGui::End();
}

void ImageEffectApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_RIGHT  && action == GLFW_RELEASE)
    {
        current_effect_index_++;

        if (current_effect_index_ >= effect_count_)
            current_effect_index_ = 0;

        effect_changed_ = true;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
        current_effect_index_--;

        if (current_effect_index_ < 0)
            current_effect_index_ = effect_count_ - 1;

        effect_changed_ = true;
    }

    App::OnKeyAction(window, key, scancode, action, mode);
}

void ImageEffectApp::ProcessInput()
{
    if (key_state_[GLFW_KEY_UP])
    {
        if (current_effect_index_ == 2)
        {
            saturation_factor_ += 0.01f;

            if (saturation_factor_ > 3.0f)
                saturation_factor_ = 0.0f;

            effect_changed_ = true;
        }
    }

    if (key_state_[GLFW_KEY_DOWN])
    {
        if (current_effect_index_ == 2)
        {
            saturation_factor_ -= 0.01f;

            if (saturation_factor_ < 0.0f)
                saturation_factor_ = 3.0f;

            effect_changed_ = true;
        }
    }
}