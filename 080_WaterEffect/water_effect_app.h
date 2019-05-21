/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

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
 * \file water_effect_app.h
 * \date 2019/01/14 10:39
 *
 * \author Xiong Xinke
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: 演示水面效果的工程
 *
 * \note
*/
#ifndef water_app_h__
#define water_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_basic_static_mesh.h"
#include "../klib/kgl_render_state_blend.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_draw_mode.h"
#include "../klib/kgl_frame_buffer.h"
#include "../klib/kgl_sky_box.h"

#include "../klib/kgl_wfx_loader.h"
#include "../klib/kgl_wfx_terrain.h"
#include "../klib/kgl_wfx_light.h"
#include "../klib/kgl_wfx_water_renderer.h"
#include "../klib/kgl_wfx_water_frame_buffer.h"
#include "../klib/kgl_wfx_water_tile.h"

#include "main_render_module.h"

namespace wfx = kgl::water_effect;

class WaterEffectApp : public kgl::App
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="WaterEffectApp"/> class.
    /// </summary>
    WaterEffectApp();

    /// <summary>
    /// Finalizes an instance of the <see cref="WaterEffectApp"/> class.
    /// </summary>
    ~WaterEffectApp();

    /// <summary>
    /// Initializes the scene.
    /// </summary>
    virtual void InitScene()  override;
protected:
    /// <summary>
    /// Renders the GUI.
    /// </summary>
    virtual void RenderGUI() override;

    /// <summary>
    /// Renders the scene.
    /// </summary>
    virtual void RenderScene() override;

    /// <summary>
    /// Processes the input.
    /// </summary>
    virtual void ProcessInput() override;

    /// <summary>
    /// Initializes the model.
    /// </summary>
    virtual void InitModel() override;

    /// <summary>
    /// Initializes the shaders.
    /// </summary>
    virtual void InitShaders() override;

	/// <summary>
	/// Initializes the main camera.
	/// </summary>
	virtual void InitMainCamera() override;

	/// <summary>
	/// Pres the render frame.
	/// </summary>
	virtual void PreRenderFrame() override;
private:
	/// <summary>
	/// Initializes the skybox model.
	/// </summary>
	void InitSkyboxModel();

	/// <summary>
	/// 初始化地形相关的资源
	/// </summary>
	void InitTerrain();
	
	void InitWater();

private:
	/// <summary>
	/// The draw_mode_
	/// </summary>
	kgl::RenderStateDrawMode draw_mode_;

	/// <summary>
	/// 是否以线框模式绘制场景
	/// </summary>
	bool wireframe_mode_ = false;

    /// <summary>
    /// 绘制世界吗
    /// </summary>
    bool is_draw_world_ = true;

	/// <summary>
	/// 装载图片和资源等等用到的装载器
	/// </summary>
	wfx::LoaderSPtr loader_;

	wfx::WaterFrameBuffersSPtr water_fb_;

	std::vector<wfx::Terrain> terrains_;

	std::vector<wfx::Light> lights_;

	std::vector<wfx::WaterTile> water_tiles_;

	MainRenderModule* main_render_module_ = nullptr;

	wfx::WaterRenderer* water_renderer_ = nullptr;
};


#endif // water_app_h__