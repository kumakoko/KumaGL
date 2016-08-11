// 简单的纹理应用示例
#ifndef lighting_app_h__
#define lighting_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_material.h"
#include "../klib/kgl_light.h"

class LightingApp : public kgl::App
{
public:
	LightingApp();
	virtual ~LightingApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame();
	void InitMaterial();
private:
	kgl::GPUProgram*		gpu_program_;
	kgl::GPUProgram*		lighting_shader_;
	kgl::PrimitiveSPtr		cube_;
	kgl::Material			golden_material_;
	kgl::Light				light_;
};

#endif // lighting_app_h__