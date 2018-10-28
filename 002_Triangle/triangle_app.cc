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
#include "triangle_app.h"
#include "../klib/kgl_defines.h"

TriangleApp::TriangleApp() :gpu_program_(nullptr), rectangle_primitive_(nullptr),
triangle_primitive_1_(nullptr), triangle_primitive_2_(nullptr), 
triangle_primitive_3_(nullptr), triangle_primitive_4_(nullptr)
{
}

TriangleApp::~TriangleApp()
{
    KGL_SAFE_DELETE(gpu_program_);
    KGL_SAFE_DELETE(rectangle_primitive_);
    KGL_SAFE_DELETE(triangle_primitive_3_);
    KGL_SAFE_DELETE(triangle_primitive_2_);
    KGL_SAFE_DELETE(triangle_primitive_3_);
    KGL_SAFE_DELETE(triangle_primitive_4_);
}

void TriangleApp::InitGuiSystem(bool use_gui, const char* bar_title)
{
    App::InitGuiSystem(use_gui, bar_title);

    if (!use_gui)
        return;

    GLFWvidmode mode;   // GLFW video mode
    
    /*
     time = 0, dt;// Current time and enlapsed time
     turn = 0;    // Model turn counter
     speed = 0.3; // Model rotation speed
     wire = 0;       // Draw model in wireframe?
     bgColor = { 0.1f, 0.2f, 0.4f };         // Background color 
     cubeColor = { 255, 0, 0, 128 }; // Model color (32bits RGBA)
     */
    bgColor[0] = 0.1f; bgColor[1] = 0.2f; bgColor[2] = 0.4f;
    cubeColor[0] = 255; cubeColor[1] = 0; cubeColor[2] = 0; cubeColor[3] = 128;
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.

    // Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
    TwAddVarRW(tw_gui_bar_, "speed", TW_TYPE_DOUBLE, &speed,
        " label='Rot speed' min=0 max=2 step=0.01 keyIncr=s keyDecr=S help='Rotation speed (turns/second)' ");

    // Add 'wire' to 'bar': it is a modifable variable of type TW_TYPE_BOOL32 (32 bits boolean). Its key shortcut is [w].
    TwAddVarRW(tw_gui_bar_, "wire", TW_TYPE_BOOL32, &wire,
        " label='Wireframe mode' key=w help='Toggle wireframe display mode.' ");

    // Add 'time' to 'bar': it is a read-only (RO) variable of type TW_TYPE_DOUBLE, with 1 precision digit
    TwAddVarRO(tw_gui_bar_, "time", TW_TYPE_DOUBLE, &time, " label='Time' precision=1 help='Time (in seconds).' ");

    // Add 'bgColor' to 'bar': it is a modifable variable of type TW_TYPE_COLOR3F (3 floats color)
    TwAddVarRW(tw_gui_bar_, "bgColor", TW_TYPE_COLOR3F, &bgColor[0], " label='Background color' ");

    // Add 'cubeColor' to 'bar': it is a modifable variable of type TW_TYPE_COLOR32 (32 bits color) with alpha
    TwAddVarRW(tw_gui_bar_, "cubeColor", TW_TYPE_COLOR32, &cubeColor[0],
        " label='Cube color' alpha help='Color and transparency of the cube.' ");
}

void TriangleApp::InitModel()
{
    gpu_program_ = new kgl::GPUProgram;
    const GLchar* vs_file_path = nullptr;
    const GLchar* fs_file_path = nullptr;
#if defined(WIN32) || defined(_WIN32)
    vs_file_path = "resources/shader/002_triangle_vs.glsl";
    fs_file_path = "resources/shader/002_triangle_fs.glsl";
#elif defined(__APPLE__) && defined(__MACH__)
    vs_file_path = "/Users/xiongxinke/Desktop/SvnChina/kgl/publish/resources/shader/002_triangle_vs.glsl";
    fs_file_path = "/Users/xiongxinke/Desktop/SvnChina/kgl/publish/resources/shader/002_triangle_fs.glsl";
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
    triangle_primitive_4_->CreateIndexed(GL_TRIANGLES, triangle_4_vertices, sizeof(triangle_4_vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, triangle_indices, sizeof(triangle_indices), GL_STATIC_DRAW, vtx_attri_array);


    // - Directly redirect GLFW mouse button events to AntTweakBar
//  glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    // - Directly redirect GLFW mouse position events to AntTweakBar
//  glfwSetMousePosCallback((GLFWmouseposfun)TwEventMousePosGLFW);
    // - Directly redirect GLFW mouse wheel events to AntTweakBar
//  glfwSetMouseWheelCallback((GLFWmousewheelfun)TwEventMouseWheelGLFW);
    // - Directly redirect GLFW key events to AntTweakBar
//  glfwSetKeyCallback((GLFWkeyfun)TwEventKeyGLFW);
    // - Directly redirect GLFW char events to AntTweakBar
//  glfwSetCharCallback((GLFWcharfun)TwEventCharGLFW);

}

void TriangleApp::RenderFrame()
{
    time = glfwGetTime();
    gpu_program_->Use();
    rectangle_primitive_->DrawIndexed();
    triangle_primitive_1_->DrawIndexed();
    triangle_primitive_2_->DrawIndexed();
    triangle_primitive_3_->DrawIndexed();
    triangle_primitive_4_->DrawIndexed();

    TwDraw();
}
