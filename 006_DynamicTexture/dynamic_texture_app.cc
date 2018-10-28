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
#include "dynamic_texture_app.h"
#include "../klib/kgl_defines.h"

const int VERTEX_DATA_COUNT = 24;

DynamicTextureApp::DynamicTextureApp() :gpu_program_(nullptr), rectangle_primitive_(nullptr)
{
    GLfloat vertices[] =
    {
        // 位置          // 颜色           // 纹理映射坐标
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上角
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下角
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下角
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  //左上角
    };

    vertices_data_ = new GLfloat[VERTEX_DATA_COUNT];
    memcpy(vertices_data_, vertices, sizeof(GLfloat) * VERTEX_DATA_COUNT);
}

DynamicTextureApp::~DynamicTextureApp()
{
    KGL_SAFE_DELETE_ARRAY(vertices_data_);
    KGL_SAFE_DELETE(gpu_program_);
    KGL_SAFE_DELETE(rectangle_primitive_);
    texture_1_.reset();
}

void DynamicTextureApp::InitScene()
{
    gpu_program_ = new kgl::GPUProgram;
    gpu_program_->CreateFromFile("resources/shader/003_texture_vs.glsl", "resources/shader/003_texture_fs.glsl", nullptr);

    kgl::TextureParams texture_param;
    texture_param.wrap_s_mode = GL_REPEAT;
    texture_param.wrap_t_mode = GL_REPEAT;
    texture_param.mag_filter_mode = GL_LINEAR;
    texture_param.min_filter_mode = GL_LINEAR;
    texture_param.internal_format = GL_RGB;
    texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
    texture_param.src_img_format = GL_RGB;
    texture_param.used_mipmap = false;

    texture_1_ = std::make_shared<kgl::SourceTexture>();
    texture_1_->Create(128, 128, texture_param);


    GLfloat vertices[] =
    {
        // 位置          // 颜色           // 纹理映射坐标
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上角
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下角
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下角
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  //左上角
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
    va_position.stride = 8 * sizeof(GLfloat); // 每个顶点的步长
    va_position.pointer = nullptr;

    kgl::VertexAttribute va_color;
    va_color.index = 1;
    va_color.normalized = GL_FALSE;
    va_color.type = GL_FLOAT;
    va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
    va_color.stride = 8 * sizeof(GLfloat); //每个顶点的步长
    va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

    kgl::VertexAttribute va_texture_coord;
    va_texture_coord.index = 2;
    va_texture_coord.normalized = GL_FALSE;
    va_texture_coord.type = GL_FLOAT;
    va_texture_coord.size = 2; // 一个“顶点坐标”的属性由3个分量组成
    va_texture_coord.stride = 8 * sizeof(GLfloat); // 每个顶点的步长
    va_texture_coord.pointer = reinterpret_cast<GLvoid*> (6 * sizeof(GLfloat)); // 纹理坐标数据在字段中的起始指针处

    std::vector<kgl::VertexAttribute> vtx_attri_array;
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_color);
    vtx_attri_array.push_back(va_texture_coord);

    rectangle_primitive_ = new kgl::Primitive;
    rectangle_primitive_->CreateIndexed(GL_TRIANGLES, vertices, sizeof(vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_STATIC_DRAW, vtx_attri_array);

    srand(static_cast<unsigned int>(glfwGetTime()));
}

void DynamicTextureApp::RenderFrame()
{
    gpu_program_->Use();

    float current_time = static_cast<float>(glfwGetTime());

    // 2秒更新一次顶点缓冲区数据
    if (current_time - last_update_vb_time_ > 2.0f)
    {
        unsigned char data[3 * 64 * 64];

        for (int i = 0; i < 3 * 64 * 64; ++i)
        {
            data[i] = static_cast<unsigned char>(rand() % 255);
        }

        texture_1_->Update(10, 10, 64, 64, data);
        last_update_vb_time_ = current_time;
    }

    gpu_program_->Use();
    gpu_program_->ApplyTexture(texture_1_, "ourTexture1", 0);
    rectangle_primitive_->DrawIndexed();
}