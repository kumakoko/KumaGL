/*!
 * \file pixel_magic_app.h
 * \date 2017/12/31 15:26
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
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
#ifndef digital_sculpt_app_h__
#define digital_sculpt_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class DigitalSculptApp : public kgl::App
{
public:
    DigitalSculptApp();
    virtual ~DigitalSculptApp();
    virtual void InitScene()  override;
protected:
    virtual void RenderScene() override;
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
    virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
    virtual void ProcessInput() override;
private:
    glm::vec2               screen_resolution_;
    glm::vec2               mouse_input_pos_;
};

#endif // digital_sculpt_app_h__