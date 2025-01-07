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
#include "hiz_culling.h"
#include "hiz_culling_no_lod.h"

// 用来描述每个小球实例
struct SphereInstance
{
    glm::vec4 position;
    glm::vec4 velocity;
};

static const unsigned VerticesPerCircle[] =
{
    SPHERE_VERT_PER_CIRC_LOD0,
    SPHERE_VERT_PER_CIRC_LOD1,
    SPHERE_VERT_PER_CIRC_LOD2,
    SPHERE_VERT_PER_CIRC_LOD3,
};

HizOcclusionCullingApp::HizOcclusionCullingApp()
{
    enable_culling_ = true;
    camera_rotation_y_ = 0.0f;
    camera_rotation_x_ = 0.0f;
    num_render_sphere_instances_ = SPHERE_INSTANCES;
    physics_speed_ = 1.0f;
    show_redundant_ = false;
    use_gui_ = false;
}


void HizOcclusionCullingApp::InitScene()
{
    this->InitMainCamera();
    this->InitLights();
    this->InitMaterials();

    // 因为model要使用到shader，所以务必要保证
    // shader在Model前完成初始化
    this->InitShaders();
    this->InitModel();
    this->InitFont();

    // 初始化两种拣选方法，带LOD和不带LOD的
    culling_implementations_.push_back(new HiZCulling);
    culling_implementations_.push_back(new HiZCullingNoLOD);
    culling_implementation_index_ = CullHiZ;

    set_show_redundant(true); // 显示冗余
    set_culling_method(CullingMethod::CullHiZ);
}

void HizOcclusionCullingApp::RenderGUI()
{
    if (!use_gui_)
        return;

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
    KGL_SAFE_DELETE(box_);

    for (uint32_t i = 0; i < SPHERE_LODS; i++)
    {
        KGL_SAFE_DELETE(sphere_[i]);
    }

    for (uint32_t i = 0; i < culling_implementations_.size(); i++)
    {
        KGL_SAFE_DELETE(culling_implementations_[i]);
    }

    culling_implementations_.clear();

    KGL_SAFE_DELETE(physics_program_);

    KGL_SAFE_DELETE(sphere_instances_buffer_);
    KGL_SAFE_DELETE(occluder_program_);
    KGL_SAFE_DELETE(quad_program_);
    KGL_SAFE_DELETE(physics_program_);

    GL_CHECK_SIMPLE(glDeleteBuffers(INDIRECT_BUFFERS, indirect_.buffer));
    GL_CHECK_SIMPLE(glDeleteBuffers(SPHERE_LODS, indirect_.instance_buffer));
}

void HizOcclusionCullingApp::InitShaders()
{
    physics_program_ = new kgl::ComputeShaderProgram();
    GL_CHECK_SIMPLE(physics_program_->CreateFromSingleFile("resources/shader/060_physics_cs.glsl"));

    occluder_program_ = new kgl::GPUProgram();
    GL_CHECK_SIMPLE(occluder_program_->CreateFromFile("resources/shader/060_scene_vs.glsl", "resources/shader/060_scene_fs.glsl", nullptr));

    sphere_program_ = new kgl::GPUProgram();
    GL_CHECK_SIMPLE(sphere_program_->CreateFromFile("resources/shader/060_scene_sphere_vs.glsl", "resources/shader/060_scene_sphere_fs.glsl", nullptr));

    quad_program_ = new kgl::GPUProgram();
    GL_CHECK_SIMPLE(quad_program_->CreateFromFile("resources/shader/060_quad_vs.glsl", "resources/shader/060_quad_fs.glsl", nullptr));
}

void HizOcclusionCullingApp::InitModel()
{
    quad_ = new GLDrawable();

    // 创建遮挡盒子
    AABB aabb = { glm::vec4(-1.0f, 0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 8.0f, 1.0f, 0.0f) };
    Mesh box_mesh = Mesh::CreateBoxMesh(aabb);
    box_ = new GLDrawable(box_mesh);

    // 根据不同的LOD值创建出不同精度的小球
    for (uint32_t i = 0; i < SPHERE_LODS; i++)
    {
        sphere_[i] = new GLDrawable(Mesh::CreateSphereMesh(1.0f, glm::vec3(0, 0, 0), VerticesPerCircle[i]));
    }

    // 将遮挡物几何体展开在 XZ 平面上的网格上。跳过世界坐标系原点，因为要把相机放在那里。
    // 多个遮挡物实例对象的坐标
    std::vector<glm::vec4> occluder_instances;

    for (uint32_t z = 0; z < 13; z++)
    {
        for (uint32_t x = 0; x < 13; x++)
        {
            if (z >= 5 && z <= 7 && x >= 5 && x <= 7)
            {
                continue;
            }

            occluder_instances.push_back(glm::vec4(3.0f) * glm::vec4(x - 6, 0, z - 6, 0));
        }
    }

    // 调整遮挡物实例坐标在数组中的排序，粗略的从前到后的排序以获得更理想的渲染顺序。
    std::sort(occluder_instances.begin(), occluder_instances.end(),
        [](const glm::vec4& a, const glm::vec4& b)
        {
            return glm::dot(a, a) < glm::dot(b, b);
        });

    num_occluder_instances = occluder_instances.size();

    // 把这一系列的遮挡物实例坐标上传到GPU的shader uniform 变量中
    occluder_instances_buffer_ = new kgl::ShaderBuffer(GL_UNIFORM_BUFFER);
    occluder_instances_buffer_->BindBufferAndUploadData(occluder_instances.size() * sizeof(glm::vec4), occluder_instances.data(), GL_STATIC_DRAW);

    // 放置具有不同位置和速度的球体。W分量包含球体半径，它是随机的。
    std::vector<SphereInstance> sphere_instances;
    glm::vec3 tmp_n;

    for (uint32_t x = 0; x < SPHERE_INSTANCES_X; x++)
    {
        for (uint32_t y = 0; y < SPHERE_INSTANCES_Y; y++)
        {
            for (uint32_t z = 0; z < SPHERE_INSTANCES_Z; z++)
            {
                SphereInstance instance;
                instance.position.x = x - 11.35f;
                instance.position.y = y * 0.10f + 0.5f;
                instance.position.z = z - 11.45f;
                instance.position.w = SPHERE_RADIUS * (1.0f - 0.5f * rand() / RAND_MAX);

                tmp_n.x = x - 11.35f;
                tmp_n.y = x - 0.5f * y - 11.55f;
                tmp_n.z = x - z - 11.25f;
                tmp_n = glm::normalize(tmp_n);

                instance.velocity.x = 4.0f * tmp_n.x;
                instance.velocity.y = 4.0f * tmp_n.y;
                instance.velocity.z = 4.0f * tmp_n.z;
                instance.velocity.w = 0.0f;

                sphere_instances.push_back(instance);
            }
        }
    }

    /*
    SSBO 与统一缓冲区对象非常相似。着色器存储块由接口块 (GLSL)定义，其定义方式与统一块几乎相同。
    存储 SSBO 的缓冲区对象绑定到 SSBO 绑定点，就像统一的缓冲区对象绑定到 UBO 绑定点一样。等等。
    
    它们之间的主要区别是：

    SSBO 可以大得多。OpenGL 规范保证 UBO 的大小最大可达 16KB（实现允许它们更大）。规范保证 SSBO 的大小最大可达 128 MB。大多数实现将允许您分配不超过 GPU 内存限制的大小。
    SSBO 是可写的，甚至是原子性的；UBO 是统一的。SSBO 的读写使用不连贯的内存访问，因此它们需要适当的屏障，就像图像加载存储操作一样。
    SSBO 可以具有可变存储，直至为该特定缓冲区绑定的缓冲区范围；UBO 必须具有特定的固定存储大小。这意味着您可以在 SSBO 中拥有任意长度的数组（更确切地说是在末尾）。可以在
    着色器中使用无界数组变量上的长度函数在运行时查询基于缓冲区绑定范围的数组的实际大小。
    
    在其他条件相同的情况下，SSBO 访问可能会比 UBO 访问慢。SSBO 通常像缓冲区纹理一样被访问，而 UBO 数据则通过内部着色器可访问的内存读取来访问。至少，UBO 不会比 SSBO 慢。
    从功能上讲，当通过图像加载存储 (Image Load Store)访问时，SSBO 可以被认为是缓冲区纹理 (Buffer Textures)的更好的接口。
    */

    // 上传存储了小球属性的SSBO
    sphere_instances_buffer_ = new kgl::ShaderBuffer(GL_SHADER_STORAGE_BUFFER);
    sphere_instances_buffer_->BindBufferAndUploadData(sphere_instances.size() * sizeof(SphereInstance), sphere_instances.data(), GL_STATIC_DRAW);
   
    // 初始化用来存储在进行剔除之后的小球实例的缓冲区。缓冲区必须至少与球体实例缓冲区一样大（假设所有的小球都可见性）。
    GL_CHECK_SIMPLE(glGenBuffers(SPHERE_LODS, indirect_.instance_buffer));

    for (uint32_t i = 0; i < SPHERE_LODS; i++)
    {
        GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, indirect_.instance_buffer[i]));
        GL_CHECK_SIMPLE(glBufferData(GL_ARRAY_BUFFER, sphere_instances.size() * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY));
    }

    // Setup occluder geometry for each implementation.
    std::vector<glm::vec4> occluder_positions;
    std::vector<uint32_t> occluder_indices;

    BakeOccluderGeometry(occluder_positions, occluder_indices, box_mesh, occluder_instances);

    // 烘焙好遮挡体的顶点缓冲值和索引缓冲值之后，交给culling处理模块上传到GPU
    for (uint32_t i = 0; i < culling_implementations_.size(); i++)
    {
        culling_implementations_[i]->SetupOccluderGeometry(occluder_positions, occluder_indices);
    }

    // 初始化间接绘制缓冲区。使用它们的环形缓冲区，因为我们可能希望读回旧结果来监控剔除性能，而不会拖延管道。
    GL_CHECK_SIMPLE(glGenBuffers(INDIRECT_BUFFERS, indirect_.buffer));

    for (uint32_t i = 0; i < SPHERE_LODS; i++)
    {
        GL_CHECK_SIMPLE(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_.buffer[i]));
        GL_CHECK_SIMPLE(glBufferData(GL_DRAW_INDIRECT_BUFFER, SPHERE_LODS * sizeof(IndirectCommand), NULL, GL_DYNAMIC_COPY));
    }
    
    indirect_.buffer_index = 0;
}

void HizOcclusionCullingApp::MoveCamera(float delta_x, float delta_y)
{
    // 角度从 [0, 1] => [0, 2pi] 弧度映射。
    camera_rotation_y_ -= delta_x * 0.25f;
    camera_rotation_x_ += delta_y * 0.15f;
    camera_rotation_x_ = std::clamp(camera_rotation_x_, -0.20f, 0.20f);
    camera_rotation_y_ -= floor(camera_rotation_y_);
}

void HizOcclusionCullingApp::UpdateCamera(float rotation_y, float rotation_x, uint32_t viewport_width, uint32_t viewport_height)
{
    // 在此计算view-projection矩阵
    float radians_y = 2.0f * glm::pi<float>() * rotation_y;
    float radians_x = 2.0f * glm::pi<float>() * rotation_x;

    glm::mat4 identity_mat = glm::mat4(1.0f);

    glm::mat4 rotation_matrix_y = glm::rotate(identity_mat, radians_y, glm::vec3(0.0f, 1.0f, 0.0f));//mat_rotate_y(radians_y);
    glm::mat4 rotation_matrix_x = glm::rotate(identity_mat, radians_x, glm::vec3(1.0f, 0.0f, 0.0f));//mat_rotate_x(radians_x);
    glm::vec3 camera_dir = glm::vec3(rotation_matrix_y * rotation_matrix_x * glm::vec4(0, 0, -1, 1));

    glm::vec3 camera_position = glm::vec3(0, 2, 0);

    view_matrix_ = glm::lookAt(camera_position, camera_position + camera_dir, glm::vec3(0, 1, 0));
    projection_matrix_ = glm::perspective(glm::radians(60.0f), float(viewport_width) / viewport_height, Z_NEAR, Z_FAR);//mat_perspective_fov(60.0f, float(viewport_width) / viewport_height, Z_NEAR, Z_FAR);
    glm::mat4 view_projection = projection_matrix_ * view_matrix_;

    auto vp_mat_ptr = glm::value_ptr(view_projection);
    occluder_program_->ApplyMatrix(vp_mat_ptr, UNIFORM_MVP_LOCATION);
    sphere_program_->ApplyMatrix(vp_mat_ptr, UNIFORM_MVP_LOCATION);

    //GL_CHECK_SIMPLE(glProgramUniformMatrix4fv(occluder_program, UNIFORM_MVP_LOCATION, 1, GL_FALSE, value_ptr(view_projection)));
    //GL_CHECK_SIMPLE(glProgramUniformMatrix4fv(sphere_program, UNIFORM_MVP_LOCATION, 1, GL_FALSE, value_ptr(view_projection)));
}

void HizOcclusionCullingApp::set_culling_method(CullingMethod method)
{
    if (method == CullNone)
    {
        enable_culling_ = false;
        culling_implementation_index_ = 0;
    }
    else
    {
        enable_culling_ = true;
        culling_implementation_index_ = static_cast<uint32_t>(method);
    }
}

void HizOcclusionCullingApp::ApplyPhysics(float delta_time)
{
    if (physics_speed_ <= 0.0f)
    {
        return;
    }

    // Do physics on the spheres, in a compute shader.
    physics_program_->Use();//GL_CHECK_SIMPLE(glUseProgram(physics_program));
    sphere_instances_buffer_->BindBufferBase(0);//GL_CHECK_SIMPLE(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sphere_instances_buffer));
    physics_program_->ApplyUint(SPHERE_INSTANCES, 0);//GL_CHECK_SIMPLE(glProgramUniform1ui(physics_program, 0, SPHERE_INSTANCES));
    physics_program_->ApplyFloat(physics_speed_ * delta_time, 1);//GL_CHECK_SIMPLE(glProgramUniform1f(physics_program, 1, physics_speed * delta_time));
    physics_program_->Dispatch((SPHERE_INSTANCES + PHYSICS_GROUP_SIZE - 1) / PHYSICS_GROUP_SIZE, 1, 1);//GL_CHECK_SIMPLE(glDispatchCompute((SPHERE_INSTANCES + PHYSICS_GROUP_SIZE - 1) / PHYSICS_GROUP_SIZE, 1, 1));

    // We don't need data here until bounding box check, so we can let rasterizer and physics run in parallel, avoiding memory barrier here.
}

void HizOcclusionCullingApp::PreRenderFrame()
{
    //App::PreRenderFrame();
}


void HizOcclusionCullingApp::PostRenderFrame()
{
    App::PostRenderFrame();
}

void HizOcclusionCullingApp::UpdateScene()
{
    // 更新摄像机
    UpdateCamera(camera_rotation_y_, camera_rotation_x_, window_width_, window_height_);

    // 给定一个固定的光源方向
    glm::vec3 light_dir = glm::normalize(glm::vec3(2, 4, 1));

    occluder_program_->ApplyVector3(glm::value_ptr(light_dir), UNIFORM_LIGHT_DIR_LOCATION);
    sphere_program_->ApplyVector3(glm::value_ptr(light_dir), UNIFORM_LIGHT_DIR_LOCATION);
    //GL_CHECK_SIMPLE(glProgramUniform3fv(occluder_program, UNIFORM_LIGHT_DIR_LOCATION, 1, value_ptr(light_dir)));
    //GL_CHECK_SIMPLE(glProgramUniform3fv(sphere_program, UNIFORM_LIGHT_DIR_LOCATION, 1, value_ptr(light_dir)));

    // 让小球运动起来，在compute shader中计算
    ApplyPhysics(frame_time_delta_);

    if (enable_culling_)
    {
        CullingInterface* culler = culling_implementations_[culling_implementation_index_];
        num_sphere_render_lods = culler->GetLodsCount();

        // Rasterize occluders to depth map and mipmap it.
        culler->SetViewProjectionMatrix(projection_matrix_, view_matrix_, glm::vec2(Z_NEAR, Z_FAR));
        culler->RasterizeOccluders();

        // We need physics results after this.
        GL_CHECK_SIMPLE(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

        IndirectCommand indirect_command[SPHERE_LODS];
        unsigned offsets[SPHERE_LODS];

        memset(indirect_command, 0, sizeof(indirect_command));

        for (unsigned i = 0; i < SPHERE_LODS; i++)
        {
            indirect_command[i].count = sphere_[i]->get_num_elements();
            offsets[i] = 4 + sizeof(IndirectCommand) * i;
        }

        // Clear out our indirect draw buffer.
        GL_CHECK_SIMPLE(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_.buffer[indirect_.buffer_index]));
        GL_CHECK_SIMPLE(glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(indirect_command), indirect_command, GL_STREAM_DRAW));

        // Test occluders and build indirect commands as well as per-instance buffers for every LOD.
        culler->TestBoundingBoxes(indirect_.buffer[indirect_.buffer_index], offsets, SPHERE_LODS,
            indirect_.instance_buffer, sphere_instances_buffer_,
            num_render_sphere_instances_);
    }
    else
    {
        // If we don't do culling, we need to make sure we call the memory barrier for physics.
        GL_CHECK_SIMPLE(glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT));
        num_sphere_render_lods = 1;
    }
}

void HizOcclusionCullingApp::render_spheres(glm::vec3 color_mod)
{
    if (enable_culling_)
    {
        GL_CHECK_SIMPLE(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_.buffer[indirect_.buffer_index]));

        for (unsigned i = 0; i < num_sphere_render_lods; i++)
        {
            // Use different colors for different LOD levels to easily spot them.
           /* GL_CHECK_SIMPLE(glProgramUniform3fv(sphere_program, UNIFORM_COLOR_LOCATION, 1,
                glm::value_ptr(color_mod * vec3(0.8f - 0.2f * i, 1.2f - 0.2f * i, 0.8f + 0.2f * i))));*/

            sphere_program_->ApplyVector3(glm::value_ptr(color_mod * glm::vec3(0.8f - 0.2f * i, 1.2f - 0.2f * i, 0.8f + 0.2f * i)), UNIFORM_COLOR_LOCATION);

            // Use different meshes for different LOD levels.
            GL_CHECK_SIMPLE(glBindVertexArray(sphere_[i]->get_vertex_array()));

            GL_CHECK_SIMPLE(glEnableVertexAttribArray(3));
            GL_CHECK_SIMPLE(glVertexAttribDivisor(3, 1));
            GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, indirect_.instance_buffer[i]));
            GL_CHECK_SIMPLE(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0));

            GL_CHECK_SIMPLE(glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT,
                reinterpret_cast<const void*>(i * sizeof(IndirectCommand))));
        }
    }
    else
    {
        // Unconditionally draw every instance of LOD0.
        sphere_program_->ApplyVector3(glm::value_ptr(color_mod * glm::vec3(0.8f, 1.2f, 0.8f)), UNIFORM_COLOR_LOCATION);

        // GL_CHECK_SIMPLE(glProgramUniform3fv(sphere_program, UNIFORM_COLOR_LOCATION, 1, value_ptr(color_mod * vec3(0.8f, 1.2f, 0.8f))));
        GL_CHECK_SIMPLE(glBindVertexArray(sphere_[0]->get_vertex_array()));
        GL_CHECK_SIMPLE(glEnableVertexAttribArray(3));
        GL_CHECK_SIMPLE(glVertexAttribDivisor(3, 1));
        sphere_instances_buffer_->Rebinding(GL_ARRAY_BUFFER);//GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, sphere_instances_buffer));
        GL_CHECK_SIMPLE(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4), 0));
        GL_CHECK_SIMPLE(glDrawElementsInstanced(GL_TRIANGLES, sphere_[0]->get_num_elements(), GL_UNSIGNED_SHORT, NULL, num_render_sphere_instances_));
    }
}

void HizOcclusionCullingApp::RenderScene()
{
    //uint32_t width, uint32_t height

    if (enable_culling_)
    {
        GL_CHECK_SIMPLE(glClearColor(0.02f, 0.02f, 0.35f, 0.05f));
    }
    else
    {
        GL_CHECK_SIMPLE(glClearColor(0.35f, 0.02f, 0.02f, 0.05f));
    }

    // 开启深度测试 开启面拣选
    GL_CHECK_SIMPLE(glEnable(GL_DEPTH_TEST));
    GL_CHECK_SIMPLE(glEnable(GL_CULL_FACE));

    // 绑定幀缓冲区，后续的绘制都回望幀缓冲区内写入
    GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK_SIMPLE(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    GL_CHECK_SIMPLE(glViewport(0, 0, window_width_, window_height_));

    // Render occluder boxes.
    occluder_program_->Use();//GL_CHECK_SIMPLE(glUseProgram(occluder_program));
    occluder_program_->ApplyFloat3(1.2f, 0.6f, 0.6f, UNIFORM_COLOR_LOCATION);//GL_CHECK_SIMPLE(glProgramUniform3f(occluder_program, UNIFORM_COLOR_LOCATION, 1.2f, 0.6f, 0.6f));

    GL_CHECK_SIMPLE(glBindVertexArray(box_->get_vertex_array()));
    occluder_instances_buffer_->Rebinding(GL_ARRAY_BUFFER);//GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, occluder_instances_buffer));

    GL_CHECK_SIMPLE(glEnableVertexAttribArray(3));
    GL_CHECK_SIMPLE(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0));
    GL_CHECK_SIMPLE(glVertexAttribDivisor(3, 1));
    GL_CHECK_SIMPLE(glDrawElementsInstanced(GL_TRIANGLES, box_->get_num_elements(), GL_UNSIGNED_SHORT, 0, num_occluder_instances));

    sphere_program_->Use();//GL_CHECK_SIMPLE(glUseProgram(sphere_program));

    if (show_redundant_)
    {
        // Draw false-positive meshes in a dark color.
        // False-positives will fail the depth test (pass with GL_GREATER).
        // We don't want to update the depth buffer, so the false-positives will be rendered in a "glitchy"
        // way due to the random ordering that occlusion culling introduces.
        GL_CHECK_SIMPLE(glDepthFunc(GL_GREATER));
        GL_CHECK_SIMPLE(glDepthMask(GL_FALSE));
        render_spheres(glm::vec3(0.25f));
        GL_CHECK_SIMPLE(glDepthMask(GL_TRUE));
        GL_CHECK_SIMPLE(glDepthFunc(GL_LESS));
    }

    render_spheres(glm::vec3(1.0f));

    if (enable_culling_)
    {
        render_depth_map();
    }

    // Restore viewport (for text rendering).
    //GL_CHECK_SIMPLE(glViewport(0, 0, width, height));
    GL_CHECK_SIMPLE(glViewport(0, 0, window_width_, window_height_));

    // Jump to next indirect draw buffer (ring buffer).
    indirect_.buffer_index = (indirect_.buffer_index + 1) % INDIRECT_BUFFERS;
}

void HizOcclusionCullingApp::render_depth_map()
{
    GL_CHECK_SIMPLE(glDisable(GL_DEPTH_TEST));
    quad_program_->Use();//GL_CHECK_SIMPLE(glUseProgram(quad_program));

    // Debug
    GL_CHECK_SIMPLE(glBindVertexArray(quad_->get_vertex_array()));

    unsigned offset_x = 0;

    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, culling_implementations_[culling_implementation_index_]->GetDepthTexture()));

    for (uint32_t lod = 0; lod <= DEPTH_SIZE_LOG2; lod++)
    {
        GL_CHECK_SIMPLE(glViewport(offset_x, 0, DEPTH_SIZE >> lod, DEPTH_SIZE >> lod));

        // Mipmapped filtering mode will ensure we draw correct miplevel.
        GL_CHECK_SIMPLE(glDrawElements(GL_TRIANGLES, quad_->get_num_elements(), GL_UNSIGNED_SHORT, 0));

        offset_x += DEPTH_SIZE >> lod;
    }
}

void HizOcclusionCullingApp::BakeOccluderGeometry(std::vector<glm::vec4>& occluder_positions, std::vector<uint32_t>& occluder_indices,const Mesh& box_mesh, const std::vector<glm::vec4>& instances_pos)
{
    uint32_t num_instances = instances_pos.size();
    uint32_t total_vertices = num_instances * box_mesh.vbo.size();
    occluder_positions.reserve(total_vertices);

    uint32_t total_indices = num_instances * box_mesh.ibo.size();
    occluder_indices.reserve(total_indices);

    //
    for (uint32_t i = 0; i < total_indices; i++)
    {
        uint32_t instance = i / box_mesh.ibo.size();
        uint32_t index = i % box_mesh.ibo.size();

        uint32_t baked_index = box_mesh.ibo[index] + instance * box_mesh.vbo.size();
        occluder_indices.push_back(baked_index);
    }

    // Bake the vertex buffer.
    for (uint32_t i = 0; i < total_vertices; i++)
    {
        uint32_t instance = i / box_mesh.vbo.size();
        uint32_t index = i % box_mesh.vbo.size();

        // 根据传递进来的instances_pos数组，更新遮挡物的位置 
        glm::vec4 pos = instances_pos[instance] + glm::vec4(box_mesh.vbo[index].position, 1.0f);
        occluder_positions.push_back(pos);
    }
}
