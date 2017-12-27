// 展示全屏模糊的渲染技术
#ifndef blur_app_h__
#define blur_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_sky_box.h"
#include "../klib/kgl_basic_static_mesh.h"
#include "../klib/kgl_light.h"

class BlurApp : public kgl::App
{
public:
	BlurApp();
	virtual ~BlurApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame() override;
	virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
	virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
	virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset) override;
	virtual void ProcessInput() override;
	virtual void InitModel() override;
	virtual void InitShader() override;
	virtual void InitMainCamera() override;
	virtual void InitLight() override;
	virtual void InitMaterial() override;
private:
	/// <summary>
	/// The model_
	/// </summary>
	kgl::BasicStaticMesh* model_ = nullptr;

	/// <summary>
	/// The model_shader_
	/// </summary>
	kgl::GPUProgramSPtr model_shader_;

	/// <summary>
	/// The rs_depth_
	/// </summary>
	kgl::RenderStateDepth rs_depth_;

	/// <summary>
	/// The directional_light_
	/// </summary>
	kgl::DirectionalLight directional_light_;

	/// <summary>
	/// The material_
	/// </summary>
	kgl::Material material_;
};


#endif // blur_app_h__