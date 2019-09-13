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
// 各种图像滤镜操作
#include "image_effect_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_string_convertor.h"

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
    texture_param.min_filter_mode = GL_LINEAR;
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

    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    glm::vec4 text_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    font_renderer->AddToRendered(kgl::StringConvertor::UTF8toUTF16LE("Image Effect : 彩色图转灰度图"), 0, 0, text_color, 1.0f);
    font_renderer->Draw();
}

void ImageEffectApp::RenderEmboss()
{
    emboss_shader_->Use();
    emboss_shader_->ApplyTexture(main_texture_, "main_texture", 0);
    rectangle_primitive_->Draw();

    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    glm::vec4 text_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    font_renderer->AddToRendered(kgl::StringConvertor::UTF8toUTF16LE("Image Effect : 浮雕效果"), 0, 0, text_color, 1.0f);
    font_renderer->Draw();
}

void ImageEffectApp::RenderSaturation()
{
    saturation_shader_->Use();
    saturation_shader_->ApplyTexture(main_texture_, "main_texture", 0);
    saturation_shader_->ApplyFloat(saturation_factor_, "saturation_factor");
    rectangle_primitive_->Draw();

    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    glm::vec4 text_color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    boost::format fmt("Image Effect : 色彩饱和度调整，当前的饱和度控制值为 %f");
    fmt % saturation_factor_;

    font_renderer->AddToRendered(kgl::StringConvertor::UTF8toUTF16LE(fmt.str().c_str()), 0, 0, text_color, 1.0f);
    font_renderer->Draw();
}

void ImageEffectApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_RIGHT  && action == GLFW_RELEASE)
    {
        current_effect_index_++;

        if (current_effect_index_ >= effect_count_)
            current_effect_index_ = 0;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
        current_effect_index_--;

        if (current_effect_index_ < 0)
            current_effect_index_ = effect_count_ - 1;
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
        }
    }

    if (key_state_[GLFW_KEY_DOWN])
    {
        if (current_effect_index_ == 2)
        {
            saturation_factor_ -= 0.01f;

            if (saturation_factor_ < 0.0f)
                saturation_factor_ = 3.0f;
        }
    }
}

void ImageEffectApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
   
}