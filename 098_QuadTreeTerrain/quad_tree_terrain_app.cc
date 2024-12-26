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
#include "quad_tree_terrain_app.h"
#include "../klib/kgl_defines.h"

QuadTreeTerrainApp::QuadTreeTerrainApp() :gpu_program_(nullptr), rectangle_primitive_(nullptr),
triangle_primitive_1_(nullptr), triangle_primitive_2_(nullptr), 
triangle_primitive_3_(nullptr), triangle_primitive_4_(nullptr)
{
}

QuadTreeTerrainApp::~QuadTreeTerrainApp()
{
    KGL_SAFE_DELETE(gpu_program_);
    KGL_SAFE_DELETE(rectangle_primitive_);
    KGL_SAFE_DELETE(triangle_primitive_3_);
    KGL_SAFE_DELETE(triangle_primitive_2_);
    KGL_SAFE_DELETE(triangle_primitive_3_);
    KGL_SAFE_DELETE(triangle_primitive_4_);
}

void QuadTreeTerrainApp::InitModel()
{
    gpu_program_ = new kgl::GPUProgram;
    const GLchar* vs_file_path = nullptr;
    const GLchar* fs_file_path = nullptr;
#if defined(WIN32) || defined(_WIN32)
    vs_file_path = "resources/shader/002_triangle_vs.glsl";
    fs_file_path = "resources/shader/002_triangle_fs.glsl";
#endif
    gpu_program_->CreateFromFile(vs_file_path, fs_file_path, nullptr);


    GLfloat vertices[] = 
    {
        // 位置             // 颜色
        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,// 右上角
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f   // 左上角 
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
    va_position.stride = 6 * sizeof(GLfloat); // 每个顶点的步长为 
    va_position.pointer = nullptr;
    
    kgl::VertexAttribute va_color;
    va_color.index = 1;
    va_color.normalized = GL_FALSE;
    va_color.type = GL_FLOAT;
    va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
    va_color.stride = 6 * sizeof(GLfloat); // 每个位置的步长为 
    va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

    std::vector<kgl::VertexAttribute> vtx_attri_array;
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_color);

    rectangle_primitive_ = new kgl::Primitive;
    rectangle_primitive_->CreateIndexed(GL_TRIANGLES, vertices, sizeof(vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_STATIC_DRAW, vtx_attri_array);


    GLfloat triangle_vertices[] =
    {
        // 位置             // 颜色
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 右下角
        -0.6f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
        -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
    };

    GLuint triangle_indices[] =
    {
        0, 1, 2,
    };

    va_position.index = 0;
    va_position.normalized = GL_FALSE;
    va_position.type = GL_FLOAT;
    va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
    va_position.stride = 6 * sizeof(GLfloat); // 每个顶点的步长为 
    va_position.pointer = nullptr;

    va_color.index = 1;
    va_color.normalized = GL_FALSE;
    va_color.type = GL_FLOAT;
    va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
    va_color.stride = 6 * sizeof(GLfloat); // 每个位置的步长为 
    va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));


    vtx_attri_array.clear();
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_color);

    triangle_primitive_1_ = new kgl::Primitive;
    triangle_primitive_1_->CreateIndexed(GL_TRIANGLES, triangle_vertices, sizeof(triangle_vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, triangle_indices, sizeof(triangle_indices), GL_STATIC_DRAW, vtx_attri_array);

    //=====================================================================================

    GLfloat triangle_2_vertices[] =
    {
        // 位置             // 颜色
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 右下角
        0.6f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
        1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
    };

    va_position.index = 0;
    va_position.normalized = GL_FALSE;
    va_position.type = GL_FLOAT;
    va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
    va_position.stride = 6 * sizeof(GLfloat); // 每个顶点的步长为 
    va_position.pointer = nullptr;

    va_color.index = 1;
    va_color.normalized = GL_FALSE;
    va_color.type = GL_FLOAT;
    va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
    va_color.stride = 6 * sizeof(GLfloat); // 每个位置的步长为 
    va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

    vtx_attri_array.clear();
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_color);

    triangle_primitive_2_ = new kgl::Primitive;
    triangle_primitive_2_->CreateIndexed(GL_TRIANGLES, triangle_2_vertices, sizeof(triangle_2_vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, triangle_indices, sizeof(triangle_indices), GL_STATIC_DRAW, vtx_attri_array);

    //= == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

    GLfloat triangle_3_vertices[] =
    {
        // 位置             // 颜色
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 右下角
        0.6f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
        1.0f, 0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
    };

    va_position.index = 0;
    va_position.normalized = GL_FALSE;
    va_position.type = GL_FLOAT;
    va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
    va_position.stride = 6 * sizeof(GLfloat); // 每个顶点的步长为 
    va_position.pointer = nullptr;

    va_color.index = 1;
    va_color.normalized = GL_FALSE;
    va_color.type = GL_FLOAT;
    va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
    va_color.stride = 6 * sizeof(GLfloat); // 每个位置的步长为 
    va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

    vtx_attri_array.clear();
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_color);

    triangle_primitive_3_ = new kgl::Primitive;
    triangle_primitive_3_->CreateIndexed(GL_TRIANGLES, triangle_3_vertices, sizeof(triangle_3_vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, triangle_indices, sizeof(triangle_indices), GL_STATIC_DRAW, vtx_attri_array);

    //= == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

    GLfloat triangle_4_vertices[] =
    {
        // 位置             // 颜色
        -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 右下角
        -0.6f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
        -1.0f, 0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
    };

    va_position.index = 0;
    va_position.normalized = GL_FALSE;
    va_position.type = GL_FLOAT;
    va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
    va_position.stride = 6 * sizeof(GLfloat); // 每个顶点的步长为 
    va_position.pointer = nullptr;

    va_color.index = 1;
    va_color.normalized = GL_FALSE;
    va_color.type = GL_FLOAT;
    va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
    va_color.stride = 6 * sizeof(GLfloat); // 每个位置的步长为 
    va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

    vtx_attri_array.clear();
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_color);

    triangle_primitive_4_ = new kgl::Primitive;
    triangle_primitive_4_->CreateIndexed(GL_TRIANGLES, triangle_4_vertices, sizeof(triangle_4_vertices), GL_STATIC_DRAW, 
        kgl::Primitive::UINT32, triangle_indices, sizeof(triangle_indices), GL_STATIC_DRAW, vtx_attri_array);
}

void QuadTreeTerrainApp::RenderScene()
{
    time = glfwGetTime();
    gpu_program_->Use();
    rectangle_primitive_->DrawIndexed();
    triangle_primitive_1_->DrawIndexed();
    triangle_primitive_2_->DrawIndexed();
    triangle_primitive_3_->DrawIndexed();
    triangle_primitive_4_->DrawIndexed();
}
