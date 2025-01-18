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
#include "../klib/kgl_error.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_string_convertor.h"
#include "constants.h"
#include "culling.h"
#include "hiz_culling.h"
#include "hiz_culling_no_lod.h"
#include "hiz_occlusion_culling_app.h"

static const uint32_t verts_per_circ[] =
{
    SPHERE_VERT_PER_CIRC_LOD0,
    SPHERE_VERT_PER_CIRC_LOD1,
    SPHERE_VERT_PER_CIRC_LOD2,
    SPHERE_VERT_PER_CIRC_LOD3,
};

// 用来描述每个小球实例
struct SphereInstance
{
    glm::vec4 position;
    glm::vec4 velocity;
};

HizOcclusionCullingApp::HizOcclusionCullingApp()
{
    camera_rotation_y_ = 0.0f;
    camera_rotation_x_ = 0.0f;

    num_render_sphere_instances_ = SPHERE_INSTANCES;
    physics_speed_ = 1.0f;

    show_redundant_ = false;
}

HizOcclusionCullingApp::~HizOcclusionCullingApp()
{
}

void HizOcclusionCullingApp::MoveCamera(float delta_x, float delta_y)
{
    camera_rotation_y_ -= delta_x * 0.25f;
    camera_rotation_x_ += delta_y * 0.15f;
    camera_rotation_x_ = std::clamp(camera_rotation_x_, -0.20f, 0.20f);
    camera_rotation_y_ -= std::floor(camera_rotation_y_);
}


// Bake our instanced occluder geometry into a single vertex buffer and index buffer.
// Makes the implementation somewhat more generic and simple.
void HizOcclusionCullingApp::BakeOccluderGeometry(std::vector<glm::vec4>& occluder_positions, std::vector<uint32_t>& occluder_indices,
    const Mesh& box_mesh, const glm::vec4* instances, uint32_t num_instances)
{
    uint32_t total_vertices = num_instances * box_mesh.vbo.size();
    occluder_positions.reserve(total_vertices);

    uint32_t total_indices = num_instances * box_mesh.ibo.size();
    occluder_indices.reserve(total_indices);

    for (uint32_t i = 0; i < total_indices; i++)
    {
        uint32_t instance = i / box_mesh.ibo.size();
        uint32_t index = i % box_mesh.ibo.size();

        uint32_t baked_index = box_mesh.ibo[index] + instance * box_mesh.vbo.size();
        occluder_indices.push_back(baked_index);
    }

    for (uint32_t i = 0; i < total_vertices; i++)
    {
        uint32_t instance = i / box_mesh.vbo.size();
        uint32_t index = i % box_mesh.vbo.size();

        glm::vec4 pos = instances[instance] + glm::vec4(box_mesh.vbo[index].position, 1.0f);
        occluder_positions.push_back(pos);
    }
}


struct OccluderSorter
{
    bool operator()(const glm::vec4& a, const glm::vec4& b)
    {
        return glm::dot(a, a) < glm::dot(b, b);
    }
};

void HizOcclusionCullingApp::InitInstances()
{
    AABB aabb = { glm::vec4(-1.0f, 0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 8.0f, 1.0f, 0.0f) };
    Mesh box_mesh = create_box_mesh(aabb);
    box_ = new GLDrawable(box_mesh);

    for (uint32_t i = 0; i < SPHERE_LODS; i++)
    {
        sphere_[i] = new GLDrawable(create_sphere_mesh(1.0f, glm::vec3(0, 0, 0), verts_per_circ[i]));
    }


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

    sort(occluder_instances.begin(), occluder_instances.end(), OccluderSorter());

    num_occluder_instances_ = occluder_instances.size();


    GL_CHECK_SIMPLE(glGenBuffers(1, &occluder_instances_buffer_));
    GL_CHECK_SIMPLE(glBindBuffer(GL_UNIFORM_BUFFER, occluder_instances_buffer_));
    GL_CHECK_SIMPLE(glBufferData(GL_UNIFORM_BUFFER, occluder_instances.size() * sizeof(glm::vec4), &occluder_instances[0], GL_STATIC_DRAW));

    std::vector<SphereInstance> sphere_instances;
    glm::vec3 tmp_n;

    for (int x = 0; x < SPHERE_INSTANCES_X; x++)
    {
        for (int y = 0; y < SPHERE_INSTANCES_Y; y++)
        {
            for (int z = 0; z < SPHERE_INSTANCES_Z; z++)
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


    GL_CHECK_SIMPLE(glGenBuffers(1, &sphere_instances_buffer_));
    GL_CHECK_SIMPLE(glBindBuffer(GL_SHADER_STORAGE_BUFFER, sphere_instances_buffer_));
    GL_CHECK_SIMPLE(glBufferData(GL_SHADER_STORAGE_BUFFER, sphere_instances.size() * sizeof(SphereInstance), &sphere_instances[0], GL_STATIC_DRAW));

    GL_CHECK_SIMPLE(glGenBuffers(SPHERE_LODS, indirect_.instance_buffer));

    for (uint32_t i = 0; i < SPHERE_LODS; i++)
    {
        GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, indirect_.instance_buffer[i]));
        GL_CHECK_SIMPLE(glBufferData(GL_ARRAY_BUFFER, sphere_instances.size() * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY));
    }

    std::vector<glm::vec4> occluder_positions;
    std::vector<uint32_t> occluder_indices;
    BakeOccluderGeometry(occluder_positions, occluder_indices, box_mesh, &occluder_instances[0], occluder_instances.size());

    for (uint32_t i = 0; i < culling_implementations_.size(); i++)
    {
        culling_implementations_[i]->SetupOccluderGeometry(occluder_positions, occluder_indices);
    }

    GL_CHECK_SIMPLE(glGenBuffers(INDIRECT_BUFFERS, indirect_.buffer));

    for (uint32_t i = 0; i < SPHERE_LODS; i++)
    {
        GL_CHECK_SIMPLE(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_.buffer[i]));
        GL_CHECK_SIMPLE(glBufferData(GL_DRAW_INDIRECT_BUFFER, SPHERE_LODS * sizeof(IndirectCommand), NULL, GL_DYNAMIC_COPY));
    }
    indirect_.buffer_index = 0;
}

void HizOcclusionCullingApp::UpdateCamera(float rotation_y, float rotation_x, uint32_t viewport_width, uint32_t viewport_height)
{
    float radians_y = 2.0f * glm::pi<float>() * rotation_y;
    float radians_x = 2.0f * glm::pi<float>() * rotation_x;

    glm::mat4 identity_mat = glm::mat4(1.0f);
    glm::mat4 rotation_matrix_y = glm::rotate(identity_mat, radians_y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotation_matrix_x = glm::rotate(identity_mat, radians_x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 camera_dir = glm::vec3(rotation_matrix_y * rotation_matrix_x * glm::vec4(0, 0, -1, 1));

    glm::vec3 camera_position = glm::vec3(0, 2, 0);

    view_matrix_ = glm::lookAt(camera_position, camera_position + camera_dir, glm::vec3(0, 1, 0));
    projection_matrix_ = glm::perspective(glm::radians(60.0f), float(viewport_width) / viewport_height, Z_NEAR, Z_FAR);
    glm::mat4 view_projection = projection_matrix_ * view_matrix_;

    auto vp_mat_ptr = glm::value_ptr(view_projection);
    GL_CHECK_SIMPLE(glProgramUniformMatrix4fv(occluder_program_, UNIFORM_MVP_LOCATION, 1, GL_FALSE, vp_mat_ptr));
    GL_CHECK_SIMPLE(glProgramUniformMatrix4fv(sphere_program_, UNIFORM_MVP_LOCATION, 1, GL_FALSE, vp_mat_ptr));
}

void HizOcclusionCullingApp::SetCullingMethod(CullingMethod method)
{
    if (method == CullNone)
    {
        enable_culling_ = false;
        culling_implementation_index_ = 2;
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
    GL_CHECK_SIMPLE(glUseProgram(physics_program_));
    GL_CHECK_SIMPLE(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sphere_instances_buffer_));
    GL_CHECK_SIMPLE(glProgramUniform1ui(physics_program_, 0, SPHERE_INSTANCES));
    GL_CHECK_SIMPLE(glProgramUniform1f(physics_program_, 1, physics_speed_ * delta_time));
    GL_CHECK_SIMPLE(glDispatchCompute((SPHERE_INSTANCES + PHYSICS_GROUP_SIZE - 1) / PHYSICS_GROUP_SIZE, 1, 1));

    // We don't need data here until bounding box check, so we can let rasterizer and physics run in parallel, avoiding memory barrier here.
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

    set_show_redundant(true); // 显示冗余
    SetCullingMethod(CullingMethod::CullNone);
}

void HizOcclusionCullingApp::RenderGUI()
{
    if (!use_gui_)
        return;

    static float f = 0.0f;
    static int counter = 0;

    if (ImGui::Begin("006 Hiz Occlusion Culling"))
    {
        ImGui::Checkbox("显示被遮挡的小球", &show_redundant_);

        // 定义拣选模式选项列表
        const char* cull_mode_names[] = { "Cull HiZ", "Cull HiZ NoLOD", "Cull None" };

        // 创建一个COMBO组件
        if (ImGui::BeginCombo("拣选模式", cull_mode_names[culling_implementation_index_]))
        {
            // 遍历所有选项
            for (int i = 0; i < IM_ARRAYSIZE(cull_mode_names); i++)
            {
                // 检查当前选项是否被选中
                bool isSelected = (culling_implementation_index_ == i);

                // 创建一个可选择项
                if (ImGui::Selectable(cull_mode_names[i], isSelected))
                {
                    // 如果用户选择了这个选项，更新selectedIndex
                    culling_implementation_index_ = i;

                    if (culling_implementation_index_ == 2)
                        SetCullingMethod(CullingMethod::CullNone);
                    else
                        SetCullingMethod(static_cast<CullingMethod>(culling_implementation_index_));
                }

                // 如果当前选项被选中，设置它为默认焦点
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            // 结束COMBO组件
            ImGui::EndCombo();
        }

        ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
    }
    ImGui::End();
}

void HizOcclusionCullingApp::InitModel()
{
    culling_implementations_.push_back(new HiZCulling);
    culling_implementations_.push_back(new HiZCullingNoLOD);
    culling_implementation_index_ = CullHiZ;
    enable_culling_ = true;

    quad_ = new GLDrawable();

    InitInstances();
}

void HizOcclusionCullingApp::InitShaders()
{
    occluder_program_ = common_compile_shader_from_file("resources/shader/060_scene_vs.glsl", "resources/shader/060_scene_fs.glsl");
    sphere_program_ = common_compile_shader_from_file("resources/shader/060_scene_sphere_vs.glsl", "resources/shader/060_scene_sphere_fs.glsl");
    quad_program_ = common_compile_shader_from_file("resources/shader/060_quad_vs.glsl", "resources/shader/060_quad_fs.glsl");
    physics_program_ = common_compile_compute_shader_from_file("resources/shader/060_physics_cs.glsl");

    culling_implementations_.push_back(new HiZCulling);
    culling_implementations_.push_back(new HiZCullingNoLOD);
    culling_implementation_index_ = CullHiZ;
}

void HizOcclusionCullingApp::UpdateScene()
{
    // 更新摄像机
    MoveCamera(frame_time_delta_ * 0.1f, 0.0f);
    UpdateCamera(camera_rotation_y_, camera_rotation_x_, window_width_, window_height_);

    // 给定一个固定的光源方向
    glm::vec3 light_dir = glm::normalize(glm::vec3(2, 4, 1));
    GL_CHECK_SIMPLE(glProgramUniform3fv(occluder_program_, UNIFORM_LIGHT_DIR_LOCATION, 1, value_ptr(light_dir)));
    GL_CHECK_SIMPLE(glProgramUniform3fv(sphere_program_, UNIFORM_LIGHT_DIR_LOCATION, 1, value_ptr(light_dir)));

    // 让小球运动起来，在compute shader 中计算
    ApplyPhysics(frame_time_delta_);

    if (enable_culling_)
    {
        Culling* culler = culling_implementations_[culling_implementation_index_];
        num_sphere_render_lods_ = culler->GetLodNum();

        // 把遮挡物光栅化到深度图上，并且形成mipmap链
        culler->SetViewProjectionMatrix(projection_matrix_, view_matrix_, glm::vec2(Z_NEAR, Z_FAR));
        culler->RasterizeOccluders();

        // 注意这里需要CPU-GPU之间的同步
        GL_CHECK_SIMPLE(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

        IndirectCommand indirect_command[SPHERE_LODS];
        uint32_t offsets[SPHERE_LODS];

        memset(indirect_command, 0, sizeof(indirect_command));

        for (uint32_t i = 0; i < SPHERE_LODS; i++)
        {
            indirect_command[i].count = sphere_[i]->get_num_elements();
            offsets[i] = 4 + sizeof(IndirectCommand) * i;
        }

        GL_CHECK_SIMPLE(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_.buffer[indirect_.buffer_index]));
        GL_CHECK_SIMPLE(glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(indirect_command), indirect_command, GL_STREAM_DRAW));

        // 测试遮挡物，并为每个 LOD 构建间接命令以及逐实例缓冲区(per-instance buffers)。
        // 即是说，在这里判断小球是否被挡住，应该不应该绘制
        culler->TestBoundingBoxes(indirect_.buffer[indirect_.buffer_index], offsets, SPHERE_LODS,
            indirect_.instance_buffer, sphere_instances_buffer_,num_render_sphere_instances_);
    }
    else
    {
        // If we don't do culling, we need to make sure we call the memory barrier for physics.
        GL_CHECK_SIMPLE(glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT));
        num_sphere_render_lods_ = 1;
    }
}


void HizOcclusionCullingApp::RenderSpheres(const glm::vec3& color_mod)
{
    if (enable_culling_)
    {
        // 间接绘制命令，首先绑定间接绘制命令用到的buffer， GL_DRAW_INDIRECT_BUFFER 是 OpenGL 中的一个
       // 缓冲区绑定目标(Buffer Binding Target)，用于存储间接绘制命令（Indirect Drawing Commands）
       // 在InitModel函数里面已经执行过一次对indirect buffer 的 glBufferData 操作了
        GL_CHECK_SIMPLE(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_.buffer[indirect_.buffer_index]));

        for (uint32_t i = 0; i < num_sphere_render_lods_; i++)
        {
            // 使用不同的LOD级别
            GL_CHECK_SIMPLE(glProgramUniform3fv(sphere_program_, UNIFORM_COLOR_LOCATION, 1,
                value_ptr(color_mod * glm::vec3(0.8f - 0.2f * i, 1.2f - 0.2f * i, 0.8f + 0.2f * i))));

            // 使用不同的LOD级别的mesh
            GL_CHECK_SIMPLE(glBindVertexArray(sphere_[i]->get_vertex_array()));

            GL_CHECK_SIMPLE(glEnableVertexAttribArray(3));
            GL_CHECK_SIMPLE(glVertexAttribDivisor(3, 1));

            // GL_ARRAY_BUFFER 是 OpenGL 中的一个缓冲区绑定目标（Buffer Binding Target），
            // 用于存储顶点数据（如顶点位置、颜色、法线、纹理坐标等）。它是 OpenGL 中最常用
            // 的缓冲区类型之一，通常与顶点缓冲对象（VBO，Vertex Buffer Object）一起使用。
            GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, indirect_.instance_buffer[i]));
            GL_CHECK_SIMPLE(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0));

            // 是 OpenGL 中的一个高级绘制函数，用于执行间接索引绘制（Indirect Indexed Drawing）。
            // 它允许将绘制命令（如顶点数量、实例数量等）存储在 GPU 缓冲区中，而不是通过 CPU 直接调用绘制函数。
            // 这种方式可以减少 CPU 和 GPU 之间的通信开销，特别适合需要频繁更新绘制命令的场景（如实例化渲染、粒子系统等）。
            GL_CHECK_SIMPLE(glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT,reinterpret_cast<const void*>(i * sizeof(IndirectCommand))));
        }
    }
    else
    {
        // 不拣选的话，就只绘制LOD0的实例
        GL_CHECK_SIMPLE(glProgramUniform3fv(sphere_program_, UNIFORM_COLOR_LOCATION, 1, value_ptr(color_mod * glm::vec3(0.8f, 1.2f, 0.8f))));
        GL_CHECK_SIMPLE(glBindVertexArray(sphere_[0]->get_vertex_array()));
        GL_CHECK_SIMPLE(glEnableVertexAttribArray(3));
        GL_CHECK_SIMPLE(glVertexAttribDivisor(3, 1));
        GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, sphere_instances_buffer_));
        GL_CHECK_SIMPLE(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4), 0));
        GL_CHECK_SIMPLE(glDrawElementsInstanced(GL_TRIANGLES, sphere_[0]->get_num_elements(),GL_UNSIGNED_SHORT, NULL, num_render_sphere_instances_));
    }
}

void HizOcclusionCullingApp::PreRenderFrame()
{
}

void HizOcclusionCullingApp::PostRenderFrame()
{
    App::PostRenderFrame();
}

void HizOcclusionCullingApp::RenderScene()
{
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

    // 绑定幀缓冲区，后续的绘制都会往幀缓冲区内写入
    GL_CHECK_SIMPLE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK_SIMPLE(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    GL_CHECK_SIMPLE(glViewport(0, 0, window_width_, window_height_));

    // 绑定幀缓冲区，后续的绘制都会往幀缓冲区内写入
    GL_CHECK_SIMPLE(glUseProgram(occluder_program_));
    GL_CHECK_SIMPLE(glProgramUniform3f(occluder_program_, UNIFORM_COLOR_LOCATION, 1.2f, 0.6f, 0.6f));

    // 绑定一个BOX的vertex array object，使之成为当前上下文中的活动VAO
    // 后面的glEnableVertexAttribArray glVertexAttribPointer glVertexAttribDivisor对应的指令操作
    // 都会对应记录于本VAO中
    GL_CHECK_SIMPLE(glBindVertexArray(box_->get_vertex_array()));

    // GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, occluder_instances_buffer));
    // glEnableVertexAttribArray 是 OpenGL 中的一个重要函数，用于启用指定的顶点属性数组。
    // 它的作用是告诉 OpenGL，某个顶点属性（例如位置、颜色、法线等）将从顶点缓冲对象（VBO）中读取数据，
    // 而不是使用默认的常量值。这函数的唯一输入参数为要启用的顶点属性的索引。这个索引对应于顶点着色器中 
    // layout(location = index) 指定的位置。
    // 在060_scene_vs.glsl中，属性顶点的instance属性，其layout值为3，如下代码语句
    // layout(location = 3) in vec3 aOffset; // Instanced array
    GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, occluder_instances_buffer_));

    GL_CHECK_SIMPLE(glEnableVertexAttribArray(3));

    // void glVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,const GLvoid * pointer);
    // 参数index:         指定要修改的顶点属性的索引值，
    // 参数size:          指定每个顶点属性的组件数量。必须为1、2、3或者4。初始值为4。（如position是由3个（x,y,z）组成，而颜色是4个（r,g,b,a)
    // 参数GLenum:        指定数组中每个组件的数据类型。可用的符号常量有GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,GL_UNSIGNED_SHORT, GL_FIXED, 和 GL_FLOAT，初始值为GL_FLOAT。
    // 参数normalized:    指定当被访问时，固定点数据值是否应该被归一化（GL_TRUE）或者直接转换为固定点值（GL_FALSE）。
    // 参数stride:        指定连续顶点属性之间的偏移量。如果为0，那么顶点属性会被理解为：它们是紧密排列在一起的。初始值为0。
    GL_CHECK_SIMPLE(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0));
    
    // ‌glVertexAttribDivisor是OpenGL中的一个函数，用于控制顶点属性的重复使用，实现实例化渲染。‌
    // 参数index：顶点属性的编号。
    // 参数divisor：增量因子，表示每个顶点属性的增量步长。例如，如果我们要在渲染100个实例时使用第0个顶点属性，
    // 并且希望每隔10个实例更新一次属性，可以使用glVertexAttribDivisor(0, 10); 。这样第0个属性将会在渲染第
    // 0个实例时使用，在渲染第10个实例时使用第1个属性，依次类推‌
    GL_CHECK_SIMPLE(glVertexAttribDivisor(3, 1));
    GL_CHECK_SIMPLE(glDrawElementsInstanced(GL_TRIANGLES, box_->get_num_elements(), GL_UNSIGNED_SHORT, 0, num_occluder_instances_));

    GL_CHECK_SIMPLE(glUseProgram(sphere_program_));

    if (show_redundant_)
    {
        // 用深色绘制伪正面(false-positive)网格。
        // 伪正面网格将无法通过深度测试（使用 GL_GREATER 通过）。
        // 但又不想更新深度缓冲区，因此由于遮挡剔除引入的随机顺序，伪正面网格性将以“故障” 方式呈现。
        GL_CHECK_SIMPLE(glDepthFunc(GL_GREATER)); // 当前片元深度比缓冲区中的片元深度值大时通过
        GL_CHECK_SIMPLE(glDepthMask(GL_FALSE));// 当 glDepthMask(GL_FALSE) 时，OpenGL 不会更新深度缓冲区，但仍然会进行深度测试。
        RenderSpheres(glm::vec3(0.25f));
        GL_CHECK_SIMPLE(glDepthMask(GL_TRUE));// 当 glDepthMask(GL_TRUE) 时，OpenGL 会将片段的深度值写入深度缓冲区。
        GL_CHECK_SIMPLE(glDepthFunc(GL_LESS));
    }
    RenderSpheres(glm::vec3(1.0f));

    if (enable_culling_)
    {
        RenderDepthMap();
    }

    GL_CHECK_SIMPLE(glViewport(0, 0, window_width_, window_height_));

    // 切换到下一个间接缓冲区
    indirect_.buffer_index = (indirect_.buffer_index + 1) % INDIRECT_BUFFERS;
}

void HizOcclusionCullingApp::RenderDepthMap()
{
    GL_CHECK_SIMPLE(glDisable(GL_DEPTH_TEST)); // 要渲染深度图，首先要关闭深度测试
    GL_CHECK_SIMPLE(glUseProgram(quad_program_));

    GL_CHECK_SIMPLE(glBindVertexArray(quad_->get_vertex_array()));

    uint32_t offset_x = 0;

    // 取出了深度贴图绘制到屏幕上
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, culling_implementations_[culling_implementation_index_]->GetDepthTexture()));

    for (uint32_t lod = 0; lod <= DEPTH_SIZE_LOG2; lod++)
    {
        GL_CHECK_SIMPLE(glViewport(offset_x, 0, DEPTH_SIZE >> lod, DEPTH_SIZE >> lod));
        GL_CHECK_SIMPLE(glDrawElements(GL_TRIANGLES, quad_->get_num_elements(), GL_UNSIGNED_SHORT, 0));
        offset_x += DEPTH_SIZE >> lod;
    }
}