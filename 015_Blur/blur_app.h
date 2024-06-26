﻿/**************************************************************************************************************************
Copyright(C) 2014 - 2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
************************************************************************************************************************** /
/*!
 * \file blur_app.h
 * \date 2017/12/31 15:16
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 展示全屏模糊的渲染技术
*/

#ifndef blur_app_h__
#define blur_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_sky_box.h"
#include "../klib/kgl_basic_static_mesh.h"
#include "../klib/kgl_light.h"
#include "../klib/kgl_frame_buffer.h"

class BlurApp : public kgl::App
{
public:
    BlurApp();
    virtual ~BlurApp();
    virtual void InitScene();
protected:
    virtual void RenderScene() override;
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
    virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
    virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset) override;
    virtual void ProcessInput() override;
    virtual void InitModel() override;
    virtual void InitShaders() override;
    virtual void InitMainCamera() override;
    virtual void InitLights() override;
    virtual void InitMaterials() override;
private:
    /// <summary>
    /// The model_
    /// </summary>
    kgl::BasicStaticMesh* model_ = nullptr;

    /// <summary>
    /// The frame_buffer_
    /// </summary>
    kgl::FrameBuffer* frame_buffer_ = nullptr;

    /// <summary>
    /// The model_shader_
    /// </summary>
    kgl::GPUProgramSPtr model_shader_;

    /// <summary>
    /// 用来产生blur效果的shader;
    /// </summary>
    kgl::GPUProgramSPtr blur_shader_;

    /// <summary>
    /// The rs_depth_
    /// </summary>
    kgl::RenderStateDepth rs_depth_;

    /// <summary>
    /// The directional_light_
    /// </summary>
    kgl::DirectionalLight directional_light_;

    /// <summary>
    /// The material_
    /// </summary>
    kgl::Material material_;

    /// <summary>
    /// 绘制scene object到frame buffer时使用的拣选模式
    /// </summary>
    kgl::RenderStateCullMode write_fb_cull_;

    /// <summary>
    /// 向frame buffer写入数据时的深度模式
    /// </summary>
    kgl::RenderStateDepth write_fb_depth_;

    /// <summary>
    /// 绘制frame buffer object上的数据到屏幕中的深度值
    /// </summary>
    kgl::RenderStateDepth draw_fb_to_screen_depth_;

    /// <summary>
    /// The draw_fb_to_scr_cull_
    /// </summary>
    kgl::RenderStateCullMode draw_fb_to_scr_cull_;

    /// <summary>
    /// The draw_fb_to_scr_depth_
    /// </summary>
    kgl::RenderStateDepth draw_fb_to_scr_depth_;

    /// <summary>
    /// 用来绘制帧缓冲区内容的图元
    /// </summary>
    kgl::PrimitiveSPtr screen_;

    /// <summary>
    /// 于0时表示进行模糊操作，小于0表示不进行模糊操作
    /// </summary>
    int use_blur_ = 1;

    /// <summary>
    /// 大于0时表示水平方向上进行模糊操作，小于0表示垂直方向上进行模糊操作
    /// </summary>
    int blur_horizontal_ = 1;
};


#endif // blur_app_h__