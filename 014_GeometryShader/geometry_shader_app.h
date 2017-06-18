// 展示几何着色器的一些相关操作
#ifndef geometry_shader_app_h__
#define geometry_shader_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_static_model.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"

class GeometryShaderApp : public kgl::App
{
public:
	GeometryShaderApp();
	virtual ~GeometryShaderApp();
	virtual void InitScene() override;
protected:
	virtual void InitModel()  override;
	virtual void RenderFrame() override;
	virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
	virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
	virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset) override;
	virtual void ProcessInput();
private:
	kgl::StaticModel* model_ = nullptr;
	kgl::GPUProgramSPtr			normal_proc_shader_; // 处理法线的着色器
	kgl::RenderStateDepth		rs_depth_;
};


#endif // triangle_app_h__