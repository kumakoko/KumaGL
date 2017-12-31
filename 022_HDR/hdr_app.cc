// 展示使用HDR的技术
#include "hdr_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_debug_tool.h"


HDRApp::HDRApp()
{

}

HDRApp::~HDRApp()
{
	
}

void HDRApp::InitScene()
{
	App::InitScene();
}

void HDRApp::PreRenderFrame()
{
	App::PreRenderFrame();
}

void HDRApp::RenderFrame()
{

}

void HDRApp::InitMaterials()
{
	App::InitMaterials();
}

void HDRApp::InitModel()
{
	
}

void HDRApp::InitShaders()
{
	
}

void HDRApp::InitLights()
{
	
}

void HDRApp::InitMainCamera()
{
}

void HDRApp::InitFont()
{
	App::InitFont();
}

void HDRApp::ProcessInput()
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


	if (key_state_[GLFW_KEY_DOWN])
	{
		
	}

	if (key_state_[GLFW_KEY_UP])
	{
		
	}

	if (key_state_[GLFW_KEY_LEFT])
	{
		
	}

	if (key_state_[GLFW_KEY_RIGHT])
	{
		
	}

	if (key_state_[GLFW_KEY_HOME])
	{
		
	}

	if (key_state_[GLFW_KEY_END])
	{
		
	}
}

void HDRApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);
}

void HDRApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void HDRApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}