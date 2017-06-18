// 展示一个简单的三角形
#ifndef triangle_app_h__
#define triangle_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"

class FrameBufferApp : public kgl::App
{
public:
	FrameBufferApp();
	virtual ~FrameBufferApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame();
	virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode);
	virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos);
	virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset);
	virtual void ProcessInput();
private:
	kgl::GPUProgram*			scene_obj_shader_;
	kgl::GPUProgram*			screen_shader_;
	kgl::PrimitiveSPtr			box_;
	kgl::PrimitiveSPtr			floor_;
	kgl::PrimitiveSPtr			screen_;
	kgl::SourceTextureSPtr		floor_texture_;
	kgl::SourceTextureSPtr		box_texture_;
	kgl::RenderStateCullMode	rs_cull_mode_;
	kgl::RenderStateDepth		rs_depth_;
};


#endif // triangle_app_h__