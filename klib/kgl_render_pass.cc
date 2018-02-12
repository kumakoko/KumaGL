/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#include "kgl_lib_pch.h"
#include "kgl_render_pass.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_defines.h"
#include "kgl_image_file_reader.h"

namespace kgl
{
	RenderPass::RenderPass()
    {
	}

	RenderPass::~RenderPass()
    {
		camera_.reset();
		gpu_program_.reset();
    }

	void RenderPass::AttachCamera(CameraSPtr camera)
	{
		camera_ = camera;
	}

	void RenderPass::AttachGPUProgram(GPUProgramSPtr program)
	{
		gpu_program_ = program;
	}

	void RenderPass::CreateAndAttachGPUProgram(const GLchar* vs_file, const GLchar* fs_file, const GLchar* gs_file)
	{
		gpu_program_.reset();
		gpu_program_ = std::make_shared<GPUProgram>();
		gpu_program_->CreateFromFile(vs_file, fs_file, gs_file);
	}

	void RenderPass::Initialize()
	{

	}

	void RenderPass::SetWorldMatrix(const glm::mat4& world_matrix, bool need_transpose)
	{
		if (-1 != location_world_matrix_)
			gpu_program_->ApplyMatrix(glm::value_ptr(world_matrix), location_world_matrix_, need_transpose);
	}

	void RenderPass::SetViewMatrix(const glm::mat4& view_matrix, bool need_transpose)
	{
		if (-1 != location_view_matrix_)
			gpu_program_->ApplyMatrix(glm::value_ptr(view_matrix), location_view_matrix_, need_transpose);
	}

	void RenderPass::SetProjectionMatrix(const glm::mat4& projection_matrix, bool need_transpose)
	{
		if (-1 != location_projection_matrix_)
			gpu_program_->ApplyMatrix(glm::value_ptr(projection_matrix), location_projection_matrix_, need_transpose);
	}

	void RenderPass::SetCameraWorldPosition(const glm::vec3& world_position)
	{
		if (-1 != location_camera_world_position_)
			gpu_program_->ApplyVector3(glm::value_ptr(world_position), location_camera_world_position_);
	}

	void RenderPass::GetWorldMatrixLocation(const char* uniform_name)
	{
		location_world_matrix_ = gpu_program_->GetUniformLocation(uniform_name);
	}

	void RenderPass::GetViewMatrixLocation(const char* uniform_name)
	{
		location_view_matrix_ = gpu_program_->GetUniformLocation(uniform_name);
	}
	
	void RenderPass::GetProjectionMatrixLocation(const char* uniform_name)
	{
		location_projection_matrix_ = gpu_program_->GetUniformLocation(uniform_name);
	}

	void RenderPass::GetCameraWorldPositionLocation(const char* uniform_name)
	{
		location_camera_world_position_ = gpu_program_->GetUniformLocation(uniform_name);
	}

	void RenderPass::Update(float current_time)
	{

	}

	void RenderPass::PrepareRendering()
	{
		gpu_program_->Use();
	}
}
