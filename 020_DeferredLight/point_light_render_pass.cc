#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "point_light_render_pass.h"

PointLightRenderPass::PointLightRenderPass()
{
}

PointLightRenderPass::~PointLightRenderPass()
{
	KGL_SAFE_DELETE(lighting_shader_);
}

bool PointLightRenderPass::Init()
{
	const char* vs_file = "resources/shader/020_dr_lighting_pass_vs.glsl";
	const char* fs_file = "resources/shader/020_dr_lighting_pass_point_fs.glsl";

	lighting_shader_ = new kgl::GPUProgram;
	lighting_shader_->CreateFromFile(vs_file, fs_file, nullptr);

	point_light_loc_ = lighting_shader_->GetPointLightUniformLocation("u_point_light");
	position_in_map_loc_ = lighting_shader_->GetUniformLocation("u_position_in_map");
	color_in_map_loc_ = lighting_shader_->GetUniformLocation("u_color_in_map");
	normal_in_map_loc_ = lighting_shader_->GetUniformLocation("u_normal_in_map");
	view_pos_in_world_loc_ = lighting_shader_->GetUniformLocation("u_view_pos_in_world");
	screen_size_loc_ = lighting_shader_->GetUniformLocation("u_screen_size");
//	world_matrix_loc_ = lighting_shader_->GetUniformLocation("u_screen_size");
	return true;
}

void PointLightRenderPass::Enable()
{
	lighting_shader_->Use();
}

void PointLightRenderPass::SetPointLight(const kgl::PointLight* point_light)
{
	lighting_shader_->ApplyPointLight(point_light, point_light_loc_);
}

void PointLightRenderPass::SetWVPMatrix(const glm::mat4& wvp_matrix)
{
	lighting_shader_->ApplyMatrix(glm::value_ptr(wvp_matrix),wvp_matrix_loc_);
}

void PointLightRenderPass::SetPositionTextureUnit(GLuint texture_unit)
{
	lighting_shader_->ApplyTexture(position_in_map_loc_, texture_unit);
}

void PointLightRenderPass::SetColorTextureUnit(GLuint texture_unit)
{
	lighting_shader_->ApplyTexture(color_in_map_loc_, texture_unit);
}

void PointLightRenderPass::SetNormalTextureUnit(GLuint texture_unit)
{
	lighting_shader_->ApplyTexture(normal_in_map_loc_, texture_unit);
}

void PointLightRenderPass::SetViewInWorldPos(const glm::vec3& view_in_world_pos)
{
	lighting_shader_->ApplyVector2(glm::value_ptr(view_in_world_pos), view_pos_in_world_loc_);
}

void PointLightRenderPass::SetScreenSize(uint32_t Width, uint32_t Height)
{
	lighting_shader_->ApplyVector2(glm::value_ptr(glm::vec2(Width, Height)), screen_size_loc_);
}

void PointLightRenderPass::SetWorldMatrix(const glm::vec4& world_matrix)
{
	lighting_shader_->ApplyMatrix(glm::value_ptr(world_matrix), world_matrix_loc_);
}