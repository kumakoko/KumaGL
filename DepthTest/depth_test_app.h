// 演示使用深度测试的效果
#ifndef depth_test_app_h__
#define depth_test_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_rendered_texture.h"
#include "../klib/kgl_render_state_blend.h"
#include "../klib/kgl_frame_buffer.h"

class DepthTestApp : public kgl::App
{
public:
	DepthTestApp();
	virtual ~DepthTestApp();
	virtual void InitScene();
protected:
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
	void RenderText();
private:
	/// <summary>
	/// 用来绘制两个面片的shader
	/// </summary>
	kgl::GPUProgram* square_shader_ = nullptr;

	/// <summary>
	///面片1，红色
	/// </summary>
	kgl::PrimitiveSPtr coordinate_square_;

	/// <summary>
	/// 面片2，黄色
	/// </summary>
	kgl::PrimitiveSPtr wood_box_square_;

	kgl::SourceTextureSPtr texture_coordinate_;

	kgl::SourceTextureSPtr texture_wood_box_;

	/// <summary>
	/// 绘制两个面片时使用的拣选模式
	/// </summary>
	kgl::RenderStateCullMode draw_square_cull_mode_;

	/// <summary>
	/// 绘制两个面片时使用的深度模式
	/// </summary>
	kgl::RenderStateDepth draw_square_depth_mode_;

	kgl::RenderStateBlend not_blend_;
	/// <summary>
	/// 红色面片在世界坐标系下的平移值
	/// </summary>
	glm::vec3 yellow_square_translation_;

	GLuint draw_square_depth_func_ = GL_LESS;
};


#endif // depth_test_app_h__