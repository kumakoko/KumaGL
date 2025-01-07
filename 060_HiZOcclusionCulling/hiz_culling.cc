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
#include "../klib/kgl_defines.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"

#include "constants.h"
#include "hiz_culling.h"

HiZCulling::HiZCulling()
{
    culling_program_ = new kgl::ComputeShaderProgram();
    culling_program_->CreateFromSingleFile("resources/shader/060_hiz_cull_cs.glsl");
    init();
}

HiZCulling::HiZCulling(const char* shader_file_path)
{
    culling_program_ = new kgl::ComputeShaderProgram();
    culling_program_->CreateFromSingleFile(shader_file_path);
    init();
}

void HiZCulling::init()
{
    quad = new GLDrawable();

    depth_render_program_ = new kgl::GPUProgram();
    depth_render_program_->CreateFromFile("resources/shader/060_depth_vs.glsl", "resources/shader/060_depth_fs.glsl", nullptr);
    depth_mip_program_ = new kgl::GPUProgram();
    depth_mip_program_->CreateFromFile("resources/shader/060_quad_vs.glsl", "resources/shader/060_depth_mip_fs.glsl", nullptr);
    
    lod_levels = DEPTH_SIZE_LOG2 + 1;

    GL_CHECK_SIMPLE(glGenTextures(1, &depth_texture));
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, depth_texture));
    GL_CHECK_SIMPLE(glTexStorage2D(GL_TEXTURE_2D, lod_levels, GL_DEPTH24_STENCIL8, DEPTH_SIZE, DEPTH_SIZE));

    // We cannot do filtering on depth textures unless we're doing shadow compare (PCF).
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));

    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Useful for debugging purposes so depth shows up as graytone and not just red.
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE));
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, 0));

    // Create FBO chain for each miplevel.
    framebuffers.resize(lod_levels);
    GL_CHECK_SIMPLE(glGenFramebuffers(lod_levels, &framebuffers[0]));

    for (uint32_t i = 0; i < lod_levels; i++)
    {
        GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]));
        GL_CHECK_SIMPLE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,GL_TEXTURE_2D, depth_texture, i));

        GL_CHECK_SIMPLE(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));

        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            //LOGE("Framebuffer for LOD %u is incomplete!", i);
        }
    }
    GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    GL_CHECK_SIMPLE(glGenBuffers(1, &occluder.vertex));
    GL_CHECK_SIMPLE(glGenBuffers(1, &occluder.index));
    GL_CHECK_SIMPLE(glGenVertexArrays(1, &occluder.vao));

    // Sampler object that is used during occlusion culling.
    // We want GL_LINEAR shadow mode (PCF), but no filtering between miplevels as we manually specify the miplevel in the compute shader.

    GL_CHECK_SIMPLE(glGenSamplers(1, &shadow_sampler));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
    GL_CHECK_SIMPLE(glSamplerParameteri(shadow_sampler, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));

    GL_CHECK_SIMPLE(glGenBuffers(1, &uniform_buffer));
    GL_CHECK_SIMPLE(glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer));
    GL_CHECK_SIMPLE(glBufferData(GL_UNIFORM_BUFFER, sizeof(Uniforms), NULL, GL_STREAM_DRAW));
}

void HiZCulling::TestBoundingBoxes(GLuint counter_buffer, const uint32_t* counter_offsets, uint32_t num_offsets,
    const GLuint* culled_instance_buffer, kgl::ShaderBuffer* instance_data_buffer/*GLuint instance_data_buffer*/, uint32_t num_instances)
{
    culling_program_->Use();//GL_CHECK(glUseProgram(culling_program));

    // Update uniform buffer.
    GL_CHECK_SIMPLE(glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer));
    GL_CHECK_SIMPLE(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Uniforms), &uniforms));
    GL_CHECK_SIMPLE(glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer));

    // Round up number of work groups.
    // The few extra threads we spawn terminate immediately due to check against num_instances.
    uint32_t aabb_groups = (num_instances + GROUP_SIZE_AABB - 1) / GROUP_SIZE_AABB;
    culling_program_->ApplyUint(num_instances, 0);//GL_CHECK(glProgramUniform1ui(culling_program, 0, num_instances));

    for (uint32_t i = 0; i < num_offsets; i++)
    {
        GL_CHECK_SIMPLE(glBindBufferRange(GL_ATOMIC_COUNTER_BUFFER, i, counter_buffer, counter_offsets[i], sizeof(uint32_t)));
        GL_CHECK_SIMPLE(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1 + i, culled_instance_buffer[i]));
    }

    // Bind Hi-Z depth map.
    GL_CHECK_SIMPLE(glActiveTexture(GL_TEXTURE0));
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, depth_texture));
    GL_CHECK_SIMPLE(glBindSampler(0, shadow_sampler));

    // Dispatch occlusion culling job.
    instance_data_buffer->BindBufferBase(0);//GL_CHECK_SIMPLE(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, instance_data_buffer));
    GL_CHECK_SIMPLE(kgl::ComputeShaderProgram::Dispatch(aabb_groups, 1, 1));

    GL_CHECK_SIMPLE(glBindSampler(0, 0));

    // We have updated instance buffer and indirect draw buffer. Memory barrier here to ensure visibility.
    GL_CHECK_SIMPLE(glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
}

void HiZCulling::SetupOccluderGeometry(const std::vector<glm::vec4>& position, const std::vector<uint32_t>& indices)
{
    // 把遮挡物的顶点数据和索引数据上传到GPU
    GL_CHECK_SIMPLE(glBindVertexArray(occluder.vao));

    GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, occluder.vertex));
    GL_CHECK_SIMPLE(glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(glm::vec4), &position[0], GL_STATIC_DRAW));

    GL_CHECK_SIMPLE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, occluder.index));
    GL_CHECK_SIMPLE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW));

    GL_CHECK_SIMPLE(glEnableVertexAttribArray(0));
    GL_CHECK_SIMPLE(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));

    GL_CHECK_SIMPLE(glBindVertexArray(0));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    occluder.elements = indices.size();
}

void HiZCulling::RasterizeOccluders()
{
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, 0));
    GL_CHECK_SIMPLE(glEnable(GL_DEPTH_TEST));

    depth_render_program_->Use(); //GL_CHECK(glUseProgram(depth_render_program));
    GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]));

    // Render occlusion geometry to miplevel 0.
    GL_CHECK_SIMPLE(glBindVertexArray(occluder.vao));
    GL_CHECK_SIMPLE(glViewport(0, 0, DEPTH_SIZE, DEPTH_SIZE));
    GL_CHECK_SIMPLE(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    GL_CHECK_SIMPLE(glDrawElements(GL_TRIANGLES, occluder.elements, GL_UNSIGNED_INT, 0));

    GL_CHECK_SIMPLE(glBindVertexArray(quad->get_vertex_array()));
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, depth_texture));
    depth_mip_program_->Use();//GL_CHECK(glUseProgram(depth_mip_program));

    for (uint32_t lod = 1; lod < lod_levels; lod++)
    {
        GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[lod]));
        GL_CHECK_SIMPLE(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        GL_CHECK_SIMPLE(glViewport(0, 0, DEPTH_SIZE >> lod, DEPTH_SIZE >> lod));

        // Need to do this to ensure that we cannot possibly read from the miplevel we are rendering to.
        // Otherwise, we have undefined behavior.
        GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, lod - 1));
        GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, lod - 1));

        // Mipmap.
        GL_CHECK_SIMPLE(glDrawElements(GL_TRIANGLES, quad->get_num_elements(), GL_UNSIGNED_SHORT, 0));
    }

    // Restore miplevels. MAX_LEVEL will be clamped accordingly.
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    GL_CHECK_SIMPLE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000));
    GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void HiZCulling::SetViewProjectionMatrix(const glm::mat4& projection, const glm::mat4& view, const glm::vec2& zNearFar)
{
    glm::mat4 view_projection = projection * view;
    depth_render_program_->ApplyMatrix(glm::value_ptr(view_projection), 0, false);
    //GL_CHECK(glProgramUniformMatrix4fv(depth_render_program, 0, 1, GL_FALSE, value_ptr(view_projection)));

    uniforms.uVP = view_projection;
    uniforms.uView = view;
    uniforms.uProj = projection;
    uniforms.zNearFar = zNearFar;

    // Compute the 6 frustum planes for frustum culling.
    ComputeFrustumFromViewProjection(uniforms.planes, view_projection);
}

HiZCulling::~HiZCulling()
{
    GL_CHECK_SIMPLE(glDeleteTextures(1, &depth_texture));

    KGL_SAFE_DELETE(depth_render_program_);//GL_CHECK(glDeleteProgram(depth_render_program));
    KGL_SAFE_DELETE(depth_mip_program_);//GL_CHECK(glDeleteProgram(depth_mip_program));
    KGL_SAFE_DELETE(culling_program_);//GL_CHECK(glDeleteProgram(culling_program));
    
    GL_CHECK(glDeleteFramebuffers(framebuffers.size(), &framebuffers[0]));

    GL_CHECK_SIMPLE(glDeleteBuffers(1, &occluder.vertex));
    GL_CHECK_SIMPLE(glDeleteBuffers(1, &occluder.index));
    GL_CHECK_SIMPLE(glDeleteBuffers(1, &uniform_buffer));
    GL_CHECK_SIMPLE(glDeleteVertexArrays(1, &occluder.vao));

    GL_CHECK_SIMPLE(glDeleteSamplers(1, &shadow_sampler));
}

GLuint HiZCulling::GetDepthTexture() const
{
    return depth_texture;
}