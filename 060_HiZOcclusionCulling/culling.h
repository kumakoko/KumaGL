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
#ifndef culling_h__
#define culling_h__

#include "mesh.h"
#include "constants.h"
#include "kgl_compute_shader_program.h"
#include "kgl_gpu_program.h"

 // Layout is defined by OpenGL ES 3.1.
 // We don't care about the three last elements in this case.
struct IndirectCommand
{
    GLuint count;
    GLuint instanceCount;
    GLuint zero[3];
};

class CullingInterface
{
public:
    virtual ~CullingInterface() 
    {
    }

    // Sets up occlusion geometry. This is mostly static and should be done at startup of a scene.
    virtual void setup_occluder_geometry(const std::vector<glm::vec4>& positions, const std::vector<uint32_t>& indices) = 0;

    // Sets current view and projection matrices.
    virtual void set_view_projection(const glm::mat4& projection, const glm::mat4& view, const glm::vec2& zNearFar) = 0;

    // Rasterize occluders to depth map.
    virtual void rasterize_occluders() = 0;

    // Test bounding boxes in our scene.
    virtual void test_bounding_boxes(GLuint counter_buffer, const uint32_t* counter_offsets, uint32_t num_offsets,
        const GLuint* culled_instance_buffer, GLuint instance_data_buffer,
        uint32_t num_instances) = 0;

    // Debugging functionality. Verify that the depth map is being rasterized correctly.
    virtual GLuint get_depth_texture() const { return 0; }

    virtual uint32_t get_num_lods() const { return SPHERE_LODS; }

protected:
    // Common functionality for various occlusion culling implementations.
    void compute_frustum_from_view_projection(glm::vec4* planes, const glm::mat4& view_projection);
};





#endif // culling_h__