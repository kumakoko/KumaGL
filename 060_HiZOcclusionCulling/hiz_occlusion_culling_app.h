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
#ifndef hiz_occlusion_culling_app_h__
#define hiz_occlusion_culling_app_h__


#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_compute_shader_program.h"
#include "../klib/kgl_shader_buffer.h"

#include "constants.h"
#include "culling.h"


class HizOcclusionCullingApp : public kgl::App
{
public:
    virtual ~HizOcclusionCullingApp();
protected:
    virtual void RenderGUI() override;
    virtual void InitShaders() override;
    bool show_demo_window_ = false;
    bool show_another_window_ = false;

private:
    GLDrawable* box;
    GLDrawable* sphere[SPHERE_LODS];
    std::vector<CullingInterface*> culling_implementations;

    uint32_t culling_implementation_index;

    bool show_redundant;
    bool enable_culling;

    void render_spheres(vec3 color_mod);

    void bake_occluder_geometry(std::vector<vec4>& occluder_positions,
        std::vector<uint32_t>& occluder_indices,
        const Mesh& box_mesh, const vec4* instances, uint32_t num_instances);

    void init_instances();

    void update_camera(float rotation_y, float rotation_x, uint32_t viewport_width, uint32_t viewport_height);

    void update(float delta_time, uint32_t width, uint32_t height);

  //  GLuint occluder_program;
  //  GLuint sphere_program;

    GLDrawable quad;
    GLuint quad_program;

    kgl::ComputeShaderProgram* physics_program_;

    void apply_physics(float delta_time);
    float physics_speed;
    GLuint sphere_instances_buffer;

    uint32_t num_occluder_instances;
    kgl::ShaderBuffer* occluder_instances_buffer;
    kgl::ShaderBuffer* sphere_instances_buffer;

    kgl::GPUProgram* occluder_program;
    kgl::GPUProgram* sphere_program;


    glm::mat4 view;
    glm::mat4 projection;

    float camera_rotation_y;
    float camera_rotation_x;

    uint32_t num_sphere_render_lods;
};


#endif // hiz_occlusion_culling_app_h__