// 各种图像操作
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
	virtual void InitShader() override;
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