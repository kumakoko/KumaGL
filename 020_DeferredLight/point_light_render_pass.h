/*!
 * \file point_light_render_pass.h
 * \date 2017/12/22 14:49
 *
 * \author soft_koko
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef point_light_render_pass_h__
#define point_light_render_pass_h__

#include "../klib/kgl_light.h"
#include "../klib/kgl_gpu_program.h"

class PointLightRenderPass
{
public:
	PointLightRenderPass();
	~PointLightRenderPass();

	/// <summary>
	/// Initializes this instance.
	/// </summary>
	/// <returns>bool.</returns>
	bool Init();

	/// <summary>
	/// Enables this instance.
	/// </summary>
	void Enable();
	
	/// <summary>
	/// Sets the point light.
	/// </summary>
	/// <param name="point_light">The point_light.</param>
	void SetPointLight(const kgl::PointLight* point_light);

	/// <summary>
	/// Sets the WVP matrix.
	/// </summary>
	/// <param name="wvp_matrix">The wvp_matrix.</param>
	void SetWVPMatrix(const glm::mat4& wvp_matrix);

	/// <summary>
	/// Sets the position texture unit.
	/// </summary>
	/// <param name="texture_unit">The texture_unit.</param>
	void SetPositionTextureUnit(GLuint texture_unit);

	/// <summary>
	/// Sets the color texture unit.
	/// </summary>
	/// <param name="texture_unit">The texture_unit.</param>
	void SetColorTextureUnit(GLuint texture_unit);

	/// <summary>
	/// Sets the normal texture unit.
	/// </summary>
	/// <param name="texture_unit">The texture_unit.</param>
	void SetNormalTextureUnit(GLuint texture_unit);

	/// <summary>
	/// Sets the view in world position.
	/// </summary>
	/// <param name="view_in_world_pos">The view_in_world_pos.</param>
	void SetViewInWorldPos(const glm::vec3& view_in_world_pos);

	/// <summary>
	/// Sets the size of the screen.
	/// </summary>
	/// <param name="Width">The width.</param>
	/// <param name="Height">The height.</param>
	void SetScreenSize(uint32_t Width, uint32_t Height);

	/// <summary>
	/// Sets the world matrix.
	/// </summary>
	/// <param name="world_matrix">The world_matrix.</param>
	void SetWorldMatrix(const glm::vec4& world_matrix);

	/// <summary>
	/// The lighting_shader_
	/// </summary>
	kgl::GPUProgram* lighting_shader_ = nullptr;

	/// <summary>
	/// shader中的uniform变量 u_point_light的location值
	/// </summary>
	kgl::PointLightUniformLocation	point_light_loc_;

	/// <summary>
	/// shader中的uniform变量 u_position_in_map的location值
	/// </summary>
	GLint position_in_map_loc_ = -1;

	/// <summary>
	/// shader中的uniform变量 u_position_in_map的location值
	/// </summary>
	GLint color_in_map_loc_ = -1;

	/// <summary>
	/// shader中的uniform变量 u_normal_in_map的location值
	/// </summary>
	GLint normal_in_map_loc_ = -1;

	/// <summary>
	/// shader中的uniform变量 u_view_pos_in_world的location值
	/// </summary>
	GLint view_pos_in_world_loc_ = -1;

	/// <summary>
	/// shader中的uniform变量 u_screen_size的location值
	/// </summary>
	GLint screen_size_loc_ = -1;

	/// <summary>
	/// shader中的uniform变量 u_wvp_matrix的location值
	/// </summary>
	GLint wvp_matrix_loc_ = -1;

	/// <summary>
	/// The world_matrix_loc_
	/// </summary>
	GLint world_matrix_loc_ = -1;
};

#endif // point_light_render_pass_h__