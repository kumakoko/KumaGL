/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

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
#include "deferred_rendering_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_string_convertor.h"


DeferredRenderingApp::DeferredRenderingApp()
{
}

DeferredRenderingApp::~DeferredRenderingApp()
{
    KGL_SAFE_DELETE(box_mesh_);
    KGL_SAFE_DELETE(light_sphere_mesh_);
    KGL_SAFE_DELETE(gbuffer_);
    KGL_SAFE_DELETE(geometry_process_shader_);
    KGL_SAFE_DELETE(lighting_shader_);
    KGL_SAFE_DELETE(light_sphere_shader_);
    screen_rectangle_.reset();
}

void DeferredRenderingApp::InitScene()
{
    this->InitBoxMeshPositions();
    App::InitScene();
    rs_depth_.SetEnable(GL_TRUE);

    // 初始化G-buffer
    gbuffer_ = new kgl::GBuffer;
    gbuffer_->Initialise(window_width_, window_height_);
    
    // 初始化NDC矩形图元
    screen_rectangle_ = kgl::PrimitiveTool::BuildNDCTexturedRectange();
}

void DeferredRenderingApp::InitModel()
{
    std::string model_path = "resources/model/box2/box.obj";
    box_mesh_ = new kgl::BasicStaticMesh;
    box_mesh_->LoadMesh(model_path);

    model_path = "resources/model/sphere.obj";
    light_sphere_mesh_ = new kgl::BasicStaticMesh;
    light_sphere_mesh_->LoadMesh(model_path);
}

void DeferredRenderingApp::InitShaders()
{
    const GLchar* vs_file = "resources/shader/020_dr_geometry_pass_vs.glsl";
    const GLchar* fs_file = "resources/shader/020_dr_geometry_pass_fs.glsl";
    geometry_process_shader_ = new kgl::GPUProgram;
    geometry_process_shader_->CreateFromFile(vs_file, fs_file, nullptr);
    
    vs_file = "resources/shader/020_dr_shading_vs.glsl";
    fs_file = "resources/shader/020_dr_shading_fs.glsl";
    lighting_shader_ = new kgl::GPUProgram;
    lighting_shader_->CreateFromFile(vs_file, fs_file, nullptr);

    vs_file = "resources/shader/020_light_sphere_vs.glsl";
    fs_file = "resources/shader/020_light_sphere_fs.glsl";
    light_sphere_shader_ = new kgl::GPUProgram;
    light_sphere_shader_->CreateFromFile(vs_file, fs_file, nullptr);
}

void DeferredRenderingApp::InitMainCamera()
{
    glm::vec3 camera_pos(-0.1f, 2.577f, 20.596f);
    float pitch_angle = -18.5f;
    float move_speed = 0.1f;
    float max_yaw_degree_per_frame = 1.0f;
    float max_pitch_degree_per_frame = 1.0f;
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, camera_pos);
    main_camera_->SetCameraSpeed(move_speed);
    main_camera_->SetMaxYawDegreePerFrame(max_yaw_degree_per_frame);
    main_camera_->SetMaxPitchDegreePerFrame(max_pitch_degree_per_frame);
    main_camera_->SetPitchAngle(pitch_angle);
}

void DeferredRenderingApp::InitFont()
{
    const char* font_name = "resources/font/wqy_wmh.ttf";
    std::string font_texture_name("wqyht24");
    int32_t font_size = 24;
    uint32_t font_texture_width = 512;
    uint32_t font_texture_height = 512;
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    font_renderer->Initialize();
    font_renderer->CreateFontTexture(font_name, font_texture_name.c_str(), font_size, font_texture_width, font_texture_height);
    font_renderer->SetCurrentFont(font_texture_name);

    toggle_help_on_text_ = kgl::StringConvertor::ANSItoUTF16LE("按下H键显示帮助");
    toggle_help_off_text_ = kgl::StringConvertor::ANSItoUTF16LE("按下H键关闭帮助");
    camera_ctrl_text_ = kgl::StringConvertor::ANSItoUTF16LE("持续按下W、S、A、D、U、J键，使得摄像机向前、后、左、右、上、下方向移动");
}

void DeferredRenderingApp::InitMaterials()
{
}

void DeferredRenderingApp::InitLights()
{
    const unsigned int NR_LIGHTS = 30;
    point_lights_.reserve(NR_LIGHTS);
    
    srand(time(nullptr));

    kgl::PointLight point_light;
    point_light.AttenuationConstant = 0.1f;
    point_light.AttenuationLinear = 0.25f;
    point_light.AttenuationExp = 0.25f;
    point_light.Ambient = glm::vec3(0.f);
    point_light.Specular = glm::vec3(0.1f);

    for (int i = 0; i < 3; ++i)
    {
        //计算点光源的position和颜色
        point_light.Position.x = -2.5f;
        point_light.Position.y = -5.0f;
        point_light.Position.z = -5.0f + i * 5.0f;

        point_light.Diffuse.r = ((rand() % 100) / 200.0f) + 0.5f; // 把颜色值控制在 0.5 到 1.0 之间
        point_light.Diffuse.g = ((rand() % 100) / 200.0f) + 0.5f; 
        point_light.Diffuse.b = ((rand() % 100) / 200.0f) + 0.5f; 

        point_lights_.push_back(point_light);

        //计算点光源的position和颜色
        point_light.Position.x = 2.5f;
        point_light.Position.y = -5.0f;
        point_light.Position.z = -5.0f + i * 5.0f;

        point_light.Diffuse.r = ((rand() % 100) / 200.0f) + 0.5f; // 把颜色值控制在 0.5 到 1.0 之间
        point_light.Diffuse.g = ((rand() % 100) / 200.0f) + 0.5f;
        point_light.Diffuse.b = ((rand() % 100) / 200.0f) + 0.5f;

        point_lights_.push_back(point_light);

        //计算点光源的position和颜色
        point_light.Position.x = 7.5f;
        point_light.Position.y = -5.0f;
        point_light.Position.z = -5.0f + i * 5.0f;

        point_light.Diffuse.r = ((rand() % 100) / 200.0f) + 0.5f; // 把颜色值控制在 0.5 到 1.0 之间
        point_light.Diffuse.g = ((rand() % 100) / 200.0f) + 0.5f;
        point_light.Diffuse.b = ((rand() % 100) / 200.0f) + 0.5f;

        point_lights_.push_back(point_light);

        //计算点光源的position和颜色
        point_light.Position.x = -7.5f;
        point_light.Position.y = -5.0f;
        point_light.Position.z = -5.0f + i * 5.0f;

        point_light.Diffuse.r = ((rand() % 100) / 200.0f) + 0.5f; // 把颜色值控制在 0.5 到 1.0 之间
        point_light.Diffuse.g = ((rand() % 100) / 200.0f) + 0.5f;
        point_light.Diffuse.b = ((rand() % 100) / 200.0f) + 0.5f;

        point_lights_.push_back(point_light);
    }

    for (size_t i = 0; i < box_positions_.size(); ++i)
    {
        //计算点光源的position和颜色
        point_light.Position = box_positions_[i];
        point_light.Position.y += 2.5f;

        point_light.Diffuse.r = ((rand() % 100) / 200.0f) + 0.5f; // 把颜色值控制在 0.5 到 1.0 之间
        point_light.Diffuse.g = ((rand() % 100) / 200.0f) + 0.5f; 
        point_light.Diffuse.b = ((rand() % 100) / 200.0f) + 0.5f; 

        point_lights_.push_back(point_light);

        point_light.Position = box_positions_[i];
        point_light.Position.y -= 2.5f;

        point_lights_.push_back(point_light);
    }
}

void DeferredRenderingApp::PreRenderFrame()
{
    //glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
     glClearDepth(1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DeferredRenderingApp::RenderScene()
{
    float current_time = (GLfloat)glfwGetTime() * 0.5f;
    
    rs_depth_.Use();
    main_camera_->Update();
    const glm::vec3& view_pos = main_camera_->GetPosition();
    const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
    const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
    glm::mat4 world_matrix;

    // 1 开始几何阶段的处理 =====================================================================
    gbuffer_->StartGeometryRenderPass();
    
    geometry_process_shader_->Use();
    geometry_process_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "u_projection_matrix");
    geometry_process_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "u_view_matrix");

    /* 因为BasicStaticMesh内部已经默认地在Render函数内部active bind了模型中指
       定的纹理了，并且指定的texture unit是0.所以不需要在这里显式地去调用并制定
    geometry_process_shader_->ApplyTexture("texture_diffuse1", 0);
    */
    for (unsigned int i = 0; i < box_positions_.size(); i++)
    {
        world_matrix = glm::scale(glm::vec3(1.0f)) * 
                       glm::translate(box_positions_[i]) * 
                       glm::rotate(current_time, glm::vec3(0.0f, 1.0f, 0.0f));
        geometry_process_shader_->ApplyMatrix(glm::value_ptr(world_matrix), "u_world_matrix");
        box_mesh_->Render();
    }

    gbuffer_->EndGeometryRenderPass();

    // 2 开始光照阶段的处理 =====================================================================

    gbuffer_->StartLightingRenderPass();

    lighting_shader_->Use();
    lighting_shader_->ApplyTexture("u_pos_in_gbuffer", 0);    // 分别从G-Buffer读取物体的位置，法线和反射信息
    lighting_shader_->ApplyTexture("u_normal_in_gbuffer", 1);
    lighting_shader_->ApplyTexture("u_albedo_in_gbuffer", 2);

    for (unsigned int i = 0; i < point_lights_.size(); i++)
    {
        lighting_shader_->ApplyPointLight(&point_lights_[i], ("u_point_lights[" + std::to_string(i) + "]").c_str());
    }

    lighting_shader_->ApplyVector3(glm::value_ptr(view_pos), "u_view_pos");

    // 把光照计算结果绘制到一个NDC和屏幕大小的offscreen矩形上 ======================================
    screen_rectangle_->Draw();

    // 3 ====================================================================================
    gbuffer_->Blit();

    // 4 在场景中渲染处表示灯光的小球 ============================================================
    light_sphere_shader_->Use();
    light_sphere_shader_->ApplyMatrix(glm::value_ptr(projection_matrix),"projection");
    light_sphere_shader_->ApplyMatrix(glm::value_ptr(view_matrix),"view");

    for (unsigned int i = 0; i < point_lights_.size(); i++)
    {
        world_matrix = glm::mat4();
        world_matrix = glm::translate(world_matrix, point_lights_[i].Position);
        world_matrix = glm::scale(world_matrix, glm::vec3(0.05f));
        light_sphere_shader_->ApplyMatrix(glm::value_ptr(world_matrix),"model");
        light_sphere_shader_->ApplyVector3(glm::value_ptr(point_lights_[i].Diffuse), "lightColor");
        light_sphere_mesh_->Render();
    }

    this->RenderHelpText(view_pos);
}

void DeferredRenderingApp::RenderHelpText(const glm::vec3& view_pos)
{
    const std::wstring& help_toggle = is_help_on_ ? toggle_help_off_text_ : toggle_help_on_text_;
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    glm::vec4 text_color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    font_renderer->AddToRendered(help_toggle, 0, 0, text_color, 1.0f);

    if (is_help_on_)
    {
        boost::format fmt("摄像机位置坐标：(%-8.3f,%-8.3f,%-8.3f) pitch角:%-8.3f Yaw角:%-8.3f");
        fmt % view_pos.x % view_pos.y % view_pos.z % main_camera_->GetPitchAngle() % main_camera_->GetYawAngle();
        font_renderer->AddToRendered(camera_ctrl_text_, 0, 25, text_color, 1.0f);
        font_renderer->AddToRendered(kgl::StringConvertor::ANSItoUTF16LE(fmt.str().c_str()), 0, 50, text_color, 1.0f);
    }

    font_renderer->Draw();
}

void DeferredRenderingApp::ProcessInput()
{
    if (key_state_[GLFW_KEY_W])
    {
        main_camera_->Move(kgl::CameraDirection::FORWARD);
    }

    if (key_state_[GLFW_KEY_S])
    {
        main_camera_->Move(kgl::CameraDirection::BACK);
    }

    if (key_state_[GLFW_KEY_A])
    {
        main_camera_->Move(kgl::CameraDirection::LEFT);
    }

    if (key_state_[GLFW_KEY_D])
    {
        main_camera_->Move(kgl::CameraDirection::RIGHT);
    }

    if (key_state_[GLFW_KEY_J])
    {
        main_camera_->Move(kgl::CameraDirection::DOWN);
    }

    if (key_state_[GLFW_KEY_U])
    {
        main_camera_->Move(kgl::CameraDirection::UP);
    }

    if (key_state_[GLFW_KEY_UP])
    {
        main_camera_->ChangePitch(0.5f);
    }

    if (key_state_[GLFW_KEY_DOWN])
    {
        main_camera_->ChangePitch(-0.5f);
    }

    if (key_state_[GLFW_KEY_LEFT])
    {
        main_camera_->ChangeYaw(0.5f);
    }

    if (key_state_[GLFW_KEY_RIGHT])
    {
        main_camera_->ChangeYaw(-0.5f);
    }
}

void DeferredRenderingApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    App::OnKeyAction(window, key, scancode, action, mode);

    if (GLFW_KEY_H == key && action == GLFW_RELEASE)
    {
        is_help_on_ = !is_help_on_;
    }
}

void DeferredRenderingApp::InitBoxMeshPositions()
{
    box_positions_.reserve(9);
    box_positions_.push_back(glm::vec3(-5.0, -5.0, -5.0));
    box_positions_.push_back(glm::vec3(0.0, -5.0, -5.0));
    box_positions_.push_back(glm::vec3(5.0, -5.0, -5.0));
    box_positions_.push_back(glm::vec3(-5.0, -5.0, 0.0));
    box_positions_.push_back(glm::vec3(0.0, -5.0, 0.0));
    box_positions_.push_back(glm::vec3(5.0, -5.0, 0.0));
    box_positions_.push_back(glm::vec3(-5.0, -5.0, 5.0));
    box_positions_.push_back(glm::vec3(0.0, -5.0, 5.0));
    box_positions_.push_back(glm::vec3(5.0, -5.0, 5.0));

}