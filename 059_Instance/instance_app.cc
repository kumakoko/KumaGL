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
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "instance_app.h"

#define UNIFORM_MVP_LOCATION 0
#define UNIFORM_COLOR_LOCATION 1
#define UNIFORM_LIGHT_DIR_LOCATION 2

void InstanceApp::RenderGUI()
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


void InstanceApp::InitModel()
{
    // Create occluder boxes.
    AABB aabb = { glm::vec4(-1.0f, 0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 8.0f, 1.0f, 0.0f) };
    Mesh box_mesh = Mesh::CreateBoxMesh(aabb);
    box = new GLDrawable(box_mesh);

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
    std::sort(occluder_instances.begin(), occluder_instances.end(), 
        [](const glm::vec4& a, const glm::vec4& b)
        {
            return glm::dot(a, a) < glm::dot(b, b);
        });

    num_occluder_instances = occluder_instances.size();

    // Upload instance buffer.
    GL_CHECK_SIMPLE(glGenBuffers(1, &occluder_instances_buffer));
    GL_CHECK_SIMPLE(glBindBuffer(GL_UNIFORM_BUFFER, occluder_instances_buffer));
    GL_CHECK_SIMPLE(glBufferData(GL_UNIFORM_BUFFER, occluder_instances.size() * sizeof(glm::vec4), &occluder_instances[0], GL_STATIC_DRAW));
}

void InstanceApp::InitShaders()
{
    occluder_program = new kgl::GPUProgram();
    GL_CHECK_SIMPLE(occluder_program->CreateFromFile("resources/shader/060_scene_vs.glsl", "resources/shader/060_scene_fs.glsl", nullptr));
}

void InstanceApp::UpdateScene()
{
    // 更新摄像机
    UpdateCamera(camera_rotation_y_, camera_rotation_x_, window_width_, window_height_);

    // 给定一个固定的光源方向
    glm::vec3 light_dir = glm::normalize(glm::vec3(2, 4, 1));

    occluder_program->ApplyVector3(glm::value_ptr(light_dir), UNIFORM_LIGHT_DIR_LOCATION);
}

void InstanceApp::PreRenderFrame()
{
}

void InstanceApp::PostRenderFrame()
{
    App::PostRenderFrame();
}

#define Z_NEAR 1.0f
#define Z_FAR 500.0f

void InstanceApp::UpdateCamera(float rotation_y, float rotation_x, uint32_t viewport_width, uint32_t viewport_height)
{
    // 在此计算 view-projection 矩阵
    float radians_y = 2.0f * glm::pi<float>() * rotation_y;
    float radians_x = 2.0f * glm::pi<float>() * rotation_x;

    glm::mat4 identity_mat = glm::mat4(1.0f);
    glm::mat4 rotation_matrix_y = glm::rotate(identity_mat, radians_y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotation_matrix_x = glm::rotate(identity_mat, radians_x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 camera_dir = glm::vec3(rotation_matrix_y * rotation_matrix_x * glm::vec4(0, 0, -1, 1));
    glm::vec3 camera_position = glm::vec3(0, 2, 0);

    auto view_matrix_ = glm::lookAt(camera_position, camera_position + camera_dir, glm::vec3(0, 1, 0));
    auto projection_matrix_ = glm::perspective(glm::radians(60.0f), float(viewport_width) / viewport_height, Z_NEAR, Z_FAR);
    glm::mat4 view_projection = projection_matrix_ * view_matrix_;

    auto vp_mat_ptr = glm::value_ptr(view_projection);
    occluder_program->ApplyMatrix(vp_mat_ptr, UNIFORM_MVP_LOCATION);
}

void InstanceApp::RenderScene()
{

    GL_CHECK_SIMPLE(glClearColor(0.35f, 0.02f, 0.02f, 0.05f));
    

    // Enable depth testing and culling.
    GL_CHECK_SIMPLE(glEnable(GL_DEPTH_TEST));
    GL_CHECK_SIMPLE(glEnable(GL_CULL_FACE));

    GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK_SIMPLE(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    GL_CHECK_SIMPLE(glViewport(0, 0, window_width_, window_height_));

    // Render occluder boxes.
    occluder_program->Use();//GL_CHECK_SIMPLE(glUseProgram(occluder_program));
    occluder_program->ApplyFloat3(1.2f, 0.6f, 0.6f, UNIFORM_COLOR_LOCATION);//GL_CHECK_SIMPLE(glProgramUniform3f(occluder_program, UNIFORM_COLOR_LOCATION, 1.2f, 0.6f, 0.6f));
    GL_CHECK_SIMPLE(glBindVertexArray(box->GetVertexArray()));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, occluder_instances_buffer));
    GL_CHECK_SIMPLE(glEnableVertexAttribArray(3));
    GL_CHECK_SIMPLE(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0));
    GL_CHECK_SIMPLE(glVertexAttribDivisor(3, 1));
    GL_CHECK_SIMPLE(glDrawElementsInstanced(GL_TRIANGLES, box->GetNumElements(), GL_UNSIGNED_SHORT, 0, num_occluder_instances));
}
