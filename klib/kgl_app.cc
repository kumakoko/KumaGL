// 应用程序类
#include "kgl_lib_pch.h"
#include "kgl_app.h"
#include "kgl_error.h"
#include "kgl_defines.h"
#include "kgl_texture_manager.h"
#include "kgl_font_renderer.h"

namespace kgl
{
    App* App::s_instance_ = nullptr;

    App::App()
    {
        KFontRenderer::MakeInstance();
        KTextureManager::MakeInstance();
        s_instance_ = this;
        key_state_.reset();
    }

    App::~App()
    {
        KFontRenderer::DeleteInstance();
        KTextureManager::DeleteInstance();
        main_camera_.reset();
        glfwTerminate();
        s_instance_ = nullptr;
    }

    void App::InitWindow(int32_t wnd_width, int32_t wnd_height, bool wnd_resizable, const char* wnd_title, int32_t context_version_major /*= 3*/, int32_t context_version_minor /*= 3*/, App::GLProfile profile /*= App::CORE*/)
    {
        std::vector<std::string> error_desc_array;
        std::vector<GLenum> error_code_array;

        int ret = glfwInit();
		
		if (!ret)
		{
			exit(EXIT_FAILURE);
		}

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_version_minor);

#if defined(__APPLE__) && defined(__MACH__)
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif defined(WIN32) || defined(_WIN32)
		switch (profile)
		{
		case App::ANY:
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
			break;
		case App::CORE:
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			break;
		case App::COMPAT:
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
			break;
		}
#endif

        glfwWindowHint(GLFW_RESIZABLE, wnd_resizable ? GL_TRUE : GL_FALSE);

        window_handle_ = glfwCreateWindow(wnd_width, wnd_height, wnd_title, nullptr, nullptr);

		if (nullptr == window_handle_)
		{
            // THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
            throw Error(L"无法创建GL窗口，程序必须退出", __FILE__, __LINE__);
			//exit(EXIT_FAILURE);
		}

        glfwMakeContextCurrent(window_handle_);

        // 设置各种回调函数
        glfwSetKeyCallback(window_handle_, App::KeyCallback);
        glfwSetCursorPosCallback(window_handle_, App::MouseCallback);
        glfwSetScrollCallback(window_handle_, App::ScrollCallback);
        glfwSetWindowSizeCallback(window_handle_, App::SizeChangedCallback);

        // GLFW Options
        //glfwSetInputMode(window_handle_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		window_height_ = wnd_height;
		window_width_ = wnd_width;
    }

    void App::InitRenderer()
    {
        std::vector<std::string> error_desc_array;
        std::vector<GLenum> error_code_array;

		glewExperimental = GL_TRUE; // : GL_FALSE;
		GLenum r = glewInit();

		if (r != GLEW_OK)
		{
			std::wstringstream wss;
			wss << L"Failed to initialise GLEW" << std::endl;
			wss << L"OpenGL Error code : " << glGetError() << std::endl;
			throw Error(wss.str(), __FILE__, __LINE__);
			return;
		}

        main_camera_ = std::make_shared<Camera>();
        int viewport_with = 0;
        int viewport_heigth = 0;

        glfwGetFramebufferSize(window_handle_, &viewport_with, &viewport_heigth);
        main_camera_->SetViewport(0, 0, viewport_with, viewport_heigth);
    }

    void App::InitScene()
    {
        this->InitMainCamera();
        this->InitLight();
        this->InitMaterial();

        // 因为model要使用到shader，所以务必要保证
        // shader在Model前完成初始化
        this->InitShader();
        this->InitModel();      
        this->InitFont();
    }

    void App::InitMaterial()
    {
    }

    void App::InitModel()
    {
    }

    void App::InitShader()
    {
    }

    void App::InitLight()
    {
    }

    void App::InitMainCamera()
    {

    }

    void App::InitFont()
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
        glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    void App::SizeChangedCallback(GLFWwindow* window, int width, int height)
    {
        s_instance_->OnSizeChangedAction(window, width, height);
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

    void App::OnSizeChangedAction(GLFWwindow* window, int width, int height)
    {

    }

    void App::ProcessInput()
    {

    }
}
