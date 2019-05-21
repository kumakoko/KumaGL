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
* \file normal_mapping_app.h
* \date 2017/12/31 15:22
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: 演示使用法线贴图显示出凹凸的效果
*
* \note
*/
#ifndef normal_mapping_app_h__
#define normal_mapping_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_light.h"

class NormalMappingApp : public kgl::App
{
public:
    NormalMappingApp();
    virtual ~NormalMappingApp();
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
    void RenderText();
private:
    /// <summary>
    /// 用来绘制法线贴图凹凸效果的的shader
    /// </summary>
    kgl::GPUProgramSPtr normal_mapping_shader_;

    /// <summary>
    /// 表示墙壁贴图的图元
    /// </summary>
    kgl::PrimitiveSPtr wall_square_;

    /// <summary>
    /// 漫反射贴图，所有顶点的漫反射光照信息都预存在这个贴图了
    /// </summary>
    kgl::SourceTextureSPtr diffuse_map_texture_;

    /// <summary>
    /// 法线贴图
    /// </summary>
    kgl::SourceTextureSPtr normal_map_texture_;

    /// <summary>
    /// 绘制两个面片时使用的拣选模式
    /// </summary>
    kgl::RenderStateCullMode draw_square_cull_mode_;

    /// <summary>
    /// 绘制面片时使用的深度模式
    /// </summary>
    kgl::RenderStateDepth draw_square_depth_mode_;

    /// <summary>
    /// 用来进行照明计算用的点光源
    /// </summary>
    kgl::PointLight point_light_;

    /// <summary>
    /// 按下H键出现提示帮助的字符串
    /// </summary>
    std::wstring toggle_help_on_text_;

    /// <summary>
    /// 按下H键关闭提示帮助的字符串
    /// </summary>
    std::wstring toggle_help_off_text_;

    /// <summary>
    /// 开关变量，用于表示是否打开帮助提示
    /// </summary>
    bool is_toggle_on_ = false;

    /// <summary>
    /// 镜面反射用到的辉光值
    /// </summary>
    float shininess_ = 32.0f;
};


#endif // normal_mapping_app_h__