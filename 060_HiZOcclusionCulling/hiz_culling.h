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

class HiZCulling : public Culling
{
public:
    HiZCulling();
    HiZCulling(const char* program);
    ~HiZCulling();

    void SetupOccluderGeometry(const std::vector<glm::vec4>& positions, const std::vector<uint32_t>& indices);
    void SetViewProjectionMatrix(const glm::mat4& projection, const glm::mat4& view, const glm::vec2& zNearFar);

    void RasterizeOccluders();
    void TestBoundingBoxes(GLuint counter_buffer, const uint32_t* counter_offsets, uint32_t num_offsets,
        const GLuint* culled_instance_buffer, GLuint instance_data_buffer,
        uint32_t num_instances);

    GLuint GetDepthTexture() const { return depth_texture_; }

private:
    GLuint depth_render_program_;
    GLuint depth_mip_program_;
    GLuint culling_program;
    GLuint depth_texture_;
    GLuint shadow_sampler_;
    GLuint uniform_buffer;
    GLDrawable quad_;
    OccluderInfo occluder_;
    uint32_t lod_levels_;
    std::vector<GLuint> framebuffers_;
    Uniforms uniforms_;
    void init();
};


#endif // hiz_culling_h__