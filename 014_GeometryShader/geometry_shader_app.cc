#include "geometry_shader_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_primitive_tool.h"

GeometryShaderApp::GeometryShaderApp()
{
}

GeometryShaderApp::~GeometryShaderApp()
{
	KGL_SAFE_DELETE(model_);
	normal_proc_shader_.reset();
}

void GeometryShaderApp::InitModel()
{
	const char* model_path = "resources/model/xd_tux_0.3ds";
	model_ = new kgl::StaticModel(kgl::VERTEX_TYPE_PN, model_path);
	std::size_t sz = model_->GetMeshCount();

	for (std::size_t s = 0; s < sz; ++s)
	{
		model_->ApplyShaderToMesh(s, normal_proc_shader_);
	}
}

void GeometryShaderApp::InitScene()
{
	rs_depth_.SetEnable(GL_TRUE);

	// 初始化shader
	normal_proc_shader_ = std::make_shared<kgl::GPUProgram>();

	normal_proc_shader_->CreateFromFile(
		"resources/shader/014_geometry_shader_vs.glsl",
		"resources/shader/014_geometry_shader_fs.glsl", 
		"resources/shader/014_geometry_shader_gs.glsl");
	
	// 初始化模型
	this->InitModel();

	// 初始化相机
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(0.0f, 3.0f, 8.0f));
}

void GeometryShaderApp::RenderFrame()
{
	
}

void GeometryShaderApp::ProcessInput()
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

void GeometryShaderApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);
}

void GeometryShaderApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void GeometryShaderApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}