// 简单的纹理应用示例
#ifndef transform_app_h__
#define transform_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"

class TransformApp : public kgl::App
{
public:
	TransformApp();
	virtual ~TransformApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame() override;
//	virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
//	virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
//	virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset) override;
	virtual void ProcessInput() override;
	virtual void InitMaterials() override;
	virtual void InitModel() override;
	virtual void InitShaders() override;
	virtual void InitLights() override;
	virtual void InitMainCamera() override;
	virtual void InitFont() override;
	void RenderText();
private:
	kgl::GPUProgram*		object_shader_ = nullptr;
	kgl::SourceTextureSPtr	texture_sun_;
	kgl::SourceTextureSPtr	texture_earth_;
	kgl::SourceTextureSPtr	texture_moon_;
	kgl::PrimitiveSPtr sun_;
	kgl::PrimitiveSPtr earth_;
	kgl::PrimitiveSPtr moon_;
	kgl::RenderStateCullMode cull_mode_;
	kgl::RenderStateDepth depth_test_mode_;
};

#endif // transform_app_h__