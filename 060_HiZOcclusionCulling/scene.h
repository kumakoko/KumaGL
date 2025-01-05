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
#ifndef scene_h__
#define scene_h__

#include "mesh.hpp"
#include "culling.hpp"
#include <vector>
#include <stdint.h>

class Scene
{
public:
    Scene();
    ~Scene();

    void update(float delta_time, uint32_t width, uint32_t height);
    void render(uint32_t width, uint32_t height);
    void move_camera(float delta_x, float delta_y);

    enum CullingMethod 
    {
        CullHiZ = 0,
        CullHiZNoLOD = 1,
        CullNone = -1
    };
    
    void set_culling_method(CullingMethod method);
    void set_physics_speed(float speed) { physics_speed = speed; }
    float get_physics_speed() const { return physics_speed; }
    void set_show_redundant(bool enable) { show_redundant = enable; }
    bool get_show_redundant() const { return show_redundant; }

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

    GLuint occluder_program;
    GLuint sphere_program;

    GLDrawable quad;
    GLuint quad_program;

    // Allow for readbacks of atomic counter without stalling GPU pipeline.
#define INDIRECT_BUFFERS 4
    struct
    {
        GLuint buffer[INDIRECT_BUFFERS];
        uint32_t buffer_index;
        GLuint instance_buffer[SPHERE_LODS];
    } indirect;

    void init_instances();
    GLuint physics_program;
    GLuint occluder_instances_buffer;
    GLuint sphere_instances_buffer;
    uint32_t num_occluder_instances;
    uint32_t num_sphere_render_lods;
    uint32_t num_render_sphere_instances;

    void apply_physics(float delta_time);
    float physics_speed;

    void render_depth_map();

    glm::mat4 projection;
    glm::mat4 view;

    float camera_rotation_y;
    float camera_rotation_x;
    void update_camera(float rotation_y, float rotation_x, uint32_t viewport_width, uint32_t viewport_height);
};

#endif // scene_h__