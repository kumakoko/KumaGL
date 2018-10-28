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
#include "render_pass_shadow_mapping.h"

RenderPassShadowMapping::RenderPassShadowMapping()
{
}

void RenderPassShadowMapping::Initialize()
{
    gpu_program_->Use();
    GetViewMatrixLocation("u_view_matrix");
    GetWorldMatrixLocation("u_world_matrix");
    GetProjectionMatrixLocation("u_projection_matrix");
    GetCameraWorldPositionLocation("u_camera_world_pos");
    GetLightWorldPositionLocation("u_light_world_pos");
    GetLightSpaceMatrixLocation("u_light_space_matrix");
    GetDiffuseTextureLocation("u_diffuse_texture");
    GetShadowMapLocation("u_shadow_map");
    GetBaisAndSampleForShadowLocation("u_bias_and_sample");
}

void RenderPassShadowMapping::GetBaisAndSampleForShadowLocation(const char* uniform_name)
{
    bias_and_sample_position_ = gpu_program_->GetUniformLocation(uniform_name);
}

void RenderPassShadowMapping::GetLightWorldPositionLocation(const char* uniform_name)
{
    location_light_world_position_ = gpu_program_->GetUniformLocation(uniform_name);
}

void RenderPassShadowMapping::GetLightSpaceMatrixLocation(const char* uniform_name)
{
    location_light_space_matrix_ = gpu_program_->GetUniformLocation(uniform_name);
}

void RenderPassShadowMapping::GetDiffuseTextureLocation(const char* uniform_name)
{
    location_diffuse_texture_ = gpu_program_->GetUniformLocation(uniform_name);
}

void RenderPassShadowMapping::GetShadowMapLocation(const char* uniform_name)
{
    location_shadow_map_ = gpu_program_->GetUniformLocation(uniform_name);
}

void RenderPassShadowMapping::SetLightWorldPosition(const glm::vec3& pos)
{
    if (-1 != location_light_world_position_)
        gpu_program_->ApplyVector3(glm::value_ptr(pos), location_light_world_position_);
}

void RenderPassShadowMapping::SetLightSpaceMatrix(const glm::mat4& mat, bool need_transpose)
{
    if (-1 != location_light_space_matrix_)
        gpu_program_->ApplyMatrix(glm::value_ptr(mat), location_light_space_matrix_, need_transpose);
}

void RenderPassShadowMapping::SetDiffuseTexture(kgl::TextureSPtr texture, GLuint slot_index)
{
    if (-1 != location_diffuse_texture_)
        gpu_program_->ApplyTexture(texture, location_diffuse_texture_,slot_index);
}

void RenderPassShadowMapping::SetShadowMap(kgl::TextureSPtr shadow_map_texture, GLuint slot_index)
{
    if (-1 != location_shadow_map_)
        gpu_program_->ApplyTexture(shadow_map_texture, location_shadow_map_, slot_index);
}

void RenderPassShadowMapping::SetBaisAndSampleForShadowLocation(bool use)
{
    if (-1 != bias_and_sample_position_)
        gpu_program_->ApplyInt(use?1:0, bias_and_sample_position_);
}

void RenderPassShadowMapping::Update(float current_time)
{

}