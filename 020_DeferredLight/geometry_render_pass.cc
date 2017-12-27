#include "geometry_render_pass.h"
#include "../klib/kgl_lib_pch.h"


/*
Re: glGetUniformLocation returns -1 for exisitng unifo
It removes unused uniforms, and since you don't use "topo" or "norm" in the shader, 
they aren't included and therefore don't have a location.
If the uniform is determined to have no effect on the final result, then they are often removed.
*/
void GeometryRenderPass::Init()
{
	gpu_program_ = std::make_shared<kgl::GPUProgram>();
	const GLchar* vs_file = "resources/shader/020_dr_geometry_pass_vs.glsl";
	const GLchar* fs_file = "resources/shader/020_dr_geometry_pass_fs.glsl";
	gpu_program_->CreateFromFile(vs_file, fs_file, nullptr);
	color_texture_unit_loc_ = gpu_program_->GetUniformLocation("u_color_map");
	wvp_matrix_loc_ = gpu_program_->GetUniformLocation("u_wvp_matrix");
	world_matrix_loc_ = gpu_program_->GetUniformLocation("u_world_matrix");
}

void GeometryRenderPass::Enable()
{
	gpu_program_->Use();
}

void GeometryRenderPass::SetWVP(const glm::mat4& WVP)
{
	gpu_program_->ApplyMatrix(glm::value_ptr(WVP), wvp_matrix_loc_);
}

void GeometryRenderPass::SetWorldMatrix(const glm::mat4& WorldInverse)
{
	gpu_program_->ApplyMatrix(glm::value_ptr(WorldInverse), world_matrix_loc_);
}

void GeometryRenderPass::SetColorTextureUnit(unsigned int TextureUnit)
{
	gpu_program_->ApplyTexture(color_texture_unit_loc_, TextureUnit);
}