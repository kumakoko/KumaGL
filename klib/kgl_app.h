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
/*!
 * \file kgl_app.h
 * \date 2017/12/31 13:34
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: 应用程序类的基类
 *
 * \note
*/

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
        /// 类<see cref="App"/>的构造函数
        /// </summary>
        App();

        /// <summary>
        /// 类 <see cref="App"/>的析构函数
        /// </summary>
        virtual ~App();

        /// <summary>
        /// 初始化主窗口
        /// </summary>
        /// <param name="wnd_width">窗口的宽度</param>
        /// <param name="wnd_height">窗口的高度</param>
        /// <param name="wnd_resizable">窗口可以调整大小吗</param>
        /// <param name="wnd_title">窗口的标题</param>
        /// <param name="context_version_major">GL设备上下文的主版本</param>
        /// <param name="context_version_minor">GL设备上下文的次版本</param>
        /// <param name="profile">GL的profile.</param>
        virtual void InitWindow(int32_t wnd_width, int32_t wnd_height, bool wnd_resizable, const char* wnd_title, int32_t context_version_major = 3, int32_t context_version_minor = 3, App::GLProfile profile = App::CORE);
        
        /// <summary>
        /// 初始化OpenGL渲染器
        /// </summary>
        virtual void InitRenderer();

        /// <summary>
        /// 初始化GUI系统
        /// </summary>
        void InitGuiSystem(bool use_gui = true);

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
        
        /// <summary>
        /// 点击鼠标时的事件响应
        /// </summary>
        /// <param name="window">接受消息的window窗口实例指针</param>
        /// <param name="received_event">窗口接受到的事件</param>
        /// <param name="button">鼠标上的按键</param>
        /// <param name="action">按键动作，要么是GLFW_PRESS，要么是GLFW_RELEASE</param>
        static void MouseButtonCallback(GLFWwindow* window, int received_event, int button, int action);

        /// <summary>
        /// GLFWs the error callback.
        /// </summary>
        /// <param name="error_code">The error_code.</param>
        /// <param name="err_str">The err_str.</param>
        static void GLFWErrorCallback(int error_code, const char* err_str);
        
        /// <summary>
        /// Characters the callback.
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="x">The x.</param>
        static void CharCallback(GLFWwindow *window, unsigned int x);
    protected:
        static App* s_instance_;
    protected:
        /// <summary>
        /// 初始化场景中使用到的材质结构
        /// </summary>
        virtual void InitMaterials();

        /// <summary>
        /// 初始化场景中的模型
        /// </summary>
        virtual void InitModel();

        /// <summary>
        /// 初始化场景中的各个shader
        /// </summary>
        virtual void InitShaders();

        /// <summary>
        /// 初始化场景中的各个动态实时光源
        /// </summary>
        virtual void InitLights();

        /// <summary>
        /// 初始化场景的主摄像机
        /// </summary>
        virtual void InitMainCamera();

        /// <summary>
        /// 初始化字体库
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
        /// 渲染每一帧
        /// </summary>
        virtual void RenderScene();

        /// <summary>
        /// 渲染GUI系统
        /// </summary>
        virtual void RenderGUI();

        void PreRenderGUI();

        void PostRenderGUI();

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

        /// <summary>
        /// Called when [character action].
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="x">The x.</param>
        virtual void OnCharAction(GLFWwindow *window, unsigned int x);

        /// <summary>
        /// Called when [GLFW error callback].
        /// </summary>
        /// <param name="error_code">The error_code.</param>
        /// <param name="err_str">The err_str.</param>
        virtual void OnGLFWErrorCallback(int error_code, const char* err_str);

        virtual void OnMouseButtonAction(GLFWwindow* window, int received_event, int button, int action);

        const char* GetGLErrorDescription(GLenum err);

        void CheckGLError();
    protected:
        GLFWwindow*         window_handle_ = nullptr;
        CameraSPtr          main_camera_;
        std::bitset<384>    key_state_;     // 维护键盘是否按下的状态bit
        int32_t             window_width_ = 0;
        int32_t             window_height_ = 0;
        GLProfile           gl_profile_ = GLProfile::ANY;
        bool                use_gui_;
    };
}

#endif // kgl_app_h__

