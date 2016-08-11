// 应用程序类

#include "kgl_lib_pch.h"
#include "kgl_app.h"
#include "kgl_error.h"
#include "kgl_defines.h"

namespace kgl
{
	App* App::s_instance_ = nullptr;

	App::App() :window_handle_(nullptr)
	{
		s_instance_ = this;
		key_state_.reset();
	}

	App::~App()
	{
		main_camera_.reset();
		renderer_.reset();

		glfwTerminate();
		s_instance_ = nullptr;
	}

	void App::InitWindow(int wnd_width, int wnd_height, bool wnd_resizable, const char* wnd_title, int context_version_major, int context_version_minor, App::GLProfile profile)
	{
		std::vector<std::string> error_desc_array;
		std::vector<GLenum> error_code_array;

		glfwInit();
//		THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_version_major);
//		THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_version_minor);
//		THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

		switch (profile)
		{
		case App::ANY:
		{
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
//			THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
		}
		break;
		case App::CORE:
		{
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//			THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
		}
		break;
		case App::COMPAT:
		{
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
//			THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
		}
		break;
		}

		glfwWindowHint(GLFW_RESIZABLE, wnd_resizable ? GL_TRUE : GL_FALSE);
//		THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

		window_handle_ = glfwCreateWindow(wnd_width, wnd_height, wnd_title, nullptr, nullptr);
//		THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

		glfwMakeContextCurrent(window_handle_);
//		THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

		// Set the required callback functions
		glfwSetKeyCallback(window_handle_, App::KeyCallback);
		glfwSetCursorPosCallback(window_handle_, App::MouseCallback);
		glfwSetScrollCallback(window_handle_, App::ScrollCallback);

		// GLFW Options
		//glfwSetInputMode(window_handle_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void App::InitRenderer()
	{
		std::vector<std::string> error_desc_array;
		std::vector<GLenum> error_code_array;

		renderer_ = std::make_shared<Renderer>();
		renderer_->Init(true);

		main_camera_ = std::make_shared<Camera>();
		int viewport_with = 0;
		int viewport_heigth = 0;

		glfwGetFramebufferSize(window_handle_, &viewport_with, &viewport_heigth);
//		THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
		
		main_camera_->SetViewport(0, 0, viewport_with, viewport_heigth);
	}

	void App::InitScene()
	{

	}

	void App::Run()
	{
		std::vector<std::string> error_desc_array;
		std::vector<GLenum> error_code_array;
		bool is_not_closed = true;

		do
		{
			is_not_closed = !glfwWindowShouldClose(window_handle_);

			if (!is_not_closed)
				break;

			glfwPollEvents();
			this->ProcessInput();
			this->PreRenderFrame();
			this->RenderFrame();
			this->PostRenderFrame();
		}
		while (is_not_closed);
	}

	void App::PreRenderFrame()
	{
		renderer_->Clear();
	}

	void App::RenderFrame()
	{

	}

	void App::PostRenderFrame()
	{
		std::vector<std::string> error_desc_array;
		std::vector<GLenum> error_code_array;
		glfwSwapBuffers(window_handle_);
		// THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
	}

	void App::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		s_instance_->OnKeyAction(window, key, scancode, action, mode);
	}

	void App::MouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		s_instance_->OnMouseAction(window, xpos, ypos);
	}
	
	void App::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		s_instance_->OnScrollAction(window,  xoffset,  yoffset);
	}

	void App::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		if (key >= 0 && key < 384)
		{
			if (action == GLFW_RELEASE)
				key_state_.reset(key);
			else if (action == GLFW_PRESS)
				key_state_.set(key);
		}
	}

	void App::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
	{

	}

	void App::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
	{

	}

	void App::ProcessInput()
	{

	}
}