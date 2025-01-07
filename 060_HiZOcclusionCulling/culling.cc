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
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"
#include "../klib/kgl_glm_ext.h"

#include "culling.h"

CullingInterface::~CullingInterface()
{
}

void CullingInterface::ComputeFrustumFromViewProjection(glm::vec4 *planes, const glm::mat4 &view_projection)
{
    glm::mat4 inv_view_proj = glm::inverse(view_projection);

    // Get world-space coordinates for clip-space bounds.
    glm::vec4 lbn = inv_view_proj * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
    glm::vec4 ltn = inv_view_proj * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
    glm::vec4 lbf = inv_view_proj * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
    glm::vec4 rbn = inv_view_proj * glm::vec4( 1.0f, -1.0f, -1.0f, 1.0f);
    glm::vec4 rtn = inv_view_proj * glm::vec4( 1.0f, 1, -1.0f, 1.0f);
    glm::vec4 rbf = inv_view_proj * glm::vec4( 1.0f, -1.0f, 1.0f, 1.0f);
    glm::vec4 rtf = inv_view_proj * glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f);

    glm::vec3 lbn_pos = kgl::vec_project(lbn);
    glm::vec3 ltn_pos = kgl::vec_project(ltn);
    glm::vec3 lbf_pos = kgl::vec_project(lbf);
    glm::vec3 rbn_pos = kgl::vec_project(rbn);
    glm::vec3 rtn_pos = kgl::vec_project(rtn);
    glm::vec3 rbf_pos = kgl::vec_project(rbf);
    glm::vec3 rtf_pos = kgl::vec_project(rtf);

    // Get plane equations for all sides of frustum.
    glm::vec3 left_normal = glm::normalize(glm::cross(lbf_pos - lbn_pos, ltn_pos - lbn_pos));
    glm::vec3 right_normal = glm::normalize(glm::cross(rtn_pos - rbn_pos, rbf_pos - rbn_pos));
    glm::vec3 top_normal = glm::normalize(glm::cross(ltn_pos - rtn_pos, rtf_pos - rtn_pos));
    glm::vec3 bottom_normal = glm::normalize(glm::cross(rbf_pos - rbn_pos, lbn_pos - rbn_pos));
    glm::vec3 near_normal = glm::normalize(glm::cross(ltn_pos - lbn_pos, rbn_pos - lbn_pos));
    glm::vec3 far_normal = glm::normalize(glm::cross(rtf_pos - rbf_pos, lbf_pos - rbf_pos));

    planes[0] = glm::vec4(left_normal, -glm::dot(left_normal, lbn_pos)); // Left
    planes[1] = glm::vec4(right_normal, -glm::dot(right_normal, rbn_pos)); // Right
    planes[2] = glm::vec4(near_normal, -glm::dot(near_normal, lbn_pos)); // Near
    planes[3] = glm::vec4(far_normal, -glm::dot(far_normal, lbf_pos)); // Far
    planes[4] = glm::vec4(top_normal, -glm::dot(top_normal, ltn_pos)); // Top
    planes[5] = glm::vec4(bottom_normal, -glm::dot(bottom_normal, lbn_pos)); // Bottom
}

uint32_t CullingInterface::GetLodsCount() const 
{
    return SPHERE_LODS; 
}

GLuint CullingInterface::GetDepthTexture() const
{
    return 0; 
}