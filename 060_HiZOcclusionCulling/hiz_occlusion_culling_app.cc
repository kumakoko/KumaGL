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
#include "hiz_occlusion_culling_app.h"

void HizOcclusionCullingApp::RenderGUI()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window_)
        ImGui::ShowDemoWindow(&show_demo_window_);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("001 Empty Window -- 简单的ImGui界面演示");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("有用的文本");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window_);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window_);
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("按钮"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }


    // 3. Show another simple window.
    if (show_another_window_)
    {
        ImGui::Begin("Another Window", &show_another_window_);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window_ = false;
        ImGui::End();
    }
}

HizOcclusionCullingApp::~HizOcclusionCullingApp()
{
    KGL_SAFE_DELETE(physics_program_);
}

void HizOcclusionCullingApp::InitShaders()
{
    physics_program_ = new kgl::ComputeShaderProgram();
    GL_CHECK_SIMPLE(physics_program_->CreateFromSingleFile("resources/shader/060_physics_cs.glsl"));

    occluder_program = new kgl::GPUProgram();
    GL_CHECK_SIMPLE(occluder_program->CreateFromFile("resources/shader/060_scene_vs.glsl", "resources/shader/060_scene_fs.glsl", nullptr));

    sphere_program = new kgl::GPUProgram();
    GL_CHECK_SIMPLE(occluder_program->CreateFromFile("resources/shader/060_scene_sphere_vs.glsl", "resources/shader/060_scene_sphere_fs.glsl", nullptr));
}



void HizOcclusionCullingApp::apply_physics(float delta_time)
{
    if (physics_speed <= 0.0f)
    {
        return;
    }

    physics_program_->Use();
    //physics_program_->BindShaderStorageBuffer(0, sphere_instances_buffer);
    physics_program_->ApplyUint(SPHERE_INSTANCES, 0);
    physics_program_->ApplyFloat(physics_speed * delta_time, 1);
    physics_program_->Dispatch((SPHERE_INSTANCES + PHYSICS_GROUP_SIZE - 1) / PHYSICS_GROUP_SIZE, 1, 1);

    /**
    // Do physics on the spheres, in a compute shader.
    GL_CHECK(glUseProgram(physics_program));
    GL_CHECK(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sphere_instances_buffer));
    GL_CHECK(glProgramUniform1ui(physics_program, 0, SPHERE_INSTANCES));
    GL_CHECK(glProgramUniform1f(physics_program, 1, physics_speed * delta_time));
    GL_CHECK(glDispatchCompute((SPHERE_INSTANCES + PHYSICS_GROUP_SIZE - 1) / PHYSICS_GROUP_SIZE, 1, 1));
    */

}

void HizOcclusionCullingApp::init_instances()
{
    // Create occluder boxes.
    AABB aabb = { glm::vec4(-1.0f, 0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 8.0f, 1.0f, 0.0f) };
    Mesh box_mesh = Mesh::create_box_mesh(aabb);
    box = new GLDrawable(box_mesh);

    // Create meshes for spheres at various LOD levels.
    for (unsigned i = 0; i < SPHERE_LODS; i++)
    {
        sphere[i] = new GLDrawable(Mesh::create_sphere_mesh(1.0f, glm::vec3(0, 0, 0), verts_per_circ[i]));
    }

    // Spread occluder geometry out on a grid on the XZ plane.
    // Skip the center, because we put our camera there.
    std::vector<glm::vec4> occluder_instances;

    for (int z = 0; z < 13; z++)
    {
        for (int x = 0; x < 13; x++)
        {
            if (z >= 5 && z <= 7 && x >= 5 && x <= 7)
            {
                continue;
            }
            occluder_instances.push_back(glm::vec4(3.0f) * glm::vec4(x - 6, 0, z - 6, 0));
        }
    }
    // Rough front-to-back ordering to get more ideal rendering order.
    std::sort(occluder_instances.begin(), occluder_instances.end(), OccluderSorter());

    num_occluder_instances = occluder_instances.size();

    // Upload instance buffer.
    occluder_instances_buffer = new kgl::ShaderBuffer(GL_UNIFORM_BUFFER);
    occluder_instances_buffer->BindBufferAndUploadData(occluder_instances.size() * sizeof(glm::vec4), &occluder_instances[0], GL_STATIC_DRAW;
    //GL_CHECK(glGenBuffers(1, &occluder_instances_buffer));
    //GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, occluder_instances_buffer));
    //GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, occluder_instances.size() * sizeof(vec4), &occluder_instances[0], GL_STATIC_DRAW));

    // Place out spheres with different positions and velocities.
    // The W component contains the sphere radius, which is random.
    std::vector<SphereInstance> sphere_instances;
    for (int x = 0; x < SPHERE_INSTANCES_X; x++)
    {
        for (int y = 0; y < SPHERE_INSTANCES_Y; y++)
        {
            for (int z = 0; z < SPHERE_INSTANCES_Z; z++)
            {
                SphereInstance instance;
                instance.position = vec4(1.0f) * vec4(x - 11.35f, y * 0.10f + 0.5f, z - 11.45f, 0);
                instance.position.c.w = SPHERE_RADIUS * (1.0f - 0.5f * rand() / RAND_MAX);
                instance.velocity = vec4(vec3(4.0) * vec_normalize(vec3(x - 11.35f, 0.5f * y - 11.55f, z - 11.25f)), 0.0f);

                sphere_instances.push_back(instance);
            }
        }
    }

    // Upload sphere instance buffer.
    sphere_instances_buffer = new kgl::ShaderBuffer(GL_SHADER_STORAGE_BUFFER);
    sphere_instances_buffer->BindBufferAndUploadData(sphere_instances.size() * sizeof(SphereInstance), &sphere_instances[0], GL_STATIC_DRAW));
    /*GL_CHECK(glGenBuffers(1, &sphere_instances_buffer));
    GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, sphere_instances_buffer));
    GL_CHECK(glBufferData(GL_SHADER_STORAGE_BUFFER, sphere_instances.size() * sizeof(SphereInstance), &sphere_instances[0], GL_STATIC_DRAW));*/

    // Initialize storage for our post-culled instance buffer.
    // The buffers must be at least as large as the sphere instance buffer (in case we have 100% visibility).
    GL_CHECK(glGenBuffers(SPHERE_LODS, indirect.instance_buffer));
    for (unsigned i = 0; i < SPHERE_LODS; i++)
    {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, indirect.instance_buffer[i]));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sphere_instances.size() * sizeof(vec4), NULL, GL_DYNAMIC_COPY));
    }

    // Setup occluder geometry for each implementation.
    vector<vec4> occluder_positions;
    vector<uint32_t> occluder_indices;
    bake_occluder_geometry(occluder_positions, occluder_indices, box_mesh, &occluder_instances[0], occluder_instances.size());

    for (unsigned i = 0; i < culling_implementations.size(); i++)
    {
        culling_implementations[i]->setup_occluder_geometry(occluder_positions, occluder_indices);
    }

    // Initialize our indirect draw buffers.
    // Use a ring buffer of them, since we might want to read back old results to monitor our culling performance without stalling the pipeline.
    GL_CHECK(glGenBuffers(INDIRECT_BUFFERS, indirect.buffer));
    for (unsigned i = 0; i < SPHERE_LODS; i++)
    {
        GL_CHECK(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect.buffer[i]));
        GL_CHECK(glBufferData(GL_DRAW_INDIRECT_BUFFER, SPHERE_LODS * sizeof(IndirectCommand), NULL, GL_DYNAMIC_COPY));
    }
    indirect.buffer_index = 0;
}




void HizOcclusionCullingApp::update_camera(float rotation_y, float rotation_x, uint32_t viewport_width, uint32_t viewport_height)
{
    // Compute view and projection matrices.
    float radians_y = 2.0f * glm::pi<float>() * rotation_y;
    float radians_x = 2.0f * glm::pi<float>() * rotation_x;

    glm::mat4 identity_mat = glm::mat4(1.0f);

    glm::mat4 rotation_matrix_y = glm::rotate(identity_mat, radians_y, glm::vec3(0.0f, 1.0f, 0.0f));//mat_rotate_y(radians_y);
    glm::mat4 rotation_matrix_x = glm::rotate(identity_mat, radians_x, glm::vec3(1.0f, 0.0f, 0.0f));//mat_rotate_x(radians_x);
    glm::vec3 camera_dir = glm::vec3(rotation_matrix_y * rotation_matrix_x * glm::vec4(0, 0, -1, 1));

    glm::vec3 camera_position = glm::vec3(0, 2, 0);

    view = glm::lookAt(camera_position, camera_position + camera_dir, glm::vec3(0, 1, 0));
    projection = glm::perspective(glm::radians(60.0f), float(viewport_width) / viewport_height, Z_NEAR, Z_FAR);//mat_perspective_fov(60.0f, float(viewport_width) / viewport_height, Z_NEAR, Z_FAR);
    glm::mat4 view_projection = projection * view;

    auto vp_mat_ptr = glm::value_ptr(view_projection);
    occluder_program->ApplyMatrix(vp_mat_ptr, UNIFORM_MVP_LOCATION);
    sphere_program->ApplyMatrix(vp_mat_ptr, UNIFORM_MVP_LOCATION);

    //GL_CHECK(glProgramUniformMatrix4fv(occluder_program, UNIFORM_MVP_LOCATION, 1, GL_FALSE, value_ptr(view_projection)));
    //GL_CHECK(glProgramUniformMatrix4fv(sphere_program, UNIFORM_MVP_LOCATION, 1, GL_FALSE, value_ptr(view_projection)));
}

void HizOcclusionCullingApp::set_culling_method(CullingMethod method)
{
    if (method == CullNone)
    {
        enable_culling = false;
        culling_implementation_index = 0;
    }
    else
    {
        enable_culling = true;
        culling_implementation_index = static_cast<unsigned>(method);
    }
}

void HizOcclusionCullingApp::apply_physics(float delta_time)
{
    if (physics_speed <= 0.0f)
    {
        return;
    }

    // Do physics on the spheres, in a compute shader.
    GL_CHECK(glUseProgram(physics_program));
    GL_CHECK(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sphere_instances_buffer));
    GL_CHECK(glProgramUniform1ui(physics_program, 0, SPHERE_INSTANCES));
    GL_CHECK(glProgramUniform1f(physics_program, 1, physics_speed * delta_time));
    GL_CHECK(glDispatchCompute((SPHERE_INSTANCES + PHYSICS_GROUP_SIZE - 1) / PHYSICS_GROUP_SIZE, 1, 1));

    // We don't need data here until bounding box check, so we can let rasterizer and physics run in parallel, avoiding memory barrier here.
}

void HizOcclusionCullingApp::update(float delta_time, unsigned width, unsigned height)
{
    // Update scene rendering parameters.
    update_camera(camera_rotation_y, camera_rotation_x, width, height);

    // Update light direction, here it's static.
    glm::vec3 light_dir = glm::normalize(glm::vec3(2, 4, 1));

    occluder_program->ApplyVector3(glm::value_ptr(light_dir), UNIFORM_LIGHT_DIR_LOCATION);
    sphere_program->ApplyVector3(glm::value_ptr(light_dir), UNIFORM_LIGHT_DIR_LOCATION);
    //GL_CHECK(glProgramUniform3fv(occluder_program, UNIFORM_LIGHT_DIR_LOCATION, 1, value_ptr(light_dir)));
    //GL_CHECK(glProgramUniform3fv(sphere_program, UNIFORM_LIGHT_DIR_LOCATION, 1, value_ptr(light_dir)));

    // Move spheres around in a compute shader to make it more exciting.
    apply_physics(delta_time);

    if (enable_culling)
    {
        CullingInterface* culler = culling_implementations[culling_implementation_index];
        num_sphere_render_lods = culler->get_num_lods();

        // Rasterize occluders to depth map and mipmap it.
        culler->set_view_projection(projection, view, vec2(Z_NEAR, Z_FAR));
        culler->rasterize_occluders();

        // We need physics results after this.
        GL_CHECK(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

        IndirectCommand indirect_command[SPHERE_LODS];
        unsigned offsets[SPHERE_LODS];

        memset(indirect_command, 0, sizeof(indirect_command));

        for (unsigned i = 0; i < SPHERE_LODS; i++)
        {
            indirect_command[i].count = sphere[i]->get_num_elements();
            offsets[i] = 4 + sizeof(IndirectCommand) * i;
        }

        // Clear out our indirect draw buffer.
        GL_CHECK(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect.buffer[indirect.buffer_index]));
        GL_CHECK(glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(indirect_command), indirect_command, GL_STREAM_DRAW));

        // Test occluders and build indirect commands as well as per-instance buffers for every LOD.
        culler->test_bounding_boxes(indirect.buffer[indirect.buffer_index], offsets, SPHERE_LODS,
            indirect.instance_buffer, sphere_instances_buffer,
            num_render_sphere_instances);
    }
    else
    {
        // If we don't do culling, we need to make sure we call the memory barrier for physics.
        GL_CHECK(glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT));
        num_sphere_render_lods = 1;
    }
}

void HizOcclusionCullingApp::render_spheres(vec3 color_mod)
{
    if (enable_culling)
    {
        GL_CHECK(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect.buffer[indirect.buffer_index]));

        for (unsigned i = 0; i < num_sphere_render_lods; i++)
        {
            // Use different colors for different LOD levels to easily spot them.
            GL_CHECK(glProgramUniform3fv(sphere_program, UNIFORM_COLOR_LOCATION, 1,
                value_ptr(color_mod * vec3(0.8f - 0.2f * i, 1.2f - 0.2f * i, 0.8f + 0.2f * i))));

            // Use different meshes for different LOD levels.
            GL_CHECK(glBindVertexArray(sphere[i]->get_vertex_array()));

            GL_CHECK(glEnableVertexAttribArray(3));
            GL_CHECK(glVertexAttribDivisor(3, 1));
            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, indirect.instance_buffer[i]));
            GL_CHECK(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), 0));

            GL_CHECK(glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT,
                reinterpret_cast<const void*>(i * sizeof(IndirectCommand))));
        }
    }
    else
    {
        // Unconditionally draw every instance of LOD0.
        GL_CHECK(glProgramUniform3fv(sphere_program, UNIFORM_COLOR_LOCATION, 1, value_ptr(color_mod * vec3(0.8f, 1.2f, 0.8f))));
        GL_CHECK(glBindVertexArray(sphere[0]->get_vertex_array()));
        GL_CHECK(glEnableVertexAttribArray(3));
        GL_CHECK(glVertexAttribDivisor(3, 1));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, sphere_instances_buffer));
        GL_CHECK(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(vec4), 0));
        GL_CHECK(glDrawElementsInstanced(GL_TRIANGLES, sphere[0]->get_num_elements(),
            GL_UNSIGNED_SHORT, NULL, num_render_sphere_instances));
    }
}

void Scene::render(unsigned width, unsigned height)
{
    if (enable_culling)
    {
        GL_CHECK(glClearColor(0.02f, 0.02f, 0.35f, 0.05f));
    }
    else
    {
        GL_CHECK(glClearColor(0.35f, 0.02f, 0.02f, 0.05f));
    }

    // Enable depth testing and culling.
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glEnable(GL_CULL_FACE));

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    GL_CHECK(glViewport(0, 0, width, height));

    // Render occluder boxes.
    GL_CHECK(glUseProgram(occluder_program));
    GL_CHECK(glProgramUniform3f(occluder_program, UNIFORM_COLOR_LOCATION, 1.2f, 0.6f, 0.6f));
    GL_CHECK(glBindVertexArray(box->get_vertex_array()));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, occluder_instances_buffer));
    GL_CHECK(glEnableVertexAttribArray(3));
    GL_CHECK(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vec4), 0));
    GL_CHECK(glVertexAttribDivisor(3, 1));
    GL_CHECK(glDrawElementsInstanced(GL_TRIANGLES, box->get_num_elements(), GL_UNSIGNED_SHORT, 0, num_occluder_instances));

    GL_CHECK(glUseProgram(sphere_program));
    if (show_redundant)
    {
        // Draw false-positive meshes in a dark color.
        // False-positives will fail the depth test (pass with GL_GREATER).
        // We don't want to update the depth buffer, so the false-positives will be rendered in a "glitchy"
        // way due to the random ordering that occlusion culling introduces.
        GL_CHECK(glDepthFunc(GL_GREATER));
        GL_CHECK(glDepthMask(GL_FALSE));
        render_spheres(vec3(0.25f));
        GL_CHECK(glDepthMask(GL_TRUE));
        GL_CHECK(glDepthFunc(GL_LESS));
    }
    render_spheres(vec3(1.0f));

    if (enable_culling)
    {
        render_depth_map();
    }

    // Restore viewport (for text rendering).
    GL_CHECK(glViewport(0, 0, width, height));

    // Jump to next indirect draw buffer (ring buffer).
    indirect.buffer_index = (indirect.buffer_index + 1) % INDIRECT_BUFFERS;
}

void Scene::render_depth_map()
{
    GL_CHECK(glDisable(GL_DEPTH_TEST));
    GL_CHECK(glUseProgram(quad_program));

    // Debug
    GL_CHECK(glBindVertexArray(quad.get_vertex_array()));

    unsigned offset_x = 0;

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, culling_implementations[culling_implementation_index]->get_depth_texture()));
    for (unsigned lod = 0; lod <= DEPTH_SIZE_LOG2; lod++)
    {
        GL_CHECK(glViewport(offset_x, 0, DEPTH_SIZE >> lod, DEPTH_SIZE >> lod));

        // Mipmapped filtering mode will ensure we draw correct miplevel.
        GL_CHECK(glDrawElements(GL_TRIANGLES, quad.get_num_elements(), GL_UNSIGNED_SHORT, 0));

        offset_x += DEPTH_SIZE >> lod;
    }
}