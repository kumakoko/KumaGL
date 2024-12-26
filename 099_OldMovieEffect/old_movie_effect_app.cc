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
// 展示老电影风格的shader
#include "old_movie_effect_app.h"
#include "../klib/kgl_defines.h"

OldMovieEffectApp::OldMovieEffectApp()
{
    film_color_ = glm::vec4(1.0f, 0.7559052f, 0.58474624f, 1.0f);
}

OldMovieEffectApp::~OldMovieEffectApp()
{
    KGL_SAFE_DELETE(shader_);
    texture_scene_.reset();
    texture_dust_.reset();
    texture_line_.reset();
    texture_tv_.reset();
    texture_noise_.reset();
}

void OldMovieEffectApp::InitScene()
{
    screen_resolution_ = glm::vec2(800.0f, 600.0f);
// ================================================================
    shader_ = new kgl::GPUProgram;
    shader_->CreateFromFile("resources/shader/old_movie_effect_vs.glsl", "resources/shader/old_movie_effect_fs.glsl", nullptr);
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

    texture_scene_ = std::make_shared<kgl::SourceTexture>();
    texture_scene_->CreateFromFile("resources/image/old_movie_effect/scene_1.png", texture_param);

    texture_dust_ = std::make_shared<kgl::SourceTexture>();
    texture_dust_->CreateFromFile("resources/image/old_movie_effect/dust.png", texture_param);

    texture_line_ = std::make_shared<kgl::SourceTexture>();
    texture_line_->CreateFromFile("resources/image/old_movie_effect/line.png", texture_param);

    texture_tv_ = std::make_shared<kgl::SourceTexture>();
    texture_tv_->CreateFromFile("resources/image/old_movie_effect/tv.png", texture_param);

    texture_noise_ = std::make_shared<kgl::SourceTexture>();
    texture_noise_->CreateFromFile("resources/image/old_movie_effect/tv.png", texture_param);

    GLfloat vertices[] =
    {
        // 位置          // 颜色            //纹理坐标1 
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,     // 右上角
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // 右下角
        -1.0f, -1.0f, 0.0f,0.0f, 0.0f, // 左下角
        - 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  //左上角
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
    va_position.stride = 5 * sizeof(GLfloat); // 每个顶点的步长
    va_position.pointer = nullptr;

    kgl::VertexAttribute va_texture_coord_1;
    va_texture_coord_1.index = 1;
    va_texture_coord_1.normalized = GL_FALSE;
    va_texture_coord_1.type = GL_FLOAT;
    va_texture_coord_1.size = 2; // 一个“顶点纹理映射坐标”的属性由2个分量组成
    va_texture_coord_1.stride = 5 * sizeof(GLfloat); // 每个顶点的步长
    va_texture_coord_1.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat)); // 纹理坐标数据在字段中的起始指针处

    std::vector<kgl::VertexAttribute> vtx_attri_array;
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_texture_coord_1);

    rectangle_primitive_ = new kgl::Primitive;
    rectangle_primitive_->CreateIndexed(GL_TRIANGLES, vertices, sizeof(vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_STATIC_DRAW, vtx_attri_array);
}

void OldMovieEffectApp::RenderScene()
{
    shader_->Use();
    shader_->ApplyFloat(glfwGetTime(), "current_time");
    shader_->ApplyFloat(scratches_level_, "scratches_level");
    shader_->ApplyFloat(scratches_amount_, "scratches_amount");
    shader_->ApplyFloat(max_frame_jitter_, "max_frame_jitter");
    shader_->ApplyFloat(frame_jitter_, "frame_jitter");
    shader_->ApplyFloat(over_exposure_amount_, "over_exposure_amount");
    shader_->ApplyFloat(grain_thickness_, "grain_thickness");
    shader_->ApplyFloat(dust_amount_, "dust_amount");
    shader_->ApplyVector2(glm::value_ptr(screen_resolution_), "window_size");
    shader_->ApplyVector4(glm::value_ptr(film_color_), "film_color");
    shader_->ApplyTexture(texture_scene_, "texture_scene", 0);
    shader_->ApplyTexture(texture_dust_, "texture_dust", 1);
    shader_->ApplyTexture(texture_line_, "texture_line", 2);
    shader_->ApplyTexture(texture_tv_, "texture_tv", 3);
    shader_->ApplyTexture(texture_noise_, "texture_noise", 4);

    rectangle_primitive_->DrawIndexed();
}


void OldMovieEffectApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_RIGHT  && action == GLFW_RELEASE)
    {
      
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
       
    }

    App::OnKeyAction(window, key, scancode, action, mode);
}

void OldMovieEffectApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
   
}