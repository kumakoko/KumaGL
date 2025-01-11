/**************************************************************************************************************************
Copyright(C) 2014-2025 www.xionggf.com

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
#ifndef hiz_culling_h__
#define hiz_culling_h__

#include "culling.h"
#include "../klib/kgl_shader_buffer.h"

class HiZCulling : public CullingInterface
{
public:
    HiZCulling();

    HiZCulling(const char* shader_file_path);

    virtual ~HiZCulling();

    virtual void SetupOccluderGeometry(const std::vector<glm::vec4>& positions, const std::vector<uint32_t>& indices) override;

    virtual void SetViewProjectionMatrix(const glm::mat4& projection, const glm::mat4& view, const glm::vec2& zNearFar) override;

    virtual void RasterizeOccluders() override;

    virtual void TestBoundingBoxes(GLuint counter_buffer, const uint32_t* counter_offsets, uint32_t num_offsets,
        const GLuint* culled_instance_buffer,kgl::ShaderBuffer* instance_data_buffer,
        uint32_t num_instances) override;

    virtual GLuint GetDepthTexture() const override;
private:
    kgl::ComputeShaderProgram* culling_program_;
    kgl::GPUProgram* depth_render_program_;
    kgl::GPUProgram* depth_mip_program_;
    GLDrawable* quad_;

    struct
    {
        GLuint vertex;
        GLuint index;
        GLuint vao;
        uint32_t elements;
    } occluder;

    GLuint depth_texture_;
    GLuint shadow_sampler_;
    uint32_t lod_levels_;
    std::vector<GLuint> frame_buffers_;

    GLuint uniform_buffer_;
    struct Uniforms
    {
        glm::mat4 uVP;
        glm::mat4 uView;
        glm::mat4 uProj;
        glm::vec4 planes[6];
        glm::vec2 zNearFar;
    };
    Uniforms uniforms_;

    void init();
};


#endif // hiz_culling_h__