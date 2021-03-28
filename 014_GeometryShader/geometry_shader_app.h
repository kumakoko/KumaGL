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
* \file geometry_shader_app.h
* \date 2017/12/31 15:16
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief 展示几何着色器的一些相关操作
*/

#ifndef geometry_shader_app_h__
#define geometry_shader_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_basic_static_mesh.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"

class GeometryShaderApp : public kgl::App
{
public:
    GeometryShaderApp();
    virtual ~GeometryShaderApp();
protected:
    virtual void PreRenderFrame() override;
    virtual void InitModel() override;
    virtual void InitShaders() override;
    virtual void InitMainCamera() override;
    virtual void InitFont() override;
    virtual void RenderScene() override;
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
    virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
    virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset) override;
    virtual void ProcessInput() override;
    virtual void RenderGUI() override;
private:
    /// <summary>
    /// 绘制彩色多边形
    /// </summary>
    void RenderColoredPolygon();

    /// <summary>
    /// 绘制爆炸的模型
    /// </summary>
    void RenderExplodeModel();
private:
    /// <summary>
    /// 绘制彩色多边形用的shader
    /// </summary>
    kgl::GPUProgramSPtr     colored_polygon_shader_;

    /// <summary>
    /// 绘制爆炸效果的shader
    /// </summary>
    kgl::GPUProgramSPtr     explode_effect_shader_;

    /// <summary>
    /// 启用深度buffer的render state
    /// </summary>
    kgl::RenderStateDepth   rs_depth_;
    
    /// <summary>
    /// 绘制房子用的图元
    /// </summary>
    kgl::Primitive*         colored_polygon_primitive_ = nullptr;

    /// <summary>
    /// 演示爆炸效果的模型
    /// </summary>
    kgl::BasicStaticMesh*   model_ = nullptr;

    /// <summary>
    /// 当前待渲染的内容的编号
    /// </summary>
    int                     current_render_ = 1;

    /// <summary>
    /// The toggle_help_on_text_
    /// </summary>
    std::wstring            toggle_help_on_text_;

    /// <summary>
    /// The toggle_help_off_text_
    /// </summary>
    std::wstring            toggle_help_off_text_;

    /// <summary>
    /// The camera_ctrl_text_
    /// </summary>
    std::wstring            camera_ctrl_text_;

    /// <summary>
    /// The material_ctrl_text_
    /// </summary>
    std::wstring            material_ctrl_text_;

    /// <summary>
    /// 是否显示帮助文档
    /// </summary>
    bool                    is_help_on_ = true;
};


#endif // triangle_app_h__