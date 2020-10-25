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
* \file frame_buffer_app.h
* \date 2017/12/31 15:01
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO:
*
* \note
*/
#ifndef frame_buffer_app_h__
#define frame_buffer_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_rendered_texture.h"
#include "../klib/kgl_frame_buffer.h"

class FrameBufferApp : public kgl::App
{
public:
    FrameBufferApp();
    virtual ~FrameBufferApp();
    virtual void InitScene();
protected:
    virtual void PreRenderFrame() override;
    virtual void RenderScene() override;
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
    virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
    virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset) override;
    virtual void ProcessInput() override;
    virtual void InitMaterials() override;
    virtual void InitModel() override;
    virtual void InitShaders() override;
    virtual void InitLights() override;
    virtual void InitMainCamera() override;
    virtual void InitFont() override;
private:
    /// <summary>
    /// The scene_obj_shader_
    /// </summary>
    kgl::GPUProgram* floor_shader_ = nullptr;

    /// <summary>
    /// The wood_box_shader_
    /// </summary>
    kgl::GPUProgram* wood_box_shader_ = nullptr;
    
    /// <summary>
    /// The screen_shader_
    /// </summary>
    kgl::GPUProgram* screen_shader_ = nullptr;
    
    /// <summary>
    /// 帧缓冲区对象
    /// </summary>
    kgl::FrameBuffer* frame_buffer_ = nullptr;

    /// <summary>
    /// 用来绘制箱子的图元
    /// </summary>
    kgl::PrimitiveSPtr box_;

    /// <summary>
    /// 用来绘制地板的图元
    /// </summary>
    kgl::PrimitiveSPtr floor_;

    /// <summary>
    /// 用来绘制帧缓冲区内容的图元
    /// </summary>
    kgl::PrimitiveSPtr screen_;
    
    /// <summary>
    /// The floor_texture_
    /// </summary>
    kgl::SourceTextureSPtr floor_texture_;

    /// <summary>
    /// The box_texture_
    /// </summary>
    kgl::SourceTextureSPtr box_texture_;

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
    /// The floor_translation_
    /// </summary>
    glm::vec3 floor_translation_;

    /// <summary>
    /// The cull_mode_
    /// </summary>
    kgl::RenderStateCullMode cull_mode_;

    /// <summary>
    /// The depth_test_mode_
    /// </summary>
    kgl::RenderStateDepth depth_test_mode_;

    /// <summary>
    /// The texture_sun_
    /// </summary>
    kgl::SourceTextureSPtr texture_sun_;

    /// <summary>
    /// The sun_
    /// </summary>
    kgl::PrimitiveSPtr sun_;

    /// <summary>
    /// The scene_shader_
    /// </summary>
    kgl::GPUProgramSPtr scene_shader_;

    /// <summary>
    /// The draw_fb_to_scr_cull_
    /// </summary>
    kgl::RenderStateCullMode draw_fb_to_scr_cull_;

    /// <summary>
    /// The draw_fb_to_scr_depth_
    /// </summary>
    kgl::RenderStateDepth draw_fb_to_scr_depth_;
};


#endif // frame_buffer_app_h__