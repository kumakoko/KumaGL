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
#include "transform_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"

TransformApp::TransformApp()
{
}

TransformApp::~TransformApp()
{
    KGL_SAFE_DELETE(object_shader_);
    texture_sun_.reset();
    texture_earth_.reset();
    texture_moon_.reset();
    sun_.reset();
    earth_.reset();
    moon_.reset();
}

void TransformApp::InitMaterials()
{

}

void TransformApp::InitModel()
{
    kgl::TextureParams texture_param = kgl::TextureManager::MakeTextureParamsRGB(GL_REPEAT, GL_LINEAR);
    
    texture_sun_ = std::make_shared<kgl::SourceTexture>();
    texture_sun_->CreateFromFile("resources/image/sun.jpg", texture_param);

    texture_earth_ = std::make_shared<kgl::SourceTexture>();
    texture_earth_->CreateFromFile("resources/image/earth.jpg", texture_param);

    texture_moon_ = std::make_shared<kgl::SourceTexture>();
    texture_moon_->CreateFromFile("resources/image/moon.jpg", texture_param);

    sun_ = kgl::PrimitiveTool::BuildSphere(30, true);
    earth_ = kgl::PrimitiveTool::BuildSphere(30, true);
    moon_ = kgl::PrimitiveTool::BuildSphere(30, true);
}

void TransformApp::InitShaders()
{
    object_shader_ = new kgl::GPUProgram;
    object_shader_->CreateFromFile("resources/shader/003_transform_draw_vs.glsl", "resources/shader/003_transform_draw_fs.glsl", nullptr);
}

void TransformApp::InitLights()
{
    App::InitLights();
}

void TransformApp::InitMainCamera()
{
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 20.0f);
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, pos);
}

void TransformApp::InitFont()
{

}

void TransformApp::InitScene()
{
    App::InitScene();
    cull_mode_.SetEnable(GL_TRUE);
    cull_mode_.SetFrontFaceMode(GL_CW);
    cull_mode_.SetCullMode(GL_BACK);
    depth_test_mode_.SetEnable(GL_TRUE);
    depth_test_mode_.SetDepthTestFunc(GL_LESS);
}

void TransformApp::RenderScene()
{
    float current_time = (GLfloat)glfwGetTime() * 0.5f;

    main_camera_->Update();

    const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
    const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();

    cull_mode_.Use();
    depth_test_mode_.Use();

    glm::mat4 matrix_sun;
    matrix_sun = glm::scale(matrix_sun, glm::vec3(5.f, 5.f, 5.f));
    matrix_sun = glm::rotate(matrix_sun, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

    object_shader_->Use();
    object_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
    object_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
    object_shader_->ApplyMatrix(glm::value_ptr(matrix_sun), "world_matrix");
    object_shader_->ApplyTexture(texture_sun_, "source_texture_1", 0);
    sun_->DrawIndexed();

    glm::mat4 matrix_earth;
    matrix_earth = glm::scale(matrix_earth, glm::vec3(2.5f, 2.5f, 2.5f)); // 自身放大1.5倍
    matrix_earth = glm::rotate(matrix_earth, current_time, glm::vec3(0.0f, 1.0f, 0.0f)); // 自身旋转
    matrix_earth = glm::translate(matrix_earth, glm::vec3(4.0f,0.0f,0.0f)); // 先沿着水平方向偏移4个单位
    matrix_earth = glm::rotate(matrix_earth, current_time, glm::vec3(0.0f, 1.0f, 0.0f)); // 绕世界坐标系下的Y轴旋转
    
    object_shader_->ApplyMatrix(glm::value_ptr(matrix_earth), "world_matrix");
    object_shader_->ApplyTexture(texture_earth_, "source_texture_1", 0);
    earth_->DrawIndexed();

    glm::mat4 matrix_moon;

    matrix_moon = glm::scale(matrix_moon, glm::vec3(1.f / 2.5f, 1.f / 2.5f, 1.f / 2.5f));
    matrix_moon = glm::rotate(matrix_moon,current_time,glm::vec3(0.0f, 1.0f, 0.0f)); // 自旋
    matrix_earth = glm::translate(matrix_earth, glm::vec3(1.0f, 0.0f, 0.0f)); // 先沿着水平方向偏移4个单位
    matrix_earth = glm::rotate(matrix_earth, current_time, glm::vec3(0.0f, 1.0f, 0.0f)); // 绕地球局部系下的Y轴旋转
    matrix_moon = matrix_earth * matrix_moon; // 移到地球坐标系下

    object_shader_->ApplyMatrix(glm::value_ptr(matrix_moon), "world_matrix");
    object_shader_->ApplyTexture(texture_moon_, "source_texture_1", 0);
    moon_->DrawIndexed();
}

void TransformApp::ProcessInput()
{
    if (key_state_[GLFW_KEY_W])
    {
        main_camera_->Move(kgl::CameraDirection::FORWARD);
    }

    if (key_state_[GLFW_KEY_S])
    {
        main_camera_->Move(kgl::CameraDirection::BACK);
    }
}
