/*!
 * \file deferred_rendering_app.h
 * \date 2017/12/31 15:17
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 延迟着色的案例
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
#ifndef deferred_rendering_app_h__
#define deferred_rendering_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_material.h"
#include "../klib/kgl_light.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_gbuffer.h"
#include "../klib/kgl_basic_static_mesh.h"

class DeferredRenderingApp : public kgl::App
{
private:
    static const uint32_t POINT_LIGHT_COUNT = 3;
public:
    /// <summary>
    /// <see cref="DeferredRenderingApp"/> 类构造函数.
    /// </summary>
    DeferredRenderingApp();

    /// <summary>
    /// <see cref="DeferredRenderingApp"/> 类析构函数.
    /// </summary>
    virtual ~DeferredRenderingApp();
    
    /// <summary>
    /// 初始化场景，继承实现自基类App
    /// </summary>
    virtual void InitScene() override;
protected:
    /// <summary>
    /// 在渲染每一帧前的操作，继承实现自基类App
    /// </summary>
    virtual void PreRenderFrame() override;

    /// <summary>
    /// 渲染每一帧，继承实现自基类App
    /// </summary>
    virtual void RenderFrame() override;

    /// <summary>
    /// 处理用户输入的函数，继承实现自基类App
    /// </summary>
    virtual void ProcessInput() override;

    /// <summary>
    /// Called when [key action].
    /// </summary>
    /// <param name="window">GLFW框架的窗口类指针</param>
    /// <param name="key">键盘鼠标按键值</param>
    /// <param name="scancode">键盘扫描码</param>
    /// <param name="action">The action.</param>
    /// <param name="mode">The mode.</param>
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;

    /// <summary>
    /// 初始化场景中用到的材质
    /// </summary>
    virtual void InitMaterials() override;

    /// <summary>
    /// 初始化场景中的模型
    /// </summary>
    virtual void InitModel() override;

    /// <summary>
    /// 初始化场景中的各个shader
    /// </summary>
    virtual void InitShaders() override;

    /// <summary>
    /// 初始化场景中的各个动态实时光源
    /// </summary>
    virtual void InitLights() override;

    /// <summary>
    /// 初始化场景的主摄像机
    /// </summary>
    virtual void InitMainCamera() override;

    /// <summary>
    /// 初始化字体库
    /// </summary>
    virtual void InitFont() override;
private:
    /// <summary>
    /// 初始化盒子模型的位置
    /// </summary>
    void InitBoxMeshPositions();

    /// <summary>
    /// 绘制帮助文档
    /// </summary>
    /// <param name="view_pos">摄像机在世界空间中的坐标</param>
    void RenderHelpText(const glm::vec3& view_pos);
private:
    /// <summary>
    /// 用来进行延迟渲染操作用的G-buffer
    /// </summary>
    kgl::GBuffer* gbuffer_ = nullptr;

    /// <summary>
    /// 处理几何信息的shader
    /// </summary>
    kgl::GPUProgram* geometry_process_shader_ = nullptr;

    /// <summary>
    /// 计算光照的shader
    /// </summary>
    kgl::GPUProgram* lighting_shader_ = nullptr;

    /// <summary>
    /// 用来绘制表示光源的小方盒子的shader
    /// </summary>
    kgl::GPUProgram* light_sphere_shader_ = nullptr;

    /// <summary>
    /// 点光源结构体数组
    /// </summary>
    std::vector<kgl::PointLight> point_lights_;
    
    /// <summary>
    /// 深度状态类
    /// </summary>
    kgl::RenderStateDepth rs_depth_;

    /// <summary>
    /// 用于多次渲染的模型位置点
    /// </summary>
    std::vector<glm::vec3> box_positions_;

    /// <summary>
    /// 帮助文档打开时的帮助内容字符串
    /// </summary>
    std::wstring toggle_help_on_text_;

    /// <summary>
    /// 帮助文档关闭时的帮助内容字符串
    /// </summary>
    std::wstring toggle_help_off_text_;

    /// <summary>
    /// 显示摄像机位置信息的字符串
    /// </summary>
    std::wstring camera_ctrl_text_;
    
    /// <summary>
    /// 帮助是否打开
    /// </summary>
    bool is_help_on_ = false;

    /// <summary>
    /// 盒子模型
    /// </summary>
    kgl::BasicStaticMesh* box_mesh_ = nullptr;

    /// <summary>
    /// 表征光源的球状模型
    /// </summary>
    kgl::BasicStaticMesh* light_sphere_mesh_ = nullptr;

    /// <summary>
    /// 基于NDC坐标的，占满了整个屏幕的矩形图元
    /// </summary>
    kgl::PrimitiveSPtr screen_rectangle_;
};

#endif // deferred_rendering_app_h__