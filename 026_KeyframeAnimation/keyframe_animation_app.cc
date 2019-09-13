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
#include "keyframe_animation_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"

KeyframeAnimationApp::KeyframeAnimationApp()
{
}

KeyframeAnimationApp::~KeyframeAnimationApp()
{

}

void KeyframeAnimationApp::InitMaterials()
{

}

void KeyframeAnimationApp::InitModel()
{
	//std::string file_path = "resources/model/md2/sphere.obj";
	//std::string file_path = "resources/model/md2/md2file.md2";
	//std::string file_path = "resources/model/box2/box.obj";
	//model_.LoadFromFile(file_path);
}

void KeyframeAnimationApp::InitShaders()
{
  
}

void KeyframeAnimationApp::InitLights()
{
    App::InitLights();
}

void KeyframeAnimationApp::InitMainCamera()
{
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 20.0f);
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, pos);
}

void KeyframeAnimationApp::RenderGUI()
{
    const glm::vec3& camera_pos = main_camera_->GetPosition();
    ImGui::Begin("004 Transform -- 变换操作");
    ImGui::Text("按W键向前，S键向后移动摄像机");
    ImGui::Text("摄像机坐标: (%.1f,%.1f,%.1f)", camera_pos.x, camera_pos.y, camera_pos.z);
    ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}

void KeyframeAnimationApp::InitScene()
{
    App::InitScene();
    cull_mode_.SetEnable(GL_TRUE);
    cull_mode_.SetFrontFaceMode(GL_CW);
    cull_mode_.SetCullMode(GL_BACK);
    depth_test_mode_.SetEnable(GL_TRUE);
    depth_test_mode_.SetDepthTestFunc(GL_LESS);
}

void KeyframeAnimationApp::RenderScene()
{
    float current_time = (GLfloat)glfwGetTime() * 0.5f;

    main_camera_->Update();

}

void KeyframeAnimationApp::ProcessInput()
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
