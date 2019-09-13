/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

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
* \file hdr_app.h
* \date 2018/01/02 9:50
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief  展示使用HDR的技术
*
* TODO: long description
*
* \note
*/

#ifndef shadow_mapping_app_h__
#define shadow_mapping_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_rendered_texture.h"
#include "../klib/kgl_frame_buffer.h"
#include "../klib/kgl_source_texture.h"
#include "render_pass_shadow_mapping.h"
#include "render_pass_shadow_mapping_depth.h"

class ShadowMappingApp : public kgl::App
{
public:
    ShadowMappingApp();
    virtual ~ShadowMappingApp();
    virtual void InitScene();
protected:
    virtual void PreRenderFrame() override;
    virtual void RenderScene() override;
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
    virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
    virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset) override;
    virtual void ProcessInput() override;
    virtual void InitModel() override;
    virtual void InitShaders() override;
    virtual void InitLights() override;
    virtual void InitMainCamera() override;
    virtual void InitFont() override;
    void RenderHelpText(const glm::vec3& view_pos);
    void RenderSceneDepthToBuffer();
    void RenderSceneWithShadow();
    void RenderDebugScreen();
    void CreateFrameBufferForDepth();
private:
    /// <summary>
    /// The shadow_mapping_pass_
    /// </summary>
    RenderPassShadowMapping* shadow_mapping_pass_ = nullptr;

    /// <summary>
    /// The shadow_mapping_depth_pass_
    /// </summary>
    RenderPassShadowMappingDepth* shadow_mapping_depth_pass_ = nullptr;

    /// <summary>
    /// 用来执行阴影计算所用到的frame buffer
    /// </summary>
    kgl::FrameBuffer* shadow_depth_buffer_ = nullptr;

    /// <summary>
    /// 深度状态
    /// </summary>
    kgl::RenderStateDepth rs_depth_;

    /// <summary>
    /// The floor_
    /// </summary>
    kgl::PrimitiveSPtr floor_;

    /// <summary>
    /// The box_
    /// </summary>
    kgl::PrimitiveSPtr box_;

    /// <summary>
    /// 用来绘制帧缓冲区内容的图元
    /// </summary>
    kgl::PrimitiveSPtr debug_screen_;

    kgl::SourceTextureSPtr wood_texture_;

    kgl::SourceTextureSPtr stone_texture_;

    kgl::RenderStateCullMode cull_mode_draw_box_;

    kgl::RenderStateCullMode cull_mode_draw_floor_;

    kgl::RenderStateCullMode draw_fb_to_scr_cull_;
    
    kgl::RenderStateDepth draw_fb_to_scr_depth_;

    kgl::GPUProgram* debug_screen_shader_ = nullptr;
    /// <summary>
    /// 提示如何关闭帮助文档的信息字符串
    /// </summary>
    std::wstring toggle_help_off_text_;

    /// <summary>
    ///  提示如何打开帮助文档的信息字符串
    /// </summary>
    std::wstring toggle_help_on_text_;

    /// <summary>
    /// 摄像机相关信息的字符串
    /// </summary>
    std::wstring camera_ctrl_text_;

    /// <summary>
    /// 帮助文档是否处于打开状态
    /// </summary>
    bool is_help_on_ = true;

    /// <summary>
    /// 是否开启顶点偏移和多采样以优化阴影质量
    /// </summary>
    bool use_bias_sample_shadow_ = true;
};


#endif // shadow_mapping_app_h__