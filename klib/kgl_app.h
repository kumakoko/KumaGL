// 应用程序类
#ifndef kgl_app_h__
#define kgl_app_h__

#include "kgl_camera.h"

namespace kgl
{
    class App
    {
    public:
        enum GLProfile
        {
            ANY,
            CORE,
            COMPAT
        };
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="App"/> class.
        /// </summary>
        App();

        /// <summary>
        /// Finalizes an instance of the <see cref="App"/> class.
        /// </summary>
        virtual ~App();

        /// <summary>
        /// Initializes the window.
        /// </summary>
        /// <param name="wnd_width">The wnd_width.</param>
        /// <param name="wnd_height">The wnd_height.</param>
        /// <param name="wnd_resizable">The wnd_resizable.</param>
        /// <param name="wnd_title">The wnd_title.</param>
        /// <param name="context_version_major">The context_version_major.</param>
        /// <param name="context_version_minor">The context_version_minor.</param>
        /// <param name="profile">The profile.</param>
        virtual void InitWindow(int32_t wnd_width, int32_t wnd_height, bool wnd_resizable, const char* wnd_title, int32_t context_version_major = 3, int32_t context_version_minor = 3, App::GLProfile profile = App::CORE);
        
        /// <summary>
        /// Initializes the renderer.
        /// </summary>
        virtual void InitRenderer();

        /// <summary>
        /// Initializes the scene.
        /// </summary>
        virtual void InitScene();

        /// <summary>
        /// Runs this instance.
        /// </summary>
        void Run();

        /// <summary>
        /// Keys the callback.
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="key">The key.</param>
        /// <param name="scancode">The scancode.</param>
        /// <param name="action">The action.</param>
        /// <param name="mode">The mode.</param>
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

        /// <summary>
        /// Mouses the callback.
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="xpos">The xpos.</param>
        /// <param name="ypos">The ypos.</param>
        static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

        /// <summary>
        /// Scrolls the callback.
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="xoffset">The xoffset.</param>
        /// <param name="yoffset">The yoffset.</param>
        static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        /// <summary>
        /// Sizes the changed callback.
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        static void SizeChangedCallback(GLFWwindow* window, int width, int height);
    protected:
        static App* s_instance_;
    protected:
        /// <summary>
        /// Initializes the material.
        /// </summary>
        virtual void InitMaterial();

        /// <summary>
        /// Initializes the model.
        /// </summary>
        virtual void InitModel();

        /// <summary>
        /// Initializes the shader.
        /// </summary>
        virtual void InitShader();

        /// <summary>
        /// Initializes the light.
        /// </summary>
        virtual void InitLight();

        /// <summary>
        /// Initializes the main camera.
        /// </summary>
        virtual void InitMainCamera();

        /// <summary>
        /// Initializes the font.
        /// </summary>
        virtual void InitFont();

        /// <summary>
        /// Processes the input.
        /// </summary>
        virtual void ProcessInput();

        /// <summary>
        /// Pres the render frame.
        /// </summary>
        virtual void PreRenderFrame();

        /// <summary>
        /// Renders the frame.
        /// </summary>
        virtual void RenderFrame();

        /// <summary>
        /// Posts the render frame.
        /// </summary>
        virtual void PostRenderFrame();

        /// <summary>
        /// Called when [key action].
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="key">The key.</param>
        /// <param name="scancode">The scancode.</param>
        /// <param name="action">The action.</param>
        /// <param name="mode">The mode.</param>
        virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode);

        /// <summary>
        /// Called when [mouse action].
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="xpos">The xpos.</param>
        /// <param name="ypos">The ypos.</param>
        virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos);

        /// <summary>
        /// Called when [scroll action].
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="xoffset">The xoffset.</param>
        /// <param name="yoffset">The yoffset.</param>
        virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset);

        /// <summary>
        /// Called when [size changed action].
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        virtual void OnSizeChangedAction(GLFWwindow* window, int width, int height);
    protected:
        GLFWwindow*         window_handle_ = nullptr;
        CameraSPtr          main_camera_;
        std::bitset<384>    key_state_;     // 维护键盘是否按下的状态bit
		int32_t				window_width_ = 0;
		int32_t				window_height_ = 0;
    };
}

#endif // kgl_app_h__

