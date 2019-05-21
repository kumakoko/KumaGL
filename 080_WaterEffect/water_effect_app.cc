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
#include "../klib/kgl_wfx_terrin_texture.h"
#include "../klib/kgl_wfx_terrin_texture_group.h"

#include "water_effect_app.h"

void WaterEffectApp::RenderGUI()
{
    const glm::vec3& camera_pos = main_camera_->GetPosition();
    ImGui::Begin("水波折射效果");
    ImGui::Text("按WSADUJ键前后左右上下移动摄像机");
    ImGui::Text("按上下键绕X轴旋转相机，左右键绕Y轴旋转相机");
    ImGui::Text("摄像机坐标值： %.3f， %.3f， %.3f", camera_pos.x, camera_pos.y, camera_pos.z);
    ImGui::Text("摄像机Pitch角，Yaw角： %.3f， %.3f", main_camera_->GetPitchAngle(), main_camera_->GetYawAngle());
    ImGui::Checkbox("以线框模式显示", &wireframe_mode_);
    ImGui::Checkbox("绘制世界", &is_draw_world_);
    ImGui::SliderFloat("水面Y值", &(water_tiles_[0].height_), -10.f,10.f);
    ImGui::End();
}

WaterEffectApp::WaterEffectApp()
{
}

WaterEffectApp::~WaterEffectApp()
{
    terrains_.clear();
    lights_.clear();
    KGL_SAFE_DELETE(main_render_module_);
    KGL_SAFE_DELETE(water_renderer_);
    water_fb_.reset();
    loader_.reset();
}

void WaterEffectApp::PreRenderFrame()
{
    glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WaterEffectApp::InitScene()
{
    App::InitScene();

    loader_ = std::make_shared<wfx::Loader>();


    this->InitTerrain();
    this->InitWater();

    main_render_module_ = new MainRenderModule(main_camera_, loader_);
}

void WaterEffectApp::InitWater()
{
    water_fb_ = std::make_shared<wfx::WaterFrameBuffers>(); // 初始化water effect用到的frame buffer对象
    water_renderer_ = new wfx::WaterRenderer(loader_, water_fb_); // 初始化水体渲染器
    water_renderer_->InitWaterShader(); // 初始化水体渲染器用到的shader
    water_tiles_.push_back(wfx::WaterTile{0.f, 0.f, 0.f});
}

void WaterEffectApp::InitTerrain()
{
    // 控制贴图混合比例的混合贴图
    wfx::TerrainTexture blend_map(loader_->loadTexture("resources/image/080_water_effect/blend_map.png"));
    wfx::TerrainTexture background_texture(loader_->loadTexture("resources/image/080_water_effect/grass.png"));
    wfx::TerrainTexture red_texture(loader_->loadTexture("resources/image/080_water_effect/mud.png"));
    wfx::TerrainTexture green_texture(loader_->loadTexture("resources/image/080_water_effect/grass_flowers.png"));
    wfx::TerrainTexture blue_texture(loader_->loadTexture("resources/image/080_water_effect/path.png"));
    wfx::TerrainTexturePack texture_group(background_texture, red_texture, green_texture, blue_texture);

    // 利用initialize list生成一个Terrain对象，然后用移动构造的方式复制到terrain array
    terrains_.emplace_back(0.f, -1.f, loader_, texture_group, blend_map, "resources/image/080_water_effect/heightmap.png");
    lights_.emplace_back(glm::vec3(0, 1000, -1000), glm::vec3(0.8f, 0.8f, 0.8f));
}

void WaterEffectApp::InitMainCamera()
{
    main_camera_->SetCameraSpeed(1.0f);
    main_camera_->SetMaxYawDegreePerFrame(1.0f);
    main_camera_->SetMaxPitchDegreePerFrame(1.0f);
    glm::vec3 pos = glm::vec3(218.f, 40.f, -280.f);
    float pitch_angle = -18.f;
    float yaw_angle = -230.f;
    float fov = 120.f;
    float near_clip_distance = 0.5f;
    float far_clip_distance = 1000.f;
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, pos, pitch_angle, yaw_angle, fov, near_clip_distance, far_clip_distance);
}

void WaterEffectApp::InitSkyboxModel()
{

}

void WaterEffectApp::InitModel()
{
    this->InitSkyboxModel();
}

void WaterEffectApp::InitShaders()
{
}

void WaterEffectApp::RenderScene()
{
    draw_mode_.SetCurrentDrawMode(wireframe_mode_ ? kgl::DM_LINE : kgl::DM_FILL);
    main_camera_->Update();
    glm::vec4 clip_plane = glm::vec4(0.0f, 1.0f, 0.0f, -1.0f);

    glEnable(GL_CLIP_DISTANCE0); // 其中shader内设置clip distance的代码
    float water_height = water_tiles_[0].getHeight();

    // 将reflection frame buffer设置为当前的render target
    water_fb_->BindReflectionFrameBuffer();

    // 将摄像机移动到水底并且向上看
    glm::vec3 camera_pos = main_camera_->GetPosition();
    float distance = 2.f * (camera_pos.y - water_height);
    camera_pos.y -= distance;
    main_camera_->SetPosition(camera_pos);
    main_camera_->InvertPitchAngle(); // 如果当前摄像机观察方向与水平面的夹角为pitch，则执行此函数后，夹角变为-pitch
    main_camera_->Update(); // pitch角发生了变化，调用此函数，更新一下观察矩阵

    // 从水底渲染一次场景到frame buffer
	main_render_module_->render(terrains_, lights_, glm::vec4{ 0.f, 1.f, 0.f, -water_height + 1.f }, main_camera_);
    water_fb_->UnbindCurrentFrameBuffer();

    // 复位摄像机,从水面上向水底方向进行渲染，渲染内容到折射纹理
    camera_pos.y += distance;
    main_camera_->SetPosition(camera_pos);
    main_camera_->InvertPitchAngle();
    main_camera_->Update();

    water_fb_->BindRefractionFrameBuffer();
	main_render_module_->render(terrains_, lights_, glm::vec4{ 0.f, -1.f, 0.f, water_height + 1.f }, main_camera_);
    glDisable(GL_CLIP_DISTANCE0);

    water_fb_->UnbindCurrentFrameBuffer();

    if (is_draw_world_)
        main_render_module_->render(terrains_, lights_, glm::vec4{0, 0, 0, 0}, main_camera_);

    water_renderer_->render(water_tiles_, main_camera_, lights_[0], 0.1f);
}

void WaterEffectApp::ProcessInput()
{
    // 因为GUI系统接管了键盘和鼠标的输入，所以App类里面的key_state_变量就不启用了
    // 而是直接使用了ImGuiIO里面的信息	
    if (ImGui::IsKeyPressed(GLFW_KEY_W))
    {
        main_camera_->Move(kgl::CameraDirection::FORWARD);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_S))
    {
        main_camera_->Move(kgl::CameraDirection::BACK);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_A))
    {
        main_camera_->Move(kgl::CameraDirection::LEFT);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_D))
    {
        main_camera_->Move(kgl::CameraDirection::RIGHT);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_U))
    {
        main_camera_->Move(kgl::CameraDirection::UP);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_J))
    {
        main_camera_->Move(kgl::CameraDirection::DOWN);
    }

    if (key_state_[GLFW_KEY_R])
    {
        main_camera_->ChangeYaw(1.f);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_UP))
    {
        main_camera_->ChangePitch(1.f);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_DOWN))
    {
        main_camera_->ChangePitch(-1.f);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_LEFT))
    {
        main_camera_->ChangeYaw(1.f);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_RIGHT))
    {
        main_camera_->ChangeYaw(-1.f);
    }

}