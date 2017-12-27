// 简单的纹理应用示例
#ifndef lighting_app_h__
#define lighting_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_material.h"
#include "../klib/kgl_light.h"
#include "../klib/kgl_basic_static_mesh.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_render_state_draw_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_render_state_blend.h"

class LightingApp : public kgl::App
{
public:
	LightingApp();
	virtual ~LightingApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame() override;
	virtual void ProcessInput() override;
	virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
	virtual void InitMaterial() override;
	virtual void InitModel() override;
	virtual void InitShader() override;
	virtual void InitLight() override;
	virtual void InitMainCamera() override;
	virtual void InitFont() override;
private:
	kgl::GPUProgramSPtr				lighting_shader_;
	kgl::DirectionalLight			directional_light_;
	kgl::PointLight					point_light_;
	kgl::SpotLight					spot_light_;
	kgl::BasicStaticMesh*			model_ = nullptr;
	kgl::RenderStateDrawMode		draw_mode_;
	kgl::RenderStateDepth			rs_depth_;
	std::wstring				    toggle_help_on_text_;
	std::wstring				    toggle_help_off_text_;
	std::wstring					camera_ctrl_text_;
	std::wstring					material_ctrl_text_;
	std::vector<std::wstring>		material_name_text_;
	std::vector<kgl::Material>		materials_;			// 程序中用到材质
	uint32_t						cur_mat_index_ = 0;
	bool							is_help_on_ = false;
};

#endif // lighting_app_h__