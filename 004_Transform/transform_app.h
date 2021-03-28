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
/*!
* \file transform_app.h
* \date 2017/12/31 14:43
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief 演示图元的坐标变换操作
*
* TODO: 演示使用kgl::RenderStateCullMode类
*       演示使用kgl::RenderStateDepth类
*
* \note
*/
#ifndef transform_app_h__
#define transform_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_render_state_draw_mode.h"

class TransformApp : public kgl::App
{
public:
    TransformApp();
    virtual ~TransformApp();
    virtual void InitScene();
protected:
    virtual void RenderScene() override;
    virtual void ProcessInput() override;
    virtual void InitMaterials() override;
    virtual void InitModel() override;
    virtual void InitShaders() override;
    virtual void InitLights() override;
    virtual void InitMainCamera() override;
    virtual void RenderGUI() override;
private:
    kgl::GPUProgram*            object_shader_;
    kgl::SourceTextureSPtr      texture_sun_;
    kgl::SourceTextureSPtr      texture_earth_;
    kgl::SourceTextureSPtr      texture_moon_;
    kgl::PrimitiveSPtr          sun_;
    kgl::PrimitiveSPtr          earth_;
    kgl::PrimitiveSPtr          moon_;
    kgl::RenderStateCullMode    cull_mode_;
    kgl::RenderStateDepth       depth_test_mode_;
	kgl::RenderStateDrawMode    draw_mode_;
    bool                        wireframe_mode_;
};

#endif // transform_app_h__