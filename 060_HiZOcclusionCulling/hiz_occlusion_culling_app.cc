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