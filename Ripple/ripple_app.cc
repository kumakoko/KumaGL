//  演示3D水波的技术

#include "ripple_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_debug_tool.h"

// http://blog.csdn.net/dreamcs/article/details/7691690

RippleApp::RippleApp()
{
	
}

RippleApp::~RippleApp()
{
	
}

void RippleApp::InitScene()
{
	App::InitScene();
	
	
}

void RippleApp::PreRenderFrame()
{
	App::PreRenderFrame();
}

void RippleApp::RenderFrame()
{
	main_camera_->Update();
}

void RippleApp::WaterSimulation()
{

	
}


void RippleApp::HeightmapToNormal()
{
	
}

void RippleApp::RenderWater()
{

}


void RippleApp::InitMaterial()
{
	App::InitMaterial();
}

void RippleApp::InitModel()
{
	
}

void RippleApp::InitShader()
{

}

void RippleApp::InitLight()
{
	App::InitLight();
}

void RippleApp::InitMainCamera()
{
	
}

void RippleApp::InitFont()
{
	App::InitFont();
}

void RippleApp::ProcessInput()
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

void RippleApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);
}

void RippleApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void RippleApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}