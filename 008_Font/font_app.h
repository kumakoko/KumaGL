// 简单的纹理应用示例
#ifndef texture_app_h__
#define texture_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_blend.h"

class FontApp : public kgl::App
{
public:
	FontApp();
	virtual ~FontApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame();
	void InitHelper();
private:
	kgl::GPUProgram*		helper_shader_;
	kgl::Primitive*			helper_rectangle_;
	kgl::RenderStateBlend	rs_blend_;
	std::wstring			text_;
};

#endif // texture_app_h__