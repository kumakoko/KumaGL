// 简单的纹理应用示例
#ifndef texture_app_h__
#define texture_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_font_texture.h"

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
	kgl::FontRenderer*		font_renderer_;
	kgl::Primitive*			helper_rectangle_;
	std::wstring			text_;
};

#endif // texture_app_h__