/**************************************************************************************************************************
Copyright(C) 2014-2025 www.xionggf.com

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
#ifndef instance_app_h__
#define instance_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "mesh.h"

class InstanceApp : public kgl::App
{
protected:
    virtual void InitModel() override;
 
    virtual void RenderGUI() override;

    virtual void InitShaders() override;

    virtual void UpdateScene() override;

    virtual void PreRenderFrame() override;

    virtual void PostRenderFrame() override;

    virtual void RenderScene() override;

    void UpdateCamera(float rotation_y, float rotation_x, uint32_t viewport_width, uint32_t viewport_height);

protected:
    bool show_demo_window_ = false;
    bool show_another_window_ = false;
    kgl::GPUProgram* occluder_program;
    GLuint occluder_instances_buffer;
    GLDrawable* box;
    std::size_t num_occluder_instances;
    float camera_rotation_y_ = 0.0f;
    float camera_rotation_x_ = 0.0f;
};


#endif // instance_app_h__