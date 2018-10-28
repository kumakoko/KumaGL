// 简单的纹理应用示例
#include "lighting_app.h"
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_string_convertor.h"

LightingApp::LightingApp()/* :model_(nullptr), cur_mat_index_(0)*/
{
}

LightingApp::~LightingApp()
{
    KGL_SAFE_DELETE(model_);
    lighting_shader_.reset();
}

void LightingApp::InitScene()
{
    App::InitScene();
    rs_depth_.SetEnable(GL_TRUE);
}

void LightingApp::InitModel()
{
    const char* model_path = "resources/model/bun_zipper_res2.3ds";
    model_ = new kgl::BasicStaticMesh;// (kgl::VERTEX_TYPE_PN, model_path);
    model_->LoadMesh(std::string(model_path));
    //std::size_t sz = model_->GetMeshCount();

    /*
    for (std::size_t s = 0; s < sz; ++s)
    {
        model_->ApplyShaderToMesh(s, lighting_shader_);
    }*/
}

void LightingApp::InitShaders()
{
    lighting_shader_ = std::make_shared<kgl::GPUProgram>();
    std::vector<std::string> vs_file_paths;
    std::vector<std::string> fs_file_paths;
    std::vector<std::string> gs_file_path;
    vs_file_paths.push_back("resources/shader/010_lighting_vs.glsl");
    fs_file_paths.push_back("resources/shader/lighting_struct.glsl");
    fs_file_paths.push_back("resources/shader/lighting_fs.glsl");
    fs_file_paths.push_back("resources/shader/010_lighting_fs.glsl");
    lighting_shader_->CreateFromFile(vs_file_paths, fs_file_paths, gs_file_path, 3, 3, 0);
}

void LightingApp::InitMainCamera()
{
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(0.0f, 0.1f, 0.305f));
    main_camera_->SetCameraSpeed(0.005f);
}

void LightingApp::InitFont()
{
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    font_renderer->Initialize();
    font_renderer->CreateFontTexture("resources/font/fzkt_sim.ttf", "fzktsim24", 24, 512, 512);
    font_renderer->SetCurrentFont("fzktsim24");

    toggle_help_on_text_ = kgl::StringConvertor::ANSItoUTF16LE("按下H键显示帮助");
    toggle_help_off_text_ = kgl::StringConvertor::ANSItoUTF16LE("按下H键关闭帮助");
    camera_ctrl_text_ = kgl::StringConvertor::ANSItoUTF16LE("持续按下W、S、A、D、U、J键，使得摄像机向前、后、左、右、上、下方向移动");
    material_ctrl_text_ = kgl::StringConvertor::ANSItoUTF16LE("按下左右箭头键，切换模型使用的材质");
}

void LightingApp::InitMaterials()
{
    kgl::Material m;

    // 黄金材质
    m.Ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
    m.Diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
    m.Specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
    m.Shininess = 0.4f * 128.f;
    materials_.push_back(m);
    material_name_text_.push_back(kgl::StringConvertor::ANSItoUTF16LE("材质：黄金"));

    // 白银材质
    m.Ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f);
    m.Diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
    m.Specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);
    m.Shininess = 0.4f * 128.f;
    materials_.push_back(m);
    material_name_text_.push_back(kgl::StringConvertor::ANSItoUTF16LE("材质：白银"));

    // 红宝石材质
    m.Ambient = glm::vec3(0.1745f, 0.01175f, 0.01175f);
    m.Diffuse = glm::vec3(0.61424f, 0.04136f, 0.04136f);
    m.Specular = glm::vec3(0.727811f, 0.626959f, 0.626959f);
    m.Shininess = 0.4f * 128.f;
    materials_.push_back(m);
    material_name_text_.push_back(kgl::StringConvertor::ANSItoUTF16LE("材质：红宝石"));

    // 红色塑料
    m.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    m.Diffuse = glm::vec3(0.5f, 0.0f, 0.0f);
    m.Specular = glm::vec3(0.7f, 0.6f, 0.6f);
    m.Shininess = 0.25f * 128.f;
    materials_.push_back(m);
    material_name_text_.push_back(kgl::StringConvertor::ANSItoUTF16LE("材质：红色塑料"));

    // 铜材质
    m.Ambient = glm::vec3(0.2125f, 0.1275f, 0.054f);
    m.Diffuse = glm::vec3(0.714f, 0.4284f, 0.18144f);
    m.Specular = glm::vec3(0.393548f, 0.271906f, 0.166721f);
    m.Shininess = 0.2f * 128.f;
    materials_.push_back(m);
    material_name_text_.push_back(kgl::StringConvertor::ANSItoUTF16LE("材质：铜"));

    //铬材质
    m.Ambient = glm::vec3(0.25f, 0.25f, 0.25f);
    m.Diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    m.Specular = glm::vec3(0.774597f, 0.774597f, 0.774597f);
    m.Shininess = 0.6f * 128.f;
    materials_.push_back(m);
    material_name_text_.push_back(kgl::StringConvertor::ANSItoUTF16LE("材质：铬"));

    // 祖母绿宝石材质
    m.Ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
    m.Diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
    m.Specular = glm::vec3(0.633f, 0.727811f, 0.633f);
    m.Shininess = 0.6f * 128.f;
    materials_.push_back(m);
    material_name_text_.push_back(kgl::StringConvertor::ANSItoUTF16LE("材质：祖母绿宝石"));
}

void LightingApp::InitLights()
{
    directional_light_.Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    directional_light_.Diffuse = glm::vec3(0.2f, 0.2f, 0.2f);
    directional_light_.Direction = glm::vec3(0.2f, 0.2f, 0.2f);
    directional_light_.Specular = glm::vec3(0.2f, 0.2f, 0.2f);

    point_light_.Ambient = glm::vec3(1.0, 1.0f, 1.0f);
    point_light_.Diffuse = glm::vec3(1.0, 1.0f, 1.0f);
    point_light_.Position = glm::vec3(0.0f, -2.0f, 0.0f);
    point_light_.Specular = glm::vec3(1.0, 1.0f, 1.0f);
    point_light_.AttenuationConstant = 1.0f;
    point_light_.AttenuationLinear = 1.0f;
    point_light_.AttenuationExp = 1.0f;

    spot_light_.Ambient = glm::vec3(1.0, 1.0f, 1.0f);
    spot_light_.Diffuse = glm::vec3(1.0, 1.0f, 1.0f);
    spot_light_.Position = glm::vec3(0.0f, -2.0f, 0.0f);
    spot_light_.Specular = glm::vec3(1.0, 1.0f, 1.0f);
    spot_light_.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spot_light_.CutOffAndAttenuation = glm::vec4(60.0f, 1.0f, 1.0f, 1.0f);
    spot_light_.Exponent = 2.0f;
}

void LightingApp::RenderFrame()
{
    main_camera_->Update();
    lighting_shader_->Use();

    glm::mat4 model_matrix;
    model_matrix = glm::rotate(model_matrix, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
    const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
    const glm::vec3& view_pos = main_camera_->GetPosition();

    lighting_shader_->Use();
    lighting_shader_->ApplyMatrix(glm::value_ptr(model_matrix), "model_matrix");
    lighting_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
    lighting_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
    lighting_shader_->ApplyVector3(glm::value_ptr(view_pos), "view_pos");
    lighting_shader_->ApplyDirectionalLight(&directional_light_, "directional_light");
    lighting_shader_->ApplyPointLight(&point_light_, "point_light");
    lighting_shader_->ApplySpotLight(&spot_light_, "spot_light");
    lighting_shader_->ApplyMaterial(&materials_[cur_mat_index_], "material");
    
    rs_depth_.Use();

    model_->Render();

    const std::wstring& help_toggle = is_help_on_ ? toggle_help_off_text_ : toggle_help_on_text_;
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();

    glm::vec4 text_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    font_renderer->AddToRendered(help_toggle, 0, 0,text_color, 1.0f);

    if (is_help_on_)
    {   
        boost::format fmt("摄像机位置坐标： x = %f , y = %f , z = %f");
        fmt % view_pos.x % view_pos.y % view_pos.z;
        font_renderer->AddToRendered(camera_ctrl_text_, 0, 25,text_color, 1.0f);
        font_renderer->AddToRendered(material_ctrl_text_, 0, 50,text_color, 1.0f);
        font_renderer->AddToRendered(material_name_text_[cur_mat_index_], 0, 75, text_color, 1.0f);
        font_renderer->AddToRendered(kgl::StringConvertor::ANSItoUTF16LE(fmt.str().c_str()), 0, 100, text_color, 1.0f);
    }

    font_renderer->Draw();
}

void LightingApp::ProcessInput()
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
}

void LightingApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    App::OnKeyAction(window, key, scancode, action, mode);

    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
        if (cur_mat_index_ == 0)
        {
            cur_mat_index_ = materials_.size() - 1;
            return;
        }
        cur_mat_index_--;
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    {
        cur_mat_index_++;

        if (cur_mat_index_ >= materials_.size())
            cur_mat_index_ = 0;
    }

    if (GLFW_KEY_F == key && action == GLFW_RELEASE)
    {
        draw_mode_.SetCurrentDrawMode(kgl::DM_FILL);
    }

    if (GLFW_KEY_L == key && action == GLFW_RELEASE)
    {
        draw_mode_.SetCurrentDrawMode(kgl::DM_LINE);
    }

    if (GLFW_KEY_P == key && action == GLFW_RELEASE)
    {
        draw_mode_.SetCurrentDrawMode(kgl::DM_POINT);
    }

    if (GLFW_KEY_H == key && action == GLFW_RELEASE)
    {
        is_help_on_ = !is_help_on_;
    }
}