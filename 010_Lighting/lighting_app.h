/*!
 * \file lighting_app.h
 * \date 2017/12/31 14:53
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 光照计算演示示例
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
#ifndef lighting_app_h__
#define lighting_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_material.h"
#include "../klib/kgl_light.h"
#include "../klib/kgl_basic_static_mesh.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_render_state_draw_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_render_state_blend.h"

class LightingApp : public kgl::App
{
public:
    LightingApp();
    virtual ~LightingApp();
    virtual void InitScene();
protected:
    virtual void RenderFrame() override;
    virtual void ProcessInput() override;
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
    virtual void InitMaterials() override;
    virtual void InitModel() override;
    virtual void InitShaders() override;
    virtual void InitLights() override;
    virtual void InitMainCamera() override;
    virtual void InitFont() override;
private:
    kgl::GPUProgramSPtr             lighting_shader_;
    kgl::DirectionalLight           directional_light_;
    kgl::PointLight                 point_light_;
    kgl::SpotLight                  spot_light_;
    kgl::BasicStaticMesh*           model_ = nullptr;
    kgl::RenderStateDrawMode        draw_mode_;
    kgl::RenderStateDepth           rs_depth_;
    std::wstring                    toggle_help_on_text_;
    std::wstring                    toggle_help_off_text_;
    std::wstring                    camera_ctrl_text_;
    std::wstring                    material_ctrl_text_;
    std::vector<std::wstring>       material_name_text_;
    std::vector<kgl::Material>      materials_;         // 程序中用到材质
    uint32_t                        cur_mat_index_ = 0;
    bool                            is_help_on_ = false;
};

#endif // lighting_app_h__