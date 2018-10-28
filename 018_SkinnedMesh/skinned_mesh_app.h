/*!
 * \file skinned_mesh_app.h
 * \date 2017/12/31 15:27
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 蒙皮骨骼动画模型
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
#ifndef skinned_mesh_app_h__
#define skinned_mesh_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_basic_skinned_mesh.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_render_state_blend.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_light.h"

class SkinnedMeshApp : public kgl::App
{ 
private:
    static const int MAX_BONES_ACCOUNT = 100;
    static const int MAX_POINT_LIGHTS = 2;
    static const int MAX_SPOT_LIGHTS = 2;
public:
    SkinnedMeshApp();
    virtual ~SkinnedMeshApp();
    virtual void InitScene()  override;
protected:
    virtual void RenderFrame() override;
    virtual void ProcessInput() override;
    virtual void InitModel() override;
    virtual void InitShaders() override;
    virtual void InitFont() override;
    virtual void InitMainCamera() override;
    virtual void InitLights() override;
    void RenderText();
private:
    kgl::GPUProgramSPtr         model_shader_;
    kgl::RenderStateDepth       rs_depth_;
    kgl::RenderStateBlend       rs_blend_;
    kgl::DirectionalLight       m_directionalLight;
    kgl::SpotLight              spot_lights_[MAX_SPOT_LIGHTS];
    kgl::PointLight             point_lights_[MAX_POINT_LIGHTS];
    std::vector<std::string>    bone_uniform_name_;
    std::vector<std::string>    point_light_uniform_name_;
    std::vector<std::string>    spot_light_uniform_name_;

    int point_lights_num_ = 2;
    int spot_lights_num_ = 2;
    float specular_intensity_ = 1.0f;
    float specular_power = 1.0f;
};

#endif // assimp_app_h__