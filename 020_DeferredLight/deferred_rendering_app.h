// 延迟着色的案例
#ifndef deferred_rendering_app_h__
#define deferred_rendering_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_material.h"
#include "../klib/kgl_light.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_render_state_draw_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_render_state_blend.h"
#include "../klib/kgl_gbuffer.h"
#include "../klib/kgl_basic_static_mesh.h"

#include "geometry_render_pass.h"
#include "point_light_render_pass.h"
#include "empty_render_pass.h"

class DeferredRenderingApp : public kgl::App
{
private:
	static const uint32_t POINT_LIGHT_COUNT = 3;
public:
	DeferredRenderingApp();
	virtual ~DeferredRenderingApp();
	virtual void InitScene();
protected:
	virtual void PreRenderFrame() override;
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
	/// <summary>
	/// 初始化延迟渲染中的渲染几何信息的渲染阶段
	/// </summary>
	void InitGeometryRenderPass();

	/// <summary>
	/// 执行延迟渲染中的渲染几何信息的渲染阶段
	/// </summary>
	void ExecuteGeometryRenderPass();

	/// <summary>
	/// 始化延迟渲染中的执行点光源的渲染阶段
	/// </summary>
	void InitPointLightRenderPass();

	/// <summary>
	/// Executes the point light render pass.
	/// </summary>
	/// <param name="point_light">The point_light.</param>
	/// <param name="world_matrix">The world_matrix.</param>
	/// <param name="view_matrix">The view_matrix.</param>
	/// <param name="projection_matrix">The projection_matrix.</param>
	void ExecutePointLightRenderPass(const kgl::PointLight* point_light, const glm::mat4& world_matrix, const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

	/// <summary>
	/// Initializes the empty render pass.
	/// </summary>
	void InitEmptyRenderPass();

	/// <summary>
	/// Executes the point light stencil pass.
	/// </summary>
	/// <param name="point_light">The point_light.</param>
	/// <param name="world_matrix">The world_matrix.</param>
	/// <param name="view_matrix">The view_matrix.</param>
	/// <param name="projection_matrix">The projection_matrix.</param>
	void ExecutePointLightStencilPass(const kgl::PointLight* point_light, const glm::mat4& world_matrix, const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

	/// <summary>
	/// Executes the final render pass.
	/// </summary>
	void ExecuteFinalRenderPass();

	float CalcPointLightBoundSphere(const kgl::PointLight& point_light);

	/// <summary>
	/// Initializes the box positions.
	/// </summary>
	void InitBoxPositions();
private:
	/// <summary>
	/// 用来进行延迟渲染操作用的G-buffer
	/// </summary>
	kgl::GBuffer* gbuffer_ = nullptr;
	
	kgl::RenderStateDrawMode		draw_mode_;
	kgl::RenderStateDepth			rs_depth_;

	/// <summary>
	/// 延迟渲染中的渲染几何信息的渲染阶段
	/// </summary>
	GeometryRenderPass* geometry_render_pass_ = nullptr;

	/// <summary>
	/// 延迟渲染中的点光源光照计算的渲染阶段
	/// </summary>
	PointLightRenderPass* point_light_render_pass_ = nullptr;

	/// <summary>
	/// The empty_render_pass_
	/// </summary>
	EmptyRenderPass* empty_render_pass_ = nullptr;

	/// <summary>
	/// 用于多次渲染的模型位置点
	/// </summary>
	std::vector<glm::vec3> box_positions_;

	std::wstring toggle_help_on_text_;
	std::wstring toggle_help_off_text_;
	std::wstring camera_ctrl_text_;
	std::wstring material_ctrl_text_;
	std::vector<std::wstring> material_name_text_;
	std::vector<kgl::Material> materials_;			// 程序中用到材质
	uint32_t cur_mat_index_ = 0;
	bool is_help_on_ = false;
	kgl::PointLight point_lights_[POINT_LIGHT_COUNT];
	kgl::BasicStaticMesh* m_box = nullptr;
	kgl::BasicStaticMesh* m_bsphere = nullptr;
	kgl::BasicStaticMesh* m_quad = nullptr;
};

#endif // deferred_rendering_app_h__