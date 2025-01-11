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

    set_show_redundant(true); // 显示冗余
    SetCullingMethod(CullingMethod::CullNone);
}

void HizOcclusionCullingApp::RenderGUI()
{
    if (!use_gui_)
        return;

    static float f = 0.0f;
    static int counter = 0;

    if(ImGui::Begin("006 Hiz Occlusion Culling"))
    {
        ImGui::Checkbox("显示冗余", &show_redundant_);

        // 定义拣选模式选项列表
        const char* cull_mode_names[] = { "Cull HiZ", "Cull HiZ NoLOD", "Cull None"};

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

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();
}

HizOcclusionCullingApp::~HizOcclusionCullingApp()
{
    KGL_SAFE_DELETE(box_);

    for (uint32_t i = 0; i < SPHERE_LODS_COUNT; i++)
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

    GL_CHECK_SIMPLE(glDeleteBuffers(INDIRECT_BUFFERS_COUNT, indirect_.buffer));
    GL_CHECK_SIMPLE(glDeleteBuffers(SPHERE_LODS_COUNT, indirect_.instance_buffer));
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
    for (uint32_t i = 0; i < SPHERE_LODS_COUNT; i++)
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

    num_occluder_instances_ = occluder_instances.size();

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
    GL_CHECK_SIMPLE(glGenBuffers(SPHERE_LODS_COUNT, indirect_.instance_buffer));

    for (uint32_t i = 0; i < SPHERE_LODS_COUNT; i++)
    {
        GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, indirect_.instance_buffer[i]));
        GL_CHECK_SIMPLE(glBufferData(GL_ARRAY_BUFFER, sphere_instances.size() * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY));
    }

    std::vector<glm::vec4> occluder_positions;
    std::vector<uint32_t> occluder_indices;

    BakeOccluderGeometry(occluder_positions, occluder_indices, box_mesh, occluder_instances);

    // 烘焙好遮挡体的顶点缓冲值和索引缓冲值之后，交给culling处理模块上传到GPU
    for (uint32_t i = 0; i < culling_implementations_.size(); i++)
    {
        culling_implementations_[i]->SetupOccluderGeometry(occluder_positions, occluder_indices);
    }

    // 初始化间接绘制缓冲区。使用环形缓冲区，因为可能希望读回旧结果来监控剔除性能，而不会拖延管道。
    GL_CHECK_SIMPLE(glGenBuffers(INDIRECT_BUFFERS_COUNT, indirect_.buffer));

    for (uint32_t i = 0; i < SPHERE_LODS_COUNT; i++)
    {
        GL_CHECK_SIMPLE(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_.buffer[i]));
        GL_CHECK_SIMPLE(glBufferData(GL_DRAW_INDIRECT_BUFFER, SPHERE_LODS_COUNT * sizeof(IndirectCommand), NULL, GL_DYNAMIC_COPY));
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
    // 在此计算 view-projection 矩阵
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
    occluder_program_->ApplyMatrix(vp_mat_ptr, UNIFORM_MVP_LOCATION);
    sphere_program_->ApplyMatrix(vp_mat_ptr, UNIFORM_MVP_LOCATION);
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

    physics_program_->Use();
    sphere_instances_buffer_->RebindBufferBase(GL_SHADER_STORAGE_BUFFER, 0);
    physics_program_->ApplyUint(SPHERE_INSTANCES, 0);
    physics_program_->ApplyFloat(physics_speed_ * delta_time, 1);
    physics_program_->Dispatch((SPHERE_INSTANCES + PHYSICS_GROUP_SIZE - 1) / PHYSICS_GROUP_SIZE, 1, 1);
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

    // 让小球运动起来，在compute shader 中计算
    ApplyPhysics(frame_time_delta_);

    if (enable_culling_)
    {
        CullingInterface* culler = culling_implementations_[culling_implementation_index_];
        num_sphere_render_lods_ = culler->GetLodsCount();

        // 把遮挡物光栅化到深度图上，并且形成mipmap链
        culler->SetViewProjectionMatrix(projection_matrix_, view_matrix_, glm::vec2(Z_NEAR, Z_FAR));
        culler->RasterizeOccluders();

        // We need physics results after this.
        GL_CHECK_SIMPLE(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

        IndirectCommand indirect_command[SPHERE_LODS_COUNT];
        uint32_t offsets[SPHERE_LODS_COUNT];

        memset(indirect_command, 0, sizeof(indirect_command));

        for (uint32_t i = 0; i < SPHERE_LODS_COUNT; i++)
        {
            indirect_command[i].count = sphere_[i]->GetNumElements();
            offsets[i] = 4 + sizeof(IndirectCommand) * i;
        }

        // Clear out our indirect draw buffer.
        GL_CHECK_SIMPLE(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_.buffer[indirect_.buffer_index]));
        GL_CHECK_SIMPLE(glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(indirect_command), indirect_command, GL_STREAM_DRAW));

        // 测试遮挡物，并为每个 LOD 构建间接命令以及逐实例缓冲区(per-instance buffers)。
        culler->TestBoundingBoxes(indirect_.buffer[indirect_.buffer_index], offsets, SPHERE_LODS_COUNT,
            indirect_.instance_buffer, sphere_instances_buffer_,num_render_sphere_instances_);
    }
    else
    {
        // If we don't do culling, we need to make sure we call the memory barrier for physics.
        GL_CHECK_SIMPLE(glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT));
        num_sphere_render_lods_ = 1;
    }
}

void HizOcclusionCullingApp::RenderSpheres(glm::vec3 color_mod)
{
    if (enable_culling_)
    {
        // 间接绘制命令，首先绑定间接绘制命令用到的buffer， GL_DRAW_INDIRECT_BUFFER 是 OpenGL 中的一个
        // 缓冲区绑定目标(Buffer Binding Target)，用于存储间接绘制命令（Indirect Drawing Commands）
        // 在InitModel函数里面已经执行过一次对indirect buffer 的 glBufferData 操作了
        GL_CHECK_SIMPLE(glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_.buffer[indirect_.buffer_index]));

        for (unsigned i = 0; i < num_sphere_render_lods_; i++)
        {
            sphere_program_->ApplyVector3(glm::value_ptr(color_mod * glm::vec3(0.8f - 0.2f * i, 1.2f - 0.2f * i, 0.8f + 0.2f * i)), UNIFORM_COLOR_LOCATION);

            // 使用不同的LOD级别
            GL_CHECK_SIMPLE(glBindVertexArray(sphere_[i]->GetVertexArray()));
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
            GL_CHECK_SIMPLE(glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT,
                reinterpret_cast<const void*>(i * sizeof(IndirectCommand))));
        }
    }
    else
    {
        // 不拣选的话，就只绘制LOD0的实例
        sphere_program_->ApplyVector3(glm::value_ptr(color_mod * glm::vec3(0.8f, 1.2f, 0.8f)), UNIFORM_COLOR_LOCATION);
        GL_CHECK_SIMPLE(glBindVertexArray(sphere_[0]->GetVertexArray()));
        GL_CHECK_SIMPLE(glEnableVertexAttribArray(3));
        GL_CHECK_SIMPLE(glVertexAttribDivisor(3, 1));
        sphere_instances_buffer_->Rebinding(GL_ARRAY_BUFFER);// 这里一开始是GL_SHADER_STORAGE_BUFFER类型，现在重新绑定为
        GL_CHECK_SIMPLE(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4), 0));
        GL_CHECK_SIMPLE(glDrawElementsInstanced(GL_TRIANGLES, sphere_[0]->GetNumElements(), GL_UNSIGNED_SHORT, NULL, num_render_sphere_instances_));
    }
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

    // 渲染用来遮挡的BOX
    occluder_program_->Use();
    occluder_program_->ApplyFloat3(1.2f, 0.6f, 0.6f, UNIFORM_COLOR_LOCATION);

    // 绑定一个BOX的vertex array object，使之成为当前上下文中的活动VAO
    // 后面的glEnableVertexAttribArray glVertexAttribPointer glVertexAttribDivisor对应的指令操作
    // 都会对应记录于本VAO中
    GL_CHECK_SIMPLE(glBindVertexArray(box_->GetVertexArray()));
    occluder_instances_buffer_->Rebinding(GL_ARRAY_BUFFER);

    /*glEnableVertexAttribArray 是 OpenGL 中的一个重要函数，用于启用指定的顶点属性数组。
    它的作用是告诉 OpenGL，某个顶点属性（例如位置、颜色、法线等）将从顶点缓冲对象（VBO）中读取数据，
    而不是使用默认的常量值。这函数的唯一输入参数为要启用的顶点属性的索引。这个索引对应于顶点着色器中 
    layout(location = index) 指定的位置。*/
    // 在060_scene_vs.glsl中，属性顶点的instance属性，其layout值为3，如下代码语句
    // layout(location = 3) in vec3 aOffset; // Instanced array
    GL_CHECK_SIMPLE(glEnableVertexAttribArray(3));

    // void glVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,const GLvoid * pointer);
    // 参数index:         指定要修改的顶点属性的索引值，
    // 参数size:          指定每个顶点属性的组件数量。必须为1、2、3或者4。初始值为4。（如position是由3个（x,y,z）组成，而颜色是4个（r,g,b,a)
    // 参数GLenum:        指定数组中每个组件的数据类型。可用的符号常量有GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,GL_UNSIGNED_SHORT, GL_FIXED, 和 GL_FLOAT，初始值为GL_FLOAT。
    // 参数normalized:    指定当被访问时，固定点数据值是否应该被归一化（GL_TRUE）或者直接转换为固定点值（GL_FALSE）。
    // 参数stride:        指定连续顶点属性之间的偏移量。如果为0，那么顶点属性会被理解为：它们是紧密排列在一起的。初始值为0。
    // 参数pointer:       指定第一个组件在数组的第一个顶点属性中的偏移量。该数组与GL_ARRAY_BUFFER绑定，储存于缓冲区中。初始值为0
    GL_CHECK_SIMPLE(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0));

    // ‌glVertexAttribDivisor是OpenGL中的一个函数，用于控制顶点属性的重复使用，实现实例化渲染。‌
    // 参数index：顶点属性的编号。
    // 参数divisor：增量因子，表示每个顶点属性的增量步长。例如，如果我们要在渲染100个实例时使用第0个顶点属性，
    // 并且希望每隔10个实例更新一次属性，可以使用glVertexAttribDivisor(0, 10); 。这样第0个属性将会在渲染第
    // 0个实例时使用，在渲染第10个实例时使用第1个属性，依次类推‌
    // glVertexAttribDivisor(3, 1)表示，每一个顶点就读取更新一次instance array里面的对应的occluder_instances的中的元素的信息
    GL_CHECK_SIMPLE(glVertexAttribDivisor(3, 1));
    GL_CHECK_SIMPLE(glDrawElementsInstanced(GL_TRIANGLES, box_->GetNumElements(), GL_UNSIGNED_SHORT, 0, num_occluder_instances_));

    sphere_program_->Use();

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

    // 如果啓動了揀選的话，要执行渲染深度图
    if (enable_culling_)
    {
        RenderDepthMap();
    }

    GL_CHECK_SIMPLE(glViewport(0, 0, window_width_, window_height_));

    // 切换到下一个间接缓冲区
    indirect_.buffer_index = (indirect_.buffer_index + 1) % INDIRECT_BUFFERS_COUNT;
}

void HizOcclusionCullingApp::RenderDepthMap()
{
    GL_CHECK_SIMPLE(glDisable(GL_DEPTH_TEST)); // 要渲染深度图，首先要关闭深度测试
    quad_program_->Use();

    GL_CHECK_SIMPLE(glBindVertexArray(quad_->GetVertexArray()));

    uint32_t offset_x = 0;

    // 取出了深度贴图绘制到屏幕上
    GL_CHECK_SIMPLE(glBindTexture(GL_TEXTURE_2D, culling_implementations_[culling_implementation_index_]->GetDepthTexture()));

    for (uint32_t lod = 0; lod <= DEPTH_SIZE_LOG2; lod++)
    {
        GL_CHECK_SIMPLE(glViewport(offset_x, 0, DEPTH_SIZE >> lod, DEPTH_SIZE >> lod));
        GL_CHECK_SIMPLE(glDrawElements(GL_TRIANGLES, quad_->GetNumElements(), GL_UNSIGNED_SHORT, 0));
        offset_x += DEPTH_SIZE >> lod;
    }
}

void HizOcclusionCullingApp::BakeOccluderGeometry(std::vector<glm::vec4>& occluder_positions, std::vector<uint32_t>& occluder_indices, const Mesh& box_mesh, const std::vector<glm::vec4>& instances_pos)
{
    uint32_t num_instances = instances_pos.size();
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

        // 根据传递进来的instances_pos数组，更新遮挡物的位置 
        glm::vec4 pos = instances_pos[instance] + glm::vec4(box_mesh.vbo[index].position, 1.0f);
        occluder_positions.push_back(pos);
    }
}
