/* Copyright (c) 2014-2017, ARM Limited and Contributors
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_string_convertor.h"
#include "hiz_culling.h"

using namespace std;

#define GROUP_SIZE_AABB 64

HiZCulling::HiZCulling()
{
    culling_program = common_compile_compute_shader_from_file("resources/shader/060_hiz_cull_cs.glsl"); // hiz_cull.cs
    init();
}

HiZCulling::HiZCulling(const char* program)
{
    culling_program = common_compile_compute_shader_from_file(program);
    init();
}

void HiZCulling::init()
{
    depth_render_program_ = common_compile_shader_from_file("resources/shader/060_depth_vs.glsl", "resources/shader/060_depth_fs.glsl");

    depth_mip_program_ = common_compile_shader_from_file("resources/shader/060_quad_vs.glsl", "resources/shader/060_depth_mip_fs.glsl");

    lod_levels_ = DEPTH_SIZE_LOG2 + 1;

    GL_CHECK_SIMPLE(glGenTextures(1, &depth_texture_));
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, depth_texture_));
    GL_CHECK_SIMPLE(glTexStorage2D(GL_TEXTURE_2D, lod_levels_, GL_DEPTH24_STENCIL8,
        DEPTH_SIZE, DEPTH_SIZE));

    // We cannot do filtering on depth textures unless we're doing shadow compare (PCF).
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));

    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Useful for debugging purposes so depth shows up as graytone and not just red.
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE));
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, 0));

    // Create FBO chain for each miplevel.
    framebuffers_.resize(lod_levels_);
    GL_CHECK_SIMPLE(glGenFramebuffers(lod_levels_, &framebuffers_[0]));
    for (uint32_t i = 0; i < lod_levels_; i++)
    {
        GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffers_[i]));
        GL_CHECK_SIMPLE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_TEXTURE_2D, depth_texture_, i));

        GL_CHECK_SIMPLE(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            //LOGE("Framebuffer for LOD %u is incomplete!", i);
        }
    }
    GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    GL_CHECK_SIMPLE(glGenBuffers(1, &occluder_.vertex));
    GL_CHECK_SIMPLE(glGenBuffers(1, &occluder_.index));
    GL_CHECK_SIMPLE(glGenVertexArrays(1, &occluder_.vao));

    // Sampler object that is used during occlusion culling.
    // We want GL_LINEAR shadow mode (PCF), but no filtering between miplevels as we manually specify the miplevel in the compute shader.
    GL_CHECK_SIMPLE(glGenSamplers(1, &shadow_sampler_));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler_, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler_, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler_, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));

    GL_CHECK_SIMPLE(glGenBuffers(1, &uniform_buffer));
    GL_CHECK_SIMPLE(glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer));
    GL_CHECK_SIMPLE(glBufferData(GL_UNIFORM_BUFFER, sizeof(Uniforms), NULL, GL_STREAM_DRAW));
}

void HiZCulling::TestBoundingBoxes(GLuint counter_buffer, const uint32_t* counter_offsets, uint32_t num_offsets,
    const GLuint* culled_instance_buffer, GLuint instance_data_buffer,
    uint32_t num_instances)
{
    // 绑定用于遮挡剔除的计算着色器程序 culling_program。
   // 负责执行 Hi-Z 遮挡剔除算法。
    GL_CHECK_SIMPLE(glUseProgram(culling_program));

    // 更新culling program shader 用到uniform变量 更新并绑定 Uniform 缓冲区。
    GL_CHECK_SIMPLE(glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer));
    GL_CHECK_SIMPLE(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Uniforms), &uniforms_));
    GL_CHECK_SIMPLE(glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer));

    // 将工作组数量四舍五入。
    // 生成的几个额外线程由于检查了 num_instances 而立即终止。
    // 计算需要派发的工作组数量，并设置实例数量 Uniform。
    // GROUP_SIZE_AABB 是每个工作组的大小（通常为 64 或 128）。
    // aabb_groups 是计算得到的工作组数量，确保所有实例都被处理。
    // ApplyUint 将实例数量 num_instances 传递给计算着色器。
    uint32_t aabb_groups = (num_instances + GROUP_SIZE_AABB - 1) / GROUP_SIZE_AABB;
    GL_CHECK_SIMPLE(glProgramUniform1ui(culling_program, 0, num_instances));

    // 绑定原子计数器和剔除后的实例缓冲区。
    for (uint32_t i = 0; i < num_offsets; i++)
    {
        GL_CHECK_SIMPLE(glBindBufferRange(GL_ATOMIC_COUNTER_BUFFER, i, counter_buffer, counter_offsets[i], sizeof(uint32_t)));
        GL_CHECK_SIMPLE(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1 + i, culled_instance_buffer[i]));
    }

    // 深度图绑定深度图纹理给compute shader
    // 绑定 Hi - Z 深度图和采样器。
    // depth_texture 是 Hi - Z 深度图，用于遮挡测试。
    // shadow_sampler 是采样器对象，用于控制纹理采样方式。
    GL_CHECK_SIMPLE(glActiveTexture(GL_TEXTURE0));
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, depth_texture_));
    GL_CHECK_SIMPLE(glBindSampler(0, shadow_sampler_));

    // 作用：派发计算着色器任务。
    // glBindBufferBase 将实例数据缓冲区绑定到绑定点 0。
    // glDispatchCompute 派发计算着色器任务，工作组数量为 aabb_groups。
    GL_CHECK_SIMPLE(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, instance_data_buffer));
    GL_CHECK_SIMPLE(glDispatchCompute(aabb_groups, 1, 1));

    GL_CHECK_SIMPLE(glBindSampler(0, 0));

    // 内存屏障
    // 作用：确保计算着色器的写入操作对后续渲染可见。
    // GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT：同步顶点属性数组的访问。
    // GL_COMMAND_BARRIER_BIT：同步命令缓冲区（如间接绘制命令）的访问。
    GL_CHECK_SIMPLE(glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
}

void HiZCulling::SetupOccluderGeometry(const std::vector<glm::vec4>& position, const vector<uint32_t>& indices)
{
    // Upload occlusion geometry to GPU. This should be mostly static.
    GL_CHECK_SIMPLE(glBindVertexArray(occluder_.vao));

    GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, occluder_.vertex));
    GL_CHECK_SIMPLE(glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(glm::vec4), &position[0], GL_STATIC_DRAW));

    GL_CHECK_SIMPLE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, occluder_.index));
    GL_CHECK_SIMPLE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW));

    GL_CHECK_SIMPLE(glEnableVertexAttribArray(0));
    GL_CHECK_SIMPLE(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));

    GL_CHECK_SIMPLE(glBindVertexArray(0));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    occluder_.elements = indices.size();
}

void HiZCulling::RasterizeOccluders()
{
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, 0));
    GL_CHECK_SIMPLE(glEnable(GL_DEPTH_TEST));

    GL_CHECK_SIMPLE(glUseProgram(depth_render_program_));
    GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffers_[0]));

    GL_CHECK_SIMPLE(glBindVertexArray(occluder_.vao));
    GL_CHECK_SIMPLE(glViewport(0, 0, DEPTH_SIZE, DEPTH_SIZE));
    GL_CHECK_SIMPLE(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    GL_CHECK_SIMPLE(glDrawElements(GL_TRIANGLES, occluder_.elements, GL_UNSIGNED_INT, 0));

    GL_CHECK_SIMPLE(glBindVertexArray(quad_.get_vertex_array()));
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, depth_texture_));
    GL_CHECK_SIMPLE(glUseProgram(depth_mip_program_));

    for (uint32_t lod = 1; lod < lod_levels_; lod++)
    {
        GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffers_[lod]));
        GL_CHECK_SIMPLE(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        GL_CHECK_SIMPLE(glViewport(0, 0, DEPTH_SIZE >> lod, DEPTH_SIZE >> lod));
        GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, lod - 1));
        GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, lod - 1));
        GL_CHECK_SIMPLE(glDrawElements(GL_TRIANGLES, quad_.get_num_elements(), GL_UNSIGNED_SHORT, 0));
    }

    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000));
    GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void HiZCulling::SetViewProjectionMatrix(const glm::mat4& projection, const glm::mat4& view, const glm::vec2& zNearFar)
{
    glm::mat4 view_projection = projection * view;
    GL_CHECK_SIMPLE(glProgramUniformMatrix4fv(depth_render_program_, 0, 1, GL_FALSE, value_ptr(view_projection)));

    uniforms_.uVP = view_projection;
    uniforms_.uView = view;
    uniforms_.uProj = projection;
    uniforms_.zNearFar = zNearFar;

    // Compute the 6 frustum planes for frustum culling.
    compute_frustum_from_view_projection(uniforms_.planes, view_projection);
}

HiZCulling::~HiZCulling()
{
    GL_CHECK_SIMPLE(glDeleteTextures(1, &depth_texture_));
    GL_CHECK_SIMPLE(glDeleteProgram(depth_render_program_));
    GL_CHECK_SIMPLE(glDeleteProgram(depth_mip_program_));
    GL_CHECK_SIMPLE(glDeleteProgram(culling_program));
    GL_CHECK_SIMPLE(glDeleteFramebuffers(framebuffers_.size(), &framebuffers_[0]));
    GL_CHECK_SIMPLE(glDeleteBuffers(1, &occluder_.vertex));
    GL_CHECK_SIMPLE(glDeleteBuffers(1, &occluder_.index));
    GL_CHECK_SIMPLE(glDeleteBuffers(1, &uniform_buffer));
    GL_CHECK_SIMPLE(glDeleteVertexArrays(1, &occluder_.vao));
    GL_CHECK_SIMPLE(glDeleteSamplers(1, &shadow_sampler_));
}