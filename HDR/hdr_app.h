// 展示使用HDR的技术
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

struct HDRLightStruct 
{
	glm::vec3 Position;
	glm::vec3 Color;
};

class HDRApp : public kgl::App
{
public:
	HDRApp();
	virtual ~HDRApp();
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
};


#endif // frame_buffer_app_h__