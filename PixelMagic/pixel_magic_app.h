// 简单的纹理应用示例
#ifndef texture_app_h__
#define texture_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"

class PixelMagicApp : public kgl::App
{
public:
	PixelMagicApp();
	virtual ~PixelMagicApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame();
	virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode);
	virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos);
	void RenderBeatHeart();
	void RenderColorfulRing();
	void RenderMobius();
	void RenderSun();
	void RenderSimpleFractal();
private:
	kgl::GPUProgram*		colorful_ring_shader_;
	kgl::GPUProgram*		heart_beat_shader_;
	kgl::GPUProgram*		mobius_shader_;
	kgl::GPUProgram*		sun_shader_;
	kgl::GPUProgram*		simple_fractal_shader_;
	kgl::Primitive*			rectangle_primitive_;
	kgl::SourceTextureSPtr	texture_1_;
	kgl::SourceTextureSPtr	texture_2_;
	kgl::SourceTextureSPtr	texture_rock_;
	glm::vec2				screen_resolution_;
	glm::vec2				mouse_input_pos_;
	int						current_effect_index_;		// 当前要渲染的效果
	int						effect_count_;				// 待渲染的效果的总数
};

#endif // texture_app_h__