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

#include "pixel_magic_app.h"
#include "../klib/kgl_defines.h"

PixelMagicApp::PixelMagicApp():
colorful_ring_shader_(nullptr),
heart_beat_shader_(nullptr),
mobius_shader_(nullptr),
sun_shader_(nullptr),
japan_flag_shader_(nullptr),
rectangle_primitive_(nullptr),
current_effect_index_(0),
effect_count_(6)
{
}

PixelMagicApp::~PixelMagicApp()
{
    KGL_SAFE_DELETE(colorful_ring_shader_);
    KGL_SAFE_DELETE(heart_beat_shader_);
    KGL_SAFE_DELETE(mobius_shader_);
    KGL_SAFE_DELETE(sun_shader_);
    KGL_SAFE_DELETE(japan_flag_shader_);
    KGL_SAFE_DELETE(japan_flag_shader_);
    KGL_SAFE_DELETE(china_flag_shader_);
    texture_1_.reset();
    texture_2_.reset();
}

void PixelMagicApp::InitScene()
{
    screen_resolution_ = glm::vec2(960.0f, 640.0f);
// ================================================================
    heart_beat_shader_ = new kgl::GPUProgram;
    heart_beat_shader_->CreateFromFile("resources/shader/pixel_magic_vs.glsl", "resources/shader/heart_beat_fs.glsl", nullptr);
// ================================================================
    colorful_ring_shader_ = new kgl::GPUProgram;
    colorful_ring_shader_->CreateFromFile("resources/shader/pixel_magic_vs.glsl", "resources/shader/colorful_ring_fs.glsl", nullptr);
// ================================================================
    mobius_shader_ = new kgl::GPUProgram;
    mobius_shader_->CreateFromFile("resources/shader/pixel_magic_vs.glsl", "resources/shader/mobius_fs.glsl", nullptr);
// ================================================================
    sun_shader_ = new kgl::GPUProgram;
    sun_shader_->CreateFromFile("resources/shader/pixel_magic_vs.glsl", "resources/shader/sun_fs.glsl", nullptr);
// ================================================================
    japan_flag_shader_ = new kgl::GPUProgram;
    japan_flag_shader_->CreateFromFile("resources/shader/pixel_magic_vs.glsl", "resources/shader/pm_japan_flag_fs.glsl", nullptr);
// ================================================================
    china_flag_shader_ = new kgl::GPUProgram;
    china_flag_shader_->CreateFromFile("resources/shader/pixel_magic_vs.glsl", "resources/shader/pm_china_flag_fs.glsl", nullptr);
// ================================================================
    
    kgl::TextureParams texture_param;
    texture_param.wrap_s_mode = GL_REPEAT;
    texture_param.wrap_t_mode = GL_REPEAT;
    texture_param.mag_filter_mode = GL_LINEAR;
    texture_param.min_filter_mode = GL_NEAREST;
    texture_param.internal_format = GL_RGB;
    texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
    texture_param.src_img_format = GL_RGB;
    texture_param.used_mipmap = false;

    texture_rock_ = std::make_shared<kgl::SourceTexture>();
    texture_rock_->CreateFromFile("resources/image/stone_small.jpg", texture_param);

    GLfloat vertices[] =
    {
        // 位置          // 颜色            //纹理坐标1 //纹理坐标2 //纹理坐标3 //纹理坐标4
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,// 右上角
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下角
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f,0.0f, 0.0f, // 左下角
        - 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f  //左上角
    };

    GLuint indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    kgl::VertexAttribute va_position;
    va_position.index = 0;
    va_position.normalized = GL_FALSE;
    va_position.type = GL_FLOAT;
    va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
    va_position.stride = 14 * sizeof(GLfloat); // 每个顶点的步长
    va_position.pointer = nullptr;

    kgl::VertexAttribute va_color;
    va_color.index = 1;
    va_color.normalized = GL_FALSE;
    va_color.type = GL_FLOAT;
    va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
    va_color.stride = 14 * sizeof(GLfloat); //每个顶点的步长
    va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

    kgl::VertexAttribute va_texture_coord_1;
    va_texture_coord_1.index = 2;
    va_texture_coord_1.normalized = GL_FALSE;
    va_texture_coord_1.type = GL_FLOAT;
    va_texture_coord_1.size = 2; // 一个“顶点坐标”的属性由3个分量组成
    va_texture_coord_1.stride = 14 * sizeof(GLfloat); // 每个顶点的步长
    va_texture_coord_1.pointer = reinterpret_cast<GLvoid*> (6 * sizeof(GLfloat)); // 纹理坐标数据在字段中的起始指针处

    kgl::VertexAttribute va_texture_coord_2;
    va_texture_coord_2.index = 3;
    va_texture_coord_2.normalized = GL_FALSE;
    va_texture_coord_2.type = GL_FLOAT;
    va_texture_coord_2.size = 2; // 一个“顶点坐标”的属性由3个分量组成
    va_texture_coord_2.stride = 14 * sizeof(GLfloat); // 每个顶点的步长
    va_texture_coord_2.pointer = reinterpret_cast<GLvoid*> (8 * sizeof(GLfloat)); // 纹理坐标数据在字段中的起始指针处

    kgl::VertexAttribute va_texture_coord_3;
    va_texture_coord_3.index = 4;
    va_texture_coord_3.normalized = GL_FALSE;
    va_texture_coord_3.type = GL_FLOAT;
    va_texture_coord_3.size = 2; // 一个“顶点坐标”的属性由3个分量组成
    va_texture_coord_3.stride = 14 * sizeof(GLfloat); // 每个顶点的步长
    va_texture_coord_3.pointer = reinterpret_cast<GLvoid*> (10 * sizeof(GLfloat)); // 纹理坐标数据在字段中的起始指针处

    kgl::VertexAttribute va_texture_coord_4;
    va_texture_coord_4.index = 5;
    va_texture_coord_4.normalized = GL_FALSE;
    va_texture_coord_4.type = GL_FLOAT;
    va_texture_coord_4.size = 2; // 一个“顶点坐标”的属性由3个分量组成
    va_texture_coord_4.stride = 14 * sizeof(GLfloat); // 每个顶点的步长
    va_texture_coord_4.pointer = reinterpret_cast<GLvoid*> (12 * sizeof(GLfloat)); // 纹理坐标数据在字段中的起始指针处

    std::vector<kgl::VertexAttribute> vtx_attri_array;
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_color);
    vtx_attri_array.push_back(va_texture_coord_1);
    vtx_attri_array.push_back(va_texture_coord_2);
    vtx_attri_array.push_back(va_texture_coord_3);
    vtx_attri_array.push_back(va_texture_coord_4);

    rectangle_primitive_ = new kgl::Primitive;
    rectangle_primitive_->CreateIndexed(GL_TRIANGLES, vertices, sizeof(vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_STATIC_DRAW, vtx_attri_array);
}

void PixelMagicApp::RenderScene()
{
    switch (current_effect_index_)
    {
    case 0:this->RenderBeatHeart(); break;
    case 1:this->RenderColorfulRing(); break;
    case 2:this->RenderMobius(); break;
    case 3:this->RenderSun(); break;
    case 4:this->RenderJapanFlag(); break;
    case 5:this->RenderChinaFlag(); break;
    }

    rectangle_primitive_->DrawIndexed();
}

void PixelMagicApp::RenderBeatHeart()
{
    heart_beat_shader_->Use();
    heart_beat_shader_->ApplyFloat(static_cast<float>(glfwGetTime()), "global_time");
    heart_beat_shader_->ApplyVector2(glm::value_ptr(screen_resolution_), "screen_resolution");
}

void PixelMagicApp::RenderColorfulRing()
{
    colorful_ring_shader_->Use();
    colorful_ring_shader_->ApplyFloat(static_cast<float>(glfwGetTime()), "global_time");
    colorful_ring_shader_->ApplyVector2(glm::value_ptr(screen_resolution_), "screen_resolution");
}

void PixelMagicApp::RenderMobius()
{
    mobius_shader_->Use();
    mobius_shader_->ApplyFloat(static_cast<float>(glfwGetTime()), "global_time");
    mobius_shader_->ApplyVector2(glm::value_ptr(screen_resolution_), "screen_resolution");
    mobius_shader_->ApplyVector2(glm::value_ptr(mouse_input_pos_), "mouse_input_pos");
}

void PixelMagicApp::RenderSun()
{
    sun_shader_->Use();
    sun_shader_->ApplyFloat(static_cast<float>(glfwGetTime()), "global_time");
    sun_shader_->ApplyVector2(glm::value_ptr(screen_resolution_), "screen_resolution");
    sun_shader_->ApplyVector2(glm::value_ptr(mouse_input_pos_), "mouse_input_pos");
    sun_shader_->ApplyTexture(texture_rock_, "texture_channel_1", 0);
}

void PixelMagicApp::RenderJapanFlag()
{
    japan_flag_shader_->Use();
    japan_flag_shader_->ApplyVector2(glm::value_ptr(screen_resolution_), "screen_resolution");
}

void PixelMagicApp::RenderChinaFlag()
{
    china_flag_shader_->Use();
    china_flag_shader_->ApplyVector2(glm::value_ptr(screen_resolution_), "screen_resolution");
    china_flag_shader_->ApplyFloat(china_flag_scale_, "flag_scale");
}

void PixelMagicApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_RIGHT  && action == GLFW_RELEASE)
    {
        current_effect_index_++;

        if (current_effect_index_  >= effect_count_)
            current_effect_index_ = 0;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
        current_effect_index_--;

        if (current_effect_index_ < 0)
            current_effect_index_ = effect_count_ -  1;
    }

    App::OnKeyAction(window, key, scancode, action, mode);
}

void PixelMagicApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    mouse_input_pos_.x = static_cast<float>(xpos);
    mouse_input_pos_.y = static_cast<float>(ypos);
}

void PixelMagicApp::ProcessInput()
{
    if (key_state_[GLFW_KEY_W])
    {
        china_flag_scale_ += 0.1f;
    }

    if (key_state_[GLFW_KEY_S])
    {
        china_flag_scale_ -= 0.1f;
        if (china_flag_scale_ < 0.0f)
            china_flag_scale_ = 0.0f;
    }
}