// 展示天空盒的渲染技术
#ifndef skybox_app_h__
#define skybox_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_sky_box.h"

class SkyBoxApp : public kgl::App
{
public:
	SkyBoxApp();
	virtual ~SkyBoxApp();
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
private:
	kgl::SkyBox*				sky_box_ = nullptr;
	kgl::GPUProgram*			scene_obj_shader_ = nullptr;
	kgl::PrimitiveSPtr			floor_;
	kgl::SourceTextureSPtr		floor_texture_;
};


#endif // skybox_app_h__