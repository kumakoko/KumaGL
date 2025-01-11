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
#include "kgl_lib_pch.h"
#include "kgl_app.h"
#include "kgl_error.h"
#include "kgl_defines.h"
#include "kgl_texture_manager.h"
#include "kgl_font_renderer.h"
#include "kgl_string_convertor.h"
#include "kgl_debug_tool.h"
#include "kgl_imgui_glfw_bridge.h"
#include "kgl_imgui_opengl_bridge.h"

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
        if (use_gui_)
        {
            ImGuiOpenGLBridge::Shutdown();
            ImGuiGlfwBridge::Shutdown();
            ImGui::DestroyContext();
        }

        KFontRenderer::DeleteInstance();
        KTextureManager::DeleteInstance();
        main_camera_.reset();
        glfwTerminate();
        s_instance_ = nullptr;
    }

    void App::InitWindow(int32_t wnd_width, int32_t wnd_height, bool wnd_resizable, const char* wnd_title, int32_t context_version_major /*= 3*/,
        int32_t context_version_minor /*= 3*/, App::GLProfile profile /*= App::CORE*/)
    {
        std::vector<std::string> error_desc_array;
        std::vector<GLenum> error_code_array;

        // 注册glfw错误回调函数函数
        glfwSetErrorCallback(GLFWErrorCallback);

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
            throw Error(L"无法创建GL窗口，程序必须退出", __FILE__, __LINE__);
        }

        glfwMakeContextCurrent(window_handle_);

        // 设置各种回调函数
        glfwSetKeyCallback(window_handle_, App::KeyCallback);
        glfwSetCursorPosCallback(window_handle_, App::MouseCallback);
        glfwSetScrollCallback(window_handle_, App::ScrollCallback);
        glfwSetWindowSizeCallback(window_handle_, App::SizeChangedCallback);
        glfwSetMouseButtonCallback(window_handle_, App::MouseButtonCallback);

        // GLFW Options
        window_height_ = wnd_height;
        window_width_ = wnd_width;
        gl_profile_ = profile;

        EnableVSync(true);
    }

    void App::InitGuiSystem(bool use_gui, bool install_callbacks, bool install_key_callback, bool install_scroll_callback,
        bool install_char_callback, bool install_mouse_btn_callback)
    {
        this->use_gui_ = use_gui;

        if (use_gui_)
        {
            // 设置ImGUI
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.WantCaptureKeyboard = false;

            // 把当前窗口句柄注册到ImGui，建立起鼠标光标，键盘等映射关系
            ImGuiGlfwBridge::Init(this->window_handle_, install_callbacks, install_key_callback,
                install_scroll_callback, install_char_callback, install_mouse_btn_callback);
            // 设置渲染ImGui的glsl版本，使用1.30版本就可以了
            ImGuiOpenGLBridge::Init("#version 130");

            // 设置界面的风格
            ImGui::StyleColorsDark();

            // 装载字库文件
            std::string font_file = bfs::current_path().string() + std::string("/resources/font/fzht_sim.ttf");
            const ImWchar* glyph_ranges = io.Fonts->GetGlyphRangesChineseFull();
            ImFont* font = io.Fonts->AddFontFromFileTTF(font_file.c_str(), 18.0f, nullptr, glyph_ranges);

            if (!font)
            {
                std::wstringstream wss;
                wss << L"无法初始化获取指定的字体库" << std::endl;
                wss << L"错误描述：无法初始化获取指定的字体库: " << StringConvertor::ANSItoUTF16LE(font_file.c_str()) << std::endl;
                wss << L"程序必须退出";
                throw Error(wss.str(), __FILE__, __LINE__);
            }
        }
    }

    void App::EnableVSync(bool enable)
    {
        enable_v_sync_ = enable;
        glfwSwapInterval(enable_v_sync_ ? 1 : 0);
    }

    bool App::IsVSyncEnabled() const
    {
        return enable_v_sync_;
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
            wss << L"Failed to initialize GLEW" << std::endl;
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
        this->InitLights();
        this->InitMaterials();

        // 因为model要使用到shader，所以务必要保证
        // shader在Model前完成初始化
        this->InitShaders();
        this->InitModel();
        this->InitFont();
    }

    void App::InitMaterials()
    {
    }

    void App::InitModel()
    {
    }

    void App::InitShaders()
    {
    }

    void App::InitLights()
    {
    }

    void App::InitMainCamera()
    {

    }

    void App::InitFont()
    {

    }

    void App::UpdateScene()
    {

    }

    const char* App::GetGLErrorDescription(GLenum err)
    {
        switch (err)
        {
        case GL_NO_ERROR:          return "No error \n";
        case GL_INVALID_ENUM:      return "!!! OpenGL Error: Invalid enum \n";
        case GL_INVALID_VALUE:     return "!!! OpenGL Error: Invalid value \n";
        case GL_INVALID_OPERATION: return "!!! OpenGL Error: Invalid operation \n";
        case GL_STACK_OVERFLOW:    return "!!! OpenGL Error: Stack overflow \n";
        case GL_STACK_UNDERFLOW:   return "!!! OpenGL Error: Stack underflow \n";
        case GL_OUT_OF_MEMORY:     return "!!! OpenGL Error: Out of memory \n";
        default:                   return "!!! OpenGL Error: Unknown error \n";
        }
    }

    void App::CheckGLError()
    {
        while (true)
        {
            const GLenum err = glGetError();

            if (GL_NO_ERROR == err)
                break;

            DebugTool::OutputDebugMessage(GetGLErrorDescription(err));
        }
    }

    void App::Run()
    {
        std::vector<std::string> error_desc_array;
        std::vector<GLenum> error_code_array;
        bool is_not_closed = true;

        current_frame_time_ = last_frame_time_ = std::chrono::steady_clock::now();

        do
        {
            is_not_closed = !glfwWindowShouldClose(window_handle_);

            if (!is_not_closed)
                break;

            // 算出时间差
            last_frame_time_ = current_frame_time_;
            current_frame_time_ = std::chrono::steady_clock::now();
            frame_time_delta_ = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time_ - last_frame_time_).count();

            this->CheckGLError();
            glfwPollEvents();
            this->ProcessInput();
            this->UpdateScene();
            this->PreRenderFrame();
            this->RenderScene();
            this->PreRenderGUI();
            this->RenderGUI();
            this->PostRenderGUI();
            this->PostRenderFrame();
        } while (is_not_closed);
    }

    void App::PreRenderFrame()
    {
        glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void App::RenderScene()
    {

    }

    void App::PostRenderGUI()
    {
        if (use_gui_)
        {
            ImGui::Render();
            ImGuiOpenGLBridge::RenderDrawData(ImGui::GetDrawData());
        }
    }

    void App::PreRenderGUI()
    {
        if (use_gui_)
        {
            ImGuiOpenGLBridge::ImGui_ImplOpenGL3_NewFrame();
            ImGuiGlfwBridge::ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
    }

    void App::RenderGUI()
    {
    }

    void App::PostRenderFrame()
    {
        glfwSwapBuffers(window_handle_);
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
        s_instance_->OnScrollAction(window, xoffset, yoffset);
    }

    void App::SizeChangedCallback(GLFWwindow* window, int width, int height)
    {
        s_instance_->OnSizeChangedAction(window, width, height);
    }

    void App::GLFWErrorCallback(int error_code, const char* err_str)
    {
        s_instance_->OnGLFWErrorCallback(error_code, err_str);
    }

    void App::MouseButtonCallback(GLFWwindow* window, int received_event, int button, int action)
    {
        s_instance_->OnMouseButtonAction(window, received_event, button, action);
    }

    void App::CharCallback(GLFWwindow* window, unsigned int x)
    {
        s_instance_->OnCharAction(window, x);
    }

    void App::OnCharAction(GLFWwindow* window, unsigned int x)
    {

    }

    void App::OnMouseButtonAction(GLFWwindow* window, int received_event, int button, int action)
    {
    }

    void App::OnGLFWErrorCallback(int error_code, const char* err_str)
    {
        std::wstring error_code_str;

        switch (error_code)
        {
        default:error_code_str = L"Unknown Error Code"; break;
        case GLFW_NOT_INITIALIZED:error_code_str = L"GLFW Error Code: GLFW_NOT_INITIALIZED"; break;
        case GLFW_NO_CURRENT_CONTEXT:error_code_str = L"GLFW Error Code: GLFW_NO_CURRENT_CONTEXT"; break;
        case GLFW_INVALID_ENUM:error_code_str = L"GLFW Error Code: GLFW_INVALID_ENUM"; break;
        case GLFW_INVALID_VALUE:error_code_str = L"GLFW Error Code: GLFW_INVALID_VALUE"; break;
        case GLFW_OUT_OF_MEMORY:error_code_str = L"GLFW Error Code: GLFW_OUT_OF_MEMORY"; break;
        case GLFW_API_UNAVAILABLE:error_code_str = L"GLFW Error Code: GLFW_API_UNAVAILABLE"; break;
        case GLFW_VERSION_UNAVAILABLE:error_code_str = L"GLFW Error Code: GLFW_VERSION_UNAVAILABLE"; break;
        case GLFW_PLATFORM_ERROR:error_code_str = L"GLFW Error Code: GLFW_PLATFORM_ERROR"; break;
        case GLFW_FORMAT_UNAVAILABLE:error_code_str = L"GLFW Error Code: GLFW_FORMAT_UNAVAILABLE"; break;
        case GLFW_NO_WINDOW_CONTEXT:error_code_str = L"GLFW Error Code: GLFW_NO_WINDOW_CONTEXT"; break;
        }

        std::wstringstream wss;
        wss << error_code_str << std::endl;
        wss << StringConvertor::UTF8toUTF16LE(err_str);

        throw Error(wss.str(), __FILE__, __LINE__);
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
