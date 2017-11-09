// 展示全屏模糊的渲染技术
#include "blur_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_debug_tool.h"

BlurApp::BlurApp()
{
}

BlurApp::~BlurApp()
{
	KGL_SAFE_DELETE(model_);
	model_shader_.reset();
}

void BlurApp::InitModel()
{
	const char* model_path = "resources/model/liberty.3ds";
	model_ = new kgl::StaticModel(kgl::VERTEX_TYPE_PN, model_path);
	std::size_t sz = model_->GetMeshCount();
	model_->ApplyShaderToModel(model_shader_);
	rs_depth_.Use();
}

void BlurApp::InitShader()
{
	model_shader_ = std::make_shared<kgl::GPUProgram>();
	std::vector<std::string> vs_file_paths;
	std::vector<std::string> fs_file_paths;
	std::vector<std::string> gs_file_path;
	vs_file_paths.push_back("resources/shader/015_blur_draw_model_vs.glsl");
	fs_file_paths.push_back("resources/shader/lighting_struct.glsl");
	fs_file_paths.push_back("resources/shader/lighting_fs.glsl");
	fs_file_paths.push_back("resources/shader/015_blur_draw_model_fs.glsl");
	model_shader_->CreateFromFile(vs_file_paths, fs_file_paths, gs_file_path, 3, 3, 0);
}

void BlurApp::InitScene()
{
	App::InitScene();
}

void BlurApp::InitMainCamera()
{
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(0.0f, 0.0f, -400.0f),0.f,0.f);
}

void BlurApp::InitLight()
{
	directional_light_.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	directional_light_.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	directional_light_.Direction = glm::vec3(0.0f, 0.0f, -1.0f);
	directional_light_.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void BlurApp::InitMaterial()
{
	// 白银材质
	material_.Ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f);
	material_.Diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
	material_.Specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);
	material_.Shininess = 0.4f * 128.f;
}

void BlurApp::RenderFrame()
{
	main_camera_->Update();

	glm::mat4 world_matrix;
	world_matrix = glm::translate(world_matrix, glm::vec3(0.0f, -35.0f, 0.0f));
	world_matrix = glm::rotate(world_matrix, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
	const glm::vec3& view_pos = main_camera_->GetPosition();

	rs_depth_.SetEnable(true);

	for (size_t mesh_index = 0; mesh_index < model_->GetMeshCount(); ++mesh_index)
	{
		model_->UseShaderForMesh(mesh_index);
		model_->ApplyLocalTransformMatrixToMesh(mesh_index, "local_transform_matrix");
		model_->ApplyMatrixToMesh(mesh_index, glm::value_ptr(world_matrix), "world_matrix");
		model_->ApplyMatrixToMesh(mesh_index, glm::value_ptr(view_matrix), "view_matrix");
		model_->ApplyMatrixToMesh(mesh_index, glm::value_ptr(projection_matrix), "projection_matrix");
		model_->ApplyVector3ToMesh(mesh_index, glm::value_ptr(view_pos), "view_pos");
		model_->ApplyDirectionalLightToMesh(mesh_index, &directional_light_, "directional_light");
		model_->ApplyMaterialToMesh(mesh_index, &material_, "material");
		model_->DrawSubset(mesh_index);
	}
}

void BlurApp::ProcessInput()
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
		main_camera_->Move(kgl::CameraDirection::UP);
	}

	if (key_state_[GLFW_KEY_U])
	{
		main_camera_->Move(kgl::CameraDirection::DOWN);
	}

	if (key_state_[GLFW_KEY_R])
	{
		main_camera_->ChangeYaw(0.01f);
	}

	if (key_state_[GLFW_KEY_UP])
	{
		main_camera_->ChangePitch(0.01f);
	}

	if (key_state_[GLFW_KEY_DOWN])
	{
		main_camera_->ChangePitch(-0.01f);
	}

	if (key_state_[GLFW_KEY_LEFT])
	{
		main_camera_->ChangeYaw(0.01f);
	}

	if (key_state_[GLFW_KEY_RIGHT])
	{
		main_camera_->ChangeYaw(-0.01f);
	}
}

void BlurApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);
}

void BlurApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void BlurApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}