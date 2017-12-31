/*!
 * \file image_effect_app.h
 * \date 2017/12/31 15:21
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: 各种图像滤镜操作
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

#ifndef image_effect_app_h__
#define image_effect_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_blend.h"

class ImageEffectApp : public kgl::App
{
public:
	ImageEffectApp();
	virtual ~ImageEffectApp();
	virtual void InitScene() override;
protected:
	virtual void RenderFrame() override;
	virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
	virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
	virtual void InitShaders() override;
	virtual void InitModel() override;
	virtual void InitFont() override;
	virtual void ProcessInput() override;
	void RenderColorToGray();
	void RenderEmboss();
	void RenderSaturation();
private:
	glm::vec2				screen_resolution_;
    kgl::GPUProgram*        color_to_gray_shader_ = nullptr;

	/// <summary>
	/// 浮雕效果相关的shader
	/// </summary>
	kgl::GPUProgram*        emboss_shader_ = nullptr;

	/// <summary>
	/// 饱和度效果相关的shader
	/// </summary>
	kgl::GPUProgram*        saturation_shader_ = nullptr;

	/// <summary>
	/// 饱和度控制系数
	/// </summary>
	float saturation_factor_ = 0.78f;

	kgl::PrimitiveSPtr		rectangle_primitive_ ;
    kgl::SourceTextureSPtr  main_texture_;					// 主画面纹理
	int                     current_effect_index_ = 0;      // 当前要渲染的效果
	int						effect_count_ = 3;

	/// <summary>
	/// The rs_blend_
	/// </summary>
	kgl::RenderStateBlend rs_blend_;
};

#endif // image_effect_app_h__