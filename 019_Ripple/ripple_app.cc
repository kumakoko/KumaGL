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

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_debug_tool.h"
#include "ripple_app.h"

// http://blog.csdn.net/dreamcs/article/details/7691690

RippleApp::RippleApp()
{
    
}

RippleApp::~RippleApp()
{
    
}

void RippleApp::InitScene()
{
    App::InitScene();
    
    
}

void RippleApp::PreRenderFrame()
{
    App::PreRenderFrame();
}

void RippleApp::RenderFrame()
{
    main_camera_->Update();
}

void RippleApp::WaterSimulation()
{

    
}


void RippleApp::HeightmapToNormal()
{
    
}

void RippleApp::RenderWater()
{

}


void RippleApp::InitMaterials()
{
    App::InitMaterials();
}

void RippleApp::InitModel()
{
    
}

void RippleApp::InitShaders()
{

}

void RippleApp::InitLights()
{
    App::InitLights();
}

void RippleApp::InitMainCamera()
{
    
}

void RippleApp::InitFont()
{
    App::InitFont();
}

void RippleApp::ProcessInput()
{
    if (key_state_[GLFW_KEY_W])
    {
        main_camera_->Move(kgl::CameraDirection::FORWARD);
    }

    if (key_state_[GLFW_KEY_S])
    {
        main_camera_->Move(kgl::CameraDirection::BACK);
    }

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


    if (key_state_[GLFW_KEY_DOWN])
    {
        
    }

    if (key_state_[GLFW_KEY_UP])
    {
        
    }

    if (key_state_[GLFW_KEY_LEFT])
    {
        
    }

    if (key_state_[GLFW_KEY_RIGHT])
    {
        
    }

    if (key_state_[GLFW_KEY_HOME])
    {
        
    }

    if (key_state_[GLFW_KEY_END])
    {
        
    }
}

void RippleApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    App::OnKeyAction(window, key, scancode, action, mode);
}

void RippleApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    App::OnMouseAction(window, xpos, ypos);
}

void RippleApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
    App::OnScrollAction(window, xoffset, yoffset);
}