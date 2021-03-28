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
#include "geometry_shader_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_string_convertor.h"

GeometryShaderApp::GeometryShaderApp()
{
}

GeometryShaderApp::~GeometryShaderApp()
{
    KGL_SAFE_DELETE(model_);
    KGL_SAFE_DELETE(colored_polygon_primitive_);
    KGL_SAFE_DELETE(model_);
    colored_polygon_shader_.reset();
    explode_effect_shader_.reset();
}

void GeometryShaderApp::PreRenderFrame()
{
    glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GeometryShaderApp::InitModel()
{
    float points[] =
    {
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top-left
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // top-right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
    };

    kgl::VertexAttribute va_position;
    va_position.index = 0;
    va_position.normalized = GL_FALSE;
    va_position.type = GL_FLOAT;
    va_position.size = 2; // 一个“顶点位置”的属性由3个分量组成
    va_position.stride = 5 * sizeof(GLfloat); // 每个顶点的步长为 
    va_position.pointer = nullptr;

    kgl::VertexAttribute va_color;
    va_color.index = 1;
    va_color.normalized = GL_FALSE;
    va_color.type = GL_FLOAT;
    va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
    va_color.stride = 5 * sizeof(GLfloat); // 每个位置的步长为 
    va_color.pointer = reinterpret_cast<GLvoid*> (2 * sizeof(GLfloat));

    std::vector<kgl::VertexAttribute> vtx_attri_array;
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_color);

    colored_polygon_primitive_ = new kgl::Primitive;
    colored_polygon_primitive_->Create(GL_POINTS, points, sizeof(points), 4, GL_STATIC_DRAW, vtx_attri_array);

    std::string model_path("resources/model/nanosuit.obj");
    model_ = new kgl::BasicStaticMesh;
    model_->LoadMesh(model_path);

    rs_depth_.SetEnable(GL_TRUE);
}


void GeometryShaderApp::InitShaders()
{
    // 初始化shader
    colored_polygon_shader_ = std::make_shared<kgl::GPUProgram>();

    colored_polygon_shader_->CreateFromFile(
        "resources/shader/014_geometry_shader_vs.glsl",
        "resources/shader/014_geometry_shader_fs.glsl",
        "resources/shader/014_geometry_shader_gs.glsl");

    // 初始化shader
    explode_effect_shader_ = std::make_shared<kgl::GPUProgram>();

    explode_effect_shader_->CreateFromFile(
        "resources/shader/014_explode_vs.glsl",
        "resources/shader/014_explode_fs.glsl",
        "resources/shader/014_explode_gs.glsl");
}

void GeometryShaderApp::InitMainCamera()
{
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(0.0f, 0.0f, 40.0f));
}

void GeometryShaderApp::RenderScene()
{
    main_camera_->Update();
    rs_depth_.Use();

    switch (current_render_)
    {
    default:
    case 0:this->RenderColoredPolygon(); break;
    case 1:this->RenderExplodeModel(); break;
    }
}

void GeometryShaderApp::RenderColoredPolygon()
{
    colored_polygon_shader_->Use();
    colored_polygon_primitive_->Draw();
}

void GeometryShaderApp::RenderExplodeModel()
{
    glm::mat4 model_matrix = glm::identity<glm::mat4>();
    const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
    const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
    explode_effect_shader_->Use();
    explode_effect_shader_->ApplyMatrix(glm::value_ptr(model_matrix), "model");
    explode_effect_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view");
    explode_effect_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection");
    explode_effect_shader_->ApplyFloat(static_cast<float>(glfwGetTime()), "time");
    model_->Render();
}

void GeometryShaderApp::RenderGUI()
{
	const glm::vec3& camera_pos = main_camera_->GetPosition();
	ImGui::Begin("014 Geometry Shader -- 几何着色器的简单使用");
	ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("摄像机坐标: (%.1f,%.1f,%.1f)", camera_pos.x, camera_pos.y, camera_pos.z);
	ImGui::Text("按WSADUJ键向前后左右上下移动摄像机,按下左右箭头键，切换当前渲染的内容");
	ImGui::End();
}

void GeometryShaderApp::ProcessInput()
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

    if (key_state_[GLFW_KEY_R])
    {
        main_camera_->ChangeYaw(0.01f);
    }

    if (key_state_[GLFW_KEY_UP])
    {
        main_camera_->ChangePitch(0.01f);
    }

    if (key_state_[GLFW_KEY_DOWN])
    {
        main_camera_->ChangePitch(-0.01f);
    }

    if (key_state_[GLFW_KEY_LEFT])
    {
        main_camera_->ChangeYaw(0.01f);
    }

    if (key_state_[GLFW_KEY_RIGHT])
    {
        main_camera_->ChangeYaw(-0.01f);
    }
}

void GeometryShaderApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    App::OnKeyAction(window, key, scancode, action, mode);

    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
        if (current_render_ == 0)
        {
            current_render_ = 1;
            return;
        }

        current_render_--;
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    {
        current_render_++;

        if (current_render_ >= 1)
            current_render_ = 0;
    }

    switch (current_render_)
    {
    case 0:
        break;
    case 1:
        main_camera_->SetPosition(glm::vec3(0.000001f,6.5f,47.0f));
        break;
    }
}

void GeometryShaderApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    App::OnMouseAction(window, xpos, ypos);
}

void GeometryShaderApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
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