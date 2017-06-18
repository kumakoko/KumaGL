// 简单的纹理应用示例
#ifndef texture_app_h__
#define texture_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"

class TextureApp : public kgl::App
{
public:
	TextureApp();
	virtual ~TextureApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame();
private:
	kgl::GPUProgram*		gpu_program_;
	kgl::Primitive*			rectangle_primitive_;
	kgl::SourceTextureSPtr	texture_1_;
	kgl::SourceTextureSPtr	texture_2_;
};

#endif // texture_app_h__