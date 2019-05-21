/**************************************************************************************************************************
Copyright(C) 2014-2016 www.xionggf.com

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
/*!
 * \file camera_app.h
 * \date 2019/03/23 10:48
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 动态改变顶点缓冲区数据的应用示例
 *
 * TODO: long description
 *
 * \note
*/
#ifndef camera_app_h__
#define camera_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"

class CameraApp : public kgl::App
{
public:
    CameraApp();
    virtual ~CameraApp();
    virtual void InitScene();
protected:
    virtual void RenderScene();
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode);
    virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos);
    virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset);
    virtual void ProcessInput();
private:
    kgl::GPUProgram*        gpu_program_;
    kgl::Primitive*         rectangle_primitive_;
    kgl::SourceTextureSPtr  texture_1_;
    kgl::SourceTextureSPtr  texture_2_;
};

#endif // camera_app_h__