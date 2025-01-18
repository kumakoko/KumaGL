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

#ifndef CULLING_HPP__
#define CULLING_HPP__

#include "constants.h"
#include "common.h"
#include "mesh.h"

enum CullingMethod
{
    CullHiZ = 0,
    CullHiZNoLOD = 1,
    CullNone = -1
};

struct OccluderInfo
{
    GLuint vertex;
    GLuint index;
    GLuint vao;
    uint32_t elements;
};

 // Layout is defined by OpenGL ES 3.1.
 // We don't care about the three last elements in this case.
struct IndirectCommand
{
    GLuint count;
    GLuint instanceCount;
    GLuint zero[3];
};

struct Uniforms
{
    glm::mat4 uVP;
    glm::mat4 uView;
    glm::mat4 uProj;
    glm::vec4 planes[6];
    glm::vec2 zNearFar;
};

class Culling
{
public:
    virtual ~Culling() {}

    virtual void SetupOccluderGeometry(const std::vector<glm::vec4>& positions, const std::vector<uint32_t>& indices) = 0;

    virtual void SetViewProjectionMatrix(const glm::mat4& projection, const glm::mat4& view, const glm::vec2& zNearFar) = 0;

    virtual void RasterizeOccluders() = 0;

    virtual void TestBoundingBoxes(GLuint counter_buffer, const uint32_t* counter_offsets, uint32_t num_offsets,
        const GLuint* culled_instance_buffer, GLuint instance_data_buffer,
        uint32_t num_instances) = 0;

    virtual GLuint GetDepthTexture() const { return 0; }

    virtual uint32_t GetLodNum() const { return SPHERE_LODS; }

protected:
    // Common functionality for various occlusion culling implementations.
    void compute_frustum_from_view_projection(glm::vec4* planes, const glm::mat4& view_projection);
};

#endif
