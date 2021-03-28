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

#ifndef frame_buffer_app_h__
#define frame_buffer_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_rendered_texture.h"
#include "../klib/kgl_frame_buffer.h"
#include "../klib/kgl_source_texture.h"

struct HDRLightStruct 
{
    glm::vec3 Position;
    glm::vec3 Color;
};

class HDRApp : public kgl::App
{
public:
    HDRApp();
    virtual ~HDRApp();
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
    virtual void RenderGUI() override;
private:
    /// <summary>
    /// 用来执行hdr光照计算所用到的frame buffer
    /// </summary>
    kgl::FrameBuffer* hdr_buffer_ = nullptr;

    /// <summary>
    /// 执行光照计算的shader
    /// </summary>
    kgl::GPUProgram* lighting_shader_ = nullptr;

    /// <summary>
    /// 执行hdr色调映射的shader
    /// </summary>
    kgl::GPUProgram* hdr_shader_ = nullptr;
    
    /// <summary>
    /// 深度状态
    /// </summary>
    kgl::RenderStateDepth rs_depth_;
    
    /// <summary>
    /// 场景中的模板纹理
    /// </summary>
    kgl::SourceTextureSPtr wood_texture_;

    /// <summary>
    /// 基于NDC坐标的，占满了整个屏幕的矩形图元
    /// </summary>
    kgl::PrimitiveSPtr screen_rectangle_;
    
    /// <summary>
    /// 用来表示场景的盒子图元
    /// </summary>
    kgl::PrimitiveSPtr cube_;

    /// <summary>
    /// 灯光的位置
    /// </summary>
    std::vector<glm::vec3> light_positions_;
    
    /// <summary>
    /// 灯光的颜色
    /// </summary>
    std::vector<glm::vec3> light_colors_;

    /// <summary>
    /// 是否启用hdr效果
    /// </summary>
    bool use_hdr_ = true;

    /// <summary>
    /// HDR曝光值
    /// </summary>
    float exposure_ = 2.0f;
};


#endif // frame_buffer_app_h__