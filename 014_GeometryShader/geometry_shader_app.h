// 展示几何着色器的一些相关操作
#ifndef geometry_shader_app_h__
#define geometry_shader_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_basic_static_mesh.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"

class GeometryShaderApp : public kgl::App
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="GeometryShaderApp"/> class.
	/// </summary>
	GeometryShaderApp();

	/// <summary>
	/// Finalizes an instance of the <see cref="GeometryShaderApp"/> class.
	/// </summary>
	virtual ~GeometryShaderApp();

protected:
	/// <summary>
	/// Pres the render frame.
	/// </summary>
	virtual void PreRenderFrame() override;

	/// <summary>
	/// Initializes the model.
	/// </summary>
	virtual void InitModel() override;

	/// <summary>
	/// Initializes the shader.
	/// </summary>
	virtual void InitShader() override;

	/// <summary>
	/// Initializes the main camera.
	/// </summary>
	virtual void InitMainCamera() override;

	/// <summary>
	/// Initializes the font.
	/// </summary>
	virtual void InitFont() override;

	/// <summary>
	/// Renders the frame.
	/// </summary>
	virtual void RenderFrame() override;

	/// <summary>
	/// Called when [key action].
	/// </summary>
	/// <param name="window">The window.</param>
	/// <param name="key">The key.</param>
	/// <param name="scancode">The scancode.</param>
	/// <param name="action">The action.</param>
	/// <param name="mode">The mode.</param>
	virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;

	/// <summary>
	/// Called when [mouse action].
	/// </summary>
	/// <param name="window">The window.</param>
	/// <param name="xpos">The xpos.</param>
	/// <param name="ypos">The ypos.</param>
	virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;

	/// <summary>
	/// Called when [scroll action].
	/// </summary>
	/// <param name="window">The window.</param>
	/// <param name="xoffset">The xoffset.</param>
	/// <param name="yoffset">The yoffset.</param>
	virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset) override;

	/// <summary>
	/// 处理外设输入的方法
	/// </summary>
	virtual void ProcessInput() override;
private:
	/// <summary>
	/// 绘制彩色多边形
	/// </summary>
	void RenderColoredPolygon();

	/// <summary>
	/// 绘制爆炸的模型
	/// </summary>
	void RenderExplodeModel();

	/// <summary>
	/// 绘制帮助文本
	/// </summary>
	/// <param name="view_pos">摄像机的位置</param>
	void DrawHelpText(const glm::vec3& view_pos);
private:
	/// <summary>
	/// 绘制彩色多边形用的shader
	/// </summary>
	kgl::GPUProgramSPtr		colored_polygon_shader_;

	/// <summary>
	/// 绘制爆炸效果的shader
	/// </summary>
	kgl::GPUProgramSPtr		explode_effect_shader_;

	/// <summary>
	/// 启用深度buffer的render state
	/// </summary>
	kgl::RenderStateDepth	rs_depth_;
	
	/// <summary>
	/// 绘制房子用的图元
	/// </summary>
	kgl::Primitive*			colored_polygon_primitive_ = nullptr;

	/// <summary>
	/// 演示爆炸效果的模型
	/// </summary>
	kgl::BasicStaticMesh* model_ = nullptr;

	/// <summary>
	/// 当前待渲染的内容的编号
	/// </summary>
	int						current_render_ = 1;

	/// <summary>
	/// The toggle_help_on_text_
	/// </summary>
	std::wstring            toggle_help_on_text_;

	/// <summary>
	/// The toggle_help_off_text_
	/// </summary>
	std::wstring            toggle_help_off_text_;

	/// <summary>
	/// The camera_ctrl_text_
	/// </summary>
	std::wstring            camera_ctrl_text_;

	/// <summary>
	/// The material_ctrl_text_
	/// </summary>
	std::wstring            material_ctrl_text_;

	/// <summary>
	/// 是否显示帮助文档
	/// </summary>
	bool					is_help_on_ = true;
};


#endif // triangle_app_h__