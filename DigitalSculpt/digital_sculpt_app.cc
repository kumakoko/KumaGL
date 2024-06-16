/**************************************************************************************************************************
Copyright(C) 2014-2024 www.xionggf.com

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
#include "digital_sculpt_app.h"

DigitalSculptApp::DigitalSculptApp()
{
}

DigitalSculptApp::~DigitalSculptApp()
{
}

void DigitalSculptApp::InitScene()
{
}

void DigitalSculptApp::RenderScene()
{
}

void DigitalSculptApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_RIGHT  && action == GLFW_RELEASE)
    {
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
    }

    App::OnKeyAction(window, key, scancode, action, mode);
}

void DigitalSculptApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
    mouse_input_pos_.x = static_cast<float>(xpos);
    mouse_input_pos_.y = static_cast<float>(ypos);
}

void DigitalSculptApp::ProcessInput()
{
    if (key_state_[GLFW_KEY_W])
    {
    }

    if (key_state_[GLFW_KEY_S])
    {
    }
}