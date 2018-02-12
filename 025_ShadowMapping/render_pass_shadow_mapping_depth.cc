/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

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
// 执行shadow mapping操作的render pass
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_rectangle.h"
#include "render_pass_shadow_mapping_depth.h"

RenderPassShadowMappingDepth::RenderPassShadowMappingDepth(GLint wnd_width, GLint wnd_height):
window_width_(wnd_width), window_height_(wnd_height)
{
	lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
}

void RenderPassShadowMappingDepth::Initialize()
{
	gpu_program_->Use();
	GetWorldMatrixLocation("u_world_matrix");
	GetLightSpaceMatrixLocation("u_light_space_matrix");
}

void RenderPassShadowMappingDepth::GetLightSpaceMatrixLocation(const char* uniform_name)
{
	location_light_space_matrix_ = gpu_program_->GetUniformLocation(uniform_name);
}

void RenderPassShadowMappingDepth::SetLightSpaceMatrix()
{
	if (-1 != location_light_space_matrix_)
		gpu_program_->ApplyMatrix(glm::value_ptr(lightSpaceMatrix), location_light_space_matrix_, false);
}

void RenderPassShadowMappingDepth::BuildLightSpaceMatrix()
{
	float near_plane = 1.0f, far_plane = 7.5f;
	
	//lightProjection = glm::perspective(glm::radians(45.0f), 
	//(GLfloat)1024 / (GLfloat)1024, near_plane, far_plane); 

	// 把光源看做是一个摄像机，以光源为坐标系远点，构建一个view，projection空间，把场景中的物体
	// 变换到光源空间中去。
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
}

void RenderPassShadowMappingDepth::Update(float current_time)
{
	this->BuildLightSpaceMatrix();
}
