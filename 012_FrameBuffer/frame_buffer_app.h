// 展示使用frame buffer的技术
#ifndef frame_buffer_app_h__
#define frame_buffer_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_rendered_texture.h"
#include "../klib/kgl_frame_buffer.h"

class FrameBufferApp : public kgl::App
{
public:
	FrameBufferApp();
	virtual ~FrameBufferApp();
	virtual void InitScene();
protected:
	virtual void PreRenderFrame() override;
	virtual void RenderFrame() override;
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
private:
	/// <summary>
	/// The scene_obj_shader_
	/// </summary>
	kgl::GPUProgram* floor_shader_ = nullptr;

	kgl::GPUProgram* wood_box_shader_ = nullptr;
	
	/// <summary>
	/// The screen_shader_
	/// </summary>
	kgl::GPUProgram* screen_shader_ = nullptr;
	
	/// <summary>
	/// 帧缓冲区对象
	/// </summary>
	kgl::FrameBuffer* frame_buffer_ = nullptr;

	/// <summary>
	/// 用来绘制箱子的图元
	/// </summary>
	kgl::PrimitiveSPtr box_;

	/// <summary>
	/// 用来绘制地板的图元
	/// </summary>
	kgl::PrimitiveSPtr floor_;

	/// <summary>
	/// 用来绘制帧缓冲区内容的图元
	/// </summary>
	kgl::PrimitiveSPtr screen_;
	
	/// <summary>
	/// The floor_texture_
	/// </summary>
	kgl::SourceTextureSPtr floor_texture_;

	/// <summary>
	/// The box_texture_
	/// </summary>
	kgl::SourceTextureSPtr box_texture_;

	/// <summary>
	/// 绘制scene object到frame buffer时使用的拣选模式
	/// </summary>
	kgl::RenderStateCullMode write_fb_cull_;

	/// <summary>
	/// 向frame buffer写入数据时的深度模式
	/// </summary>
	kgl::RenderStateDepth write_fb_depth_;

	/// <summary>
	/// 绘制frame buffer object上的数据到屏幕中的深度值
	/// </summary>
	kgl::RenderStateDepth draw_fb_to_screen_depth_;

	glm::vec3 floor_translation_;

	kgl::RenderStateCullMode cull_mode_;
	kgl::RenderStateDepth depth_test_mode_;

	kgl::SourceTextureSPtr texture_sun_;
	kgl::PrimitiveSPtr sun_;
	kgl::GPUProgramSPtr scene_shader_;

	kgl::RenderStateCullMode draw_fb_to_scr_cull_;
	kgl::RenderStateDepth draw_fb_to_scr_depth_;
};


#endif // frame_buffer_app_h__