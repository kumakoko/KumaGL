#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "empty_render_pass.h"

EmptyRenderPass::EmptyRenderPass()
{
}

EmptyRenderPass::~EmptyRenderPass()
{
	KGL_SAFE_DELETE(program_);
}

void EmptyRenderPass::Enable()
{
	program_->Use();
}

bool EmptyRenderPass::Init()
{
	const char* vs_file = "resources/shader/020_dr_empty_pass_vs.glsl";
	const char* fs_file = "resources/shader/020_dr_empty_pass_fs.glsl";
	program_ = new kgl::GPUProgram;
	program_->CreateFromFile(vs_file, fs_file, nullptr);
	wvp_matrix_loc_ = program_->GetUniformLocation("gWVP");
	return true;
}

void EmptyRenderPass::SetWVPMatrix(const glm::mat4& wvp_matrix)
{
	program_->ApplyMatrix(glm::value_ptr(wvp_matrix), wvp_matrix_loc_);
}