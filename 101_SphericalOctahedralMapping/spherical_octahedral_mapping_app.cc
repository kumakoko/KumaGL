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

#include "spherical_octahedral_mapping_app.h"
#include "../klib/kgl_defines.h"


SphericalOctahedralMappingApp::SphericalOctahedralMappingApp():
shader_(nullptr),
rectangle_primitive_(nullptr)
{
}

SphericalOctahedralMappingApp::~SphericalOctahedralMappingApp()
{
    KGL_SAFE_DELETE(shader_);
}

void SphericalOctahedralMappingApp::InitScene()
{
    screen_resolution_ = glm::vec2(960.0f, 640.0f);

// ================================================================
    shader_ = new kgl::GPUProgram;
    shader_->CreateFromFile("resources/shader/shader_joy_vs.glsl", "resources/shader/colorful_ring_fs.glsl", nullptr);

    GLfloat vertices[] =
    {
        // 位置          // 颜色            
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 右上角
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 右下角
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f //左上角
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


    std::vector<kgl::VertexAttribute> vtx_attri_array;
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_color);


    rectangle_primitive_ = new kgl::Primitive;
    rectangle_primitive_->CreateIndexed(GL_TRIANGLES, vertices, sizeof(vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_STATIC_DRAW, vtx_attri_array);
}

void SphericalOctahedralMappingApp::RenderScene()
{
    shader_->Use();
    shader_->ApplyFloat(static_cast<float>(glfwGetTime()), "global_time");
    shader_->ApplyVector2(glm::value_ptr(screen_resolution_), "screen_resolution");
    shader_->ApplyVector3(glm::value_ptr(mouse_input_pos_), "iMouse");
    rectangle_primitive_->DrawIndexed();
}
void SphericalOctahedralMappingApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_RIGHT  && action == GLFW_RELEASE)
    {
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
    }

    App::OnKeyAction(window, key, scancode, action, mode);
}

void SphericalOctahedralMappingApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    mouse_input_pos_.x = static_cast<float>(xpos);
    mouse_input_pos_.y = static_cast<float>(ypos);
    mouse_input_pos_.z = 0.0f;
}

void SphericalOctahedralMappingApp::ProcessInput()
{
    if (key_state_[GLFW_KEY_W])
    {
    }

    if (key_state_[GLFW_KEY_S])
    {
    }
}