// 展示一个简单的三角形
#include "sky_box_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_primitive_tool.h"

SkyBoxApp::SkyBoxApp()
{
}

SkyBoxApp::~SkyBoxApp()
{
	KGL_SAFE_DELETE(sky_box_);
	KGL_SAFE_DELETE(scene_obj_shader_);
	floor_.reset();
	floor_texture_.reset();
}

void SkyBoxApp::InitModel()
{
	// 初始化纹理
	kgl::TextureParams texture_param;
	texture_param.wrap_s_mode = GL_REPEAT;
	texture_param.wrap_t_mode = GL_REPEAT;
	texture_param.mag_filter_mode = GL_LINEAR;
	texture_param.min_filter_mode = GL_LINEAR;
	texture_param.internal_format = GL_RGB;
	texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
	texture_param.src_img_format = GL_RGB;
	texture_param.used_mipmap = false;

	floor_texture_ = std::dynamic_pointer_cast<kgl::SourceTexture>(kgl::KTextureManager::GetInstance()->CreateTextureFromFile(
		"resources/image/floor.jpg", kgl::SOURCE_2D_TEXTURE, texture_param));
	floor_ = kgl::PrimitiveTool::BuildTexturedXZPlane(0.f, 10.f, 10.f, 10);

	sky_box_ = new kgl::SkyBox(main_camera_);

	const std::string& positive_x_file = "resources/image/sky_box/sp3/sp3right.jpg";
	const std::string& negative_x_file = "resources/image/sky_box/sp3/sp3left.jpg";
	const std::string& positive_y_file = "resources/image/sky_box/sp3/sp3right.jpg";
	const std::string& negative_y_file = "resources/image/sky_box/sp3/sp3left.jpg";
	const std::string& positive_z_file = "resources/image/sky_box/sp3/sp3front.jpg";
	const std::string& negative_z_file = "resources/image/sky_box/sp3/sp3back.jpg";

	sky_box_->LoadSkyBox(positive_x_file, negative_x_file, positive_y_file, negative_y_file, positive_z_file, negative_z_file);
}

void SkyBoxApp::InitShaders()
{
	// 初始化shader
	scene_obj_shader_ = new kgl::GPUProgram;
	/*
	scene_obj_shader_->CreateFromFile(
		"resources/shader/framework/sky_box_vs.glsl", 
		"resources/shader/framework/sky_box_fs.glsl", nullptr);
	*/
}

void SkyBoxApp::InitScene()
{
	App::InitScene();
}

void SkyBoxApp::InitMainCamera()
{
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(0.0f, 3.0f, 8.0f));
	main_camera_->SetMaxYawDegreePerFrame(1.0f);
	main_camera_->SetMaxPitchDegreePerFrame(1.0f);
}

void SkyBoxApp::RenderFrame()
{
	main_camera_->Update();
	
	// 绘制地板
	sky_box_->Draw();
	/*
	// 绘制地板
	glm::mat4 model_matrix;
	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();

	scene_obj_shader_->Use();
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(model_matrix), "model_matrix");
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
	scene_obj_shader_->ApplyTexture(floor_texture_, "source_texture_1", 0);

	floor_->Draw();
	*/
}

void SkyBoxApp::ProcessInput()
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
		main_camera_->ChangeYaw(1.f);
	}

	if (key_state_[GLFW_KEY_UP])
	{
		main_camera_->ChangePitch(1.f);
	}

	if (key_state_[GLFW_KEY_DOWN])
	{
		main_camera_->ChangePitch(-1.f);
	}

	if (key_state_[GLFW_KEY_LEFT])
	{
		main_camera_->ChangeYaw(5.f);
	}

	if (key_state_[GLFW_KEY_RIGHT])
	{
		main_camera_->ChangeYaw(-5.f);
	}
}

void SkyBoxApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);
}

void SkyBoxApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void SkyBoxApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}