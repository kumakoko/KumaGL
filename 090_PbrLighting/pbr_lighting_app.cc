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
#include "pbr_lighting_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"

PbrLightingApp::PbrLightingApp() :use_metallic_texture_(true), use_roughness_texture_(true),
base_reflectivity_(0.04f)
{
}

PbrLightingApp::~PbrLightingApp()
{
	shader_.reset();
	sphere_.reset();
	
	current_albedo_texture_.reset();
	current_normal_texture_.reset();
	current_metallic_texture_.reset();
	current_roughness_texture_.reset();
	current_ambient_occlusion_texture_.reset();

	for (int material_index = 0; material_index < MATERIAL_COUNT; ++material_index)
	{
		albedo_textures_[material_index].reset();
		metallic_textures_[material_index].reset();
		normal_textures_[material_index].reset();
		roughness_textures_[material_index].reset();
		ambient_occlusion_textures_[material_index].reset();
	}
}

void PbrLightingApp::InitMainCamera()
{
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(0.0f, 0.1f, 10.0f));
	main_camera_->SetCameraSpeed(0.5f);
}

void PbrLightingApp::RenderGUI()
{
	const glm::vec3& camera_pos = main_camera_->GetPosition();
	ImGui::Begin("090 PBR -- 基于物理的渲染效果");
	ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("按WSAD键向前后左右移动摄像机");
	ImGui::Text("摄像机坐标: (%.1f,%.1f,%.1f)", camera_pos.x, camera_pos.y, camera_pos.z);
	ImGui::Checkbox("使用金属度贴图控制金属度",&use_metallic_texture_);
	ImGui::Checkbox("使用粗糙度贴图控制粗糙度", &use_roughness_texture_);
	ImGui::SliderFloat3("用于非金属的基础反射率", glm::value_ptr(base_reflectivity_), 0.001f, 0.17f);
	ImGui::SliderFloat3("灯光颜色", glm::value_ptr(light_color_), 0.0f, 255.0f);

	if (!use_metallic_texture_)
	{
		ImGui::SliderFloat("金属度参数", &metallic_roughness_.z, 0.001f,1.0f);
	}

	if (!use_roughness_texture_)
	{
		ImGui::SliderFloat("粗糙度参数", &metallic_roughness_.w, 0.001f, 1.0f);
	}

	const char* material_name[MATERIAL_COUNT] = 
	{
		"生锈铁球", "花岗岩", "划痕金属", "竹子", "花纹黄铜器"
	};

	// 当前选中的那个 animation clip 的名字
	static const char* item_current = material_name[0];

	if (ImGui::BeginCombo("材质名", item_current, 0))
	{
		for (int n = 0; n < MATERIAL_COUNT; n++)
		{
			bool is_selected = (item_current == material_name[n]);

			if (ImGui::Selectable(material_name[n], is_selected))
			{
				item_current = material_name[n];

				current_albedo_texture_ = albedo_textures_[n];
				current_normal_texture_ = normal_textures_[n];
				current_metallic_texture_ = metallic_textures_[n];
				current_roughness_texture_ = roughness_textures_[n];
				current_ambient_occlusion_texture_ = ambient_occlusion_textures_[n];
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	ImGui::End();
}

void PbrLightingApp::InitModel()
{
	LoadTextures();

	current_albedo_texture_ = albedo_textures_[0];
	current_normal_texture_ = normal_textures_[0];
	current_metallic_texture_ = metallic_textures_[0];
	current_roughness_texture_ = roughness_textures_[0];
	current_ambient_occlusion_texture_ = ambient_occlusion_textures_[0];

	sphere_ = kgl::PrimitiveTool::BuildSphere(100, true);
}

void PbrLightingApp::InitShaders()
{
	shader_ = std::make_shared<kgl::GPUProgram>();
	const char* vs_file = "resources/shader/090_pbr_lighting_vs.glsl";
	const char* fs_file = "resources/shader/090_pbr_lighting_fs.glsl";
	shader_->CreateFromFile(vs_file, fs_file, nullptr);
}

void PbrLightingApp::LoadTextures()
{
	const char* albedo_paths[MATERIAL_COUNT] = { 
		"resources/image/pbr/unreal/rustediron2_basecolor.jpg",
		"resources/image/pbr/gray_granite_flecks/albedo.jpg",
		"resources/image/pbr/streaked_metal/albedo.jpg",
		"resources/image/pbr/bamboo/albedo.jpg",
		"resources/image/pbr/ornate_brass/albedo.jpg" };
	const char* metallic_paths[MATERIAL_COUNT] = { 
		"resources/image/pbr/unreal/rustediron2_metallic.jpg",
		"resources/image/pbr/gray_granite_flecks/metallic.jpg",
		"resources/image/pbr/streaked_metal/metallic.jpg",
		"resources/image/pbr/bamboo/metallic.jpg",
		"resources/image/pbr/ornate_brass/metallic.jpg" };
	const char* normal_paths[MATERIAL_COUNT] = { 
		"resources/image/pbr/unreal/rustediron2_normal.jpg",
		"resources/image/pbr/gray_granite_flecks/normal.jpg", 
		"resources/image/pbr/streaked_metal/normal.jpg" ,
		"resources/image/pbr/bamboo/normal.jpg",
		"resources/image/pbr/ornate_brass/normal.jpg" };
	const char* roughness_paths[MATERIAL_COUNT] = { 
		"resources/image/pbr/unreal/rustediron2_roughness.jpg",
		"resources/image/pbr/gray_granite_flecks/roughness.jpg", 
		"resources/image/pbr/streaked_metal/roughness.jpg" ,
		"resources/image/pbr/ornate_brass/roughness.jpg",
		"resources/image/pbr/bamboo/roughness.jpg" };
	const char* ambient_occlusion_paths[MATERIAL_COUNT] = {
		"resources/image/pbr/unreal/rustediron2_ambient_occlusion.jpg",
		"resources/gray_granite_flecks/ambient_occlusion.jpg", 
		"resources/image/pbr/streaked_metal/ambient_occlusion.jpg",
		"resources/image/pbr/bamboo/ambient_occlusion.jpg",
		"resources/image/pbr/ornate_brass/ambient_occlusion.jpg" };

	kgl::TextureParams texture_rgb_param =
		kgl::TextureManager::MakeTextureParamsRGB(GL_REPEAT, GL_LINEAR, GL_UNSIGNED_BYTE, true);

	for (int material_index = 0; material_index < MATERIAL_COUNT; ++material_index)
	{
		albedo_textures_[material_index] = std::make_shared<kgl::SourceTexture>();
		albedo_textures_[material_index]->CreateFromFile(albedo_paths[material_index], texture_rgb_param);

		metallic_textures_[material_index] = std::make_shared<kgl::SourceTexture>();
		metallic_textures_[material_index]->CreateFromFile(metallic_paths[material_index], texture_rgb_param);

		normal_textures_[material_index] = std::make_shared<kgl::SourceTexture>();
		normal_textures_[material_index]->CreateFromFile(normal_paths[material_index], texture_rgb_param);

		roughness_textures_[material_index] = std::make_shared<kgl::SourceTexture>();
		roughness_textures_[material_index]->CreateFromFile(roughness_paths[material_index], texture_rgb_param);

		ambient_occlusion_textures_[material_index] = std::make_shared<kgl::SourceTexture>();
		ambient_occlusion_textures_[material_index]->CreateFromFile(ambient_occlusion_paths[material_index], texture_rgb_param);
	}
}

void PbrLightingApp::InitLights()
{
	light_position_ = glm::vec3(0.0f, 0.0f, 10.0f);
	light_color_ = glm::vec3(255.0f, 255.0f, 255.0f);
}

void PbrLightingApp::InitScene()
{
	App::InitScene();

	// 设置拣选模式和深度检测模式
	cull_mode_.SetEnable(TRUE);
	cull_mode_.SetFrontFaceMode(GL_CW);
	cull_mode_.SetCullMode(GL_BACK);
	depth_test_mode_.SetEnable(GL_TRUE);
	depth_test_mode_.SetDepthTestFunc(GL_LESS);
	cull_mode_.Use();
	depth_test_mode_.Use();
}

void PbrLightingApp::RenderScene()
{
	metallic_roughness_.x = use_metallic_texture_ ? 1.0f : -1.0f;
	metallic_roughness_.y = use_roughness_texture_ ? 1.0f : -1.0f;
	
	main_camera_->Update();

	float current_time = (GLfloat)glfwGetTime() * 0.5f;
	glm::vec3 newPos = light_position_ +glm::vec3(sin(glfwGetTime()) * 5.0, 0.0, 0.0);

	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();

	auto world_matrix = glm::identity<glm::mat4>();
	world_matrix = glm::scale(world_matrix, glm::vec3(5.f, 5.f, 5.f));
	world_matrix = glm::rotate(world_matrix, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

	shader_->Use();
	shader_->ApplyMatrix(glm::value_ptr(view_matrix), "u_view_matrix");
	shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "u_projection_matrix");
	shader_->ApplyMatrix(glm::value_ptr(world_matrix), "u_world_matrix");
	shader_->ApplyTexture(current_albedo_texture_, "u_albedo_map", 0);
	shader_->ApplyTexture(current_normal_texture_, "u_normal_map", 1);
	shader_->ApplyTexture(current_metallic_texture_, "u_metallic_map", 2);
	shader_->ApplyTexture(current_roughness_texture_, "u_roughness_map", 3);
	shader_->ApplyTexture(current_ambient_occlusion_texture_, "u_ambient_occlusion_map", 4);
	shader_->ApplyVector3(glm::value_ptr(main_camera_->GetPosition()),"u_camera_pos_in_world");
	shader_->ApplyVector3(glm::value_ptr(newPos), "u_light_pos_in_world");
	shader_->ApplyVector3(glm::value_ptr(light_color_), "u_light_color");
	shader_->ApplyVector4(glm::value_ptr(metallic_roughness_), "u_metallic_roughness");
	shader_->ApplyVector4(glm::value_ptr(base_reflectivity_), "u_base_reflectivity");
	sphere_->DrawIndexed();
}

void PbrLightingApp::ProcessInput()
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
		light_position_.x += 0.1f;
	}

	if (key_state_[GLFW_KEY_K])
	{
		light_position_.x -= 0.1f;
	}
}
