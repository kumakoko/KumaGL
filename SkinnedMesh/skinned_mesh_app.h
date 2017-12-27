// 蒙皮骨骼动画模型
#ifndef skinned_mesh_app_h__
#define skinned_mesh_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_basic_skinned_mesh.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_render_state_blend.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_light.h"

class SkinnedMeshApp : public kgl::App
{ 
private:
	static const int MAX_BONES_ACCOUNT = 100;
	static const int MAX_POINT_LIGHTS = 2;
	static const int MAX_SPOT_LIGHTS = 2;
public:
	SkinnedMeshApp();
	virtual ~SkinnedMeshApp();
	virtual void InitScene()  override;
protected:
	virtual void RenderFrame() override;
	virtual void ProcessInput() override;
	virtual void InitModel() override;
	virtual void InitShader() override;
	virtual void InitFont() override;
	virtual void InitMainCamera() override;
	virtual void InitLight() override;
	void RenderText();
private:
	kgl::GPUProgramSPtr			model_shader_;
	kgl::RenderStateDepth		rs_depth_;
	kgl::RenderStateBlend		rs_blend_;
	kgl::DirectionalLight		m_directionalLight;
	kgl::SpotLight				spot_lights_[MAX_SPOT_LIGHTS];
	kgl::PointLight				point_lights_[MAX_POINT_LIGHTS];
	std::vector<std::string>    bone_uniform_name_;
	std::vector<std::string>	point_light_uniform_name_;
	std::vector<std::string>    spot_light_uniform_name_;

	int point_lights_num_ = 2;
	int spot_lights_num_ = 2;
	float specular_intensity_ = 1.0f;
	float specular_power = 1.0f;
};

#endif // assimp_app_h__