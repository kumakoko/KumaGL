// 简单的纹理应用示例
#ifndef assimp_app_h__
#define assimp_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_model.h"
#include "../klib/kgl_font_renderer.h"

class AssimpApp : public kgl::App
{
public:
	AssimpApp();
	virtual ~AssimpApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame();
	virtual void ProcessInput();
	void InitModel();
	void InitShader();
	void InitFont();
	void RenderText();
private:
	kgl::GPUProgramSPtr		model_shader_;
	kgl::Model*				model_;
};

#endif // assimp_app_h__