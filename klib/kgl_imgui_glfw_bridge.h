/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

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
 * \file kgl_imgui_glfw_bridge.h
 * \date 2018/11/03 0:41
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: 整合ImGUI和glfw的桥接文件
 *
 * \note
*/
#ifndef kgl_imgui_glfw_bridge_h__
#define kgl_imgui_glfw_bridge_h__

namespace kgl
{
    class ImGuiGlfwBridge
    {
    private:
        static GLFWwindow* glfw_wnd_;
        static GLFWcursor* mouse_cursors_[ImGuiMouseCursor_COUNT];
        static double      time_;

        static bool        s_mouse_just_pressed_[5];

        /// <summary>
        /// 是否截取键盘按键输入并且注册对应的回调
        /// </summary>
        static bool        s_install_key_callback_;

        /// <summary>
        /// 是否截取鼠标滚轮的滚动输入并且注册对应的回调
        /// </summary>
        static bool        s_install_scroll_callback_;

        /// <summary>
        /// 是否截取鼠标的按键输入并且注册对应的回调
        /// </summary>
        static bool        s_install_mouse_btn_callback_;
        
        /// <summary>
        /// 是否截取字符信息的输入并且注册对应的回调
        /// </summary>
        static bool        s_install_char_callback_;
    public:
        /// <summary>
        /// 初始化ImGUI和GLFW的桥接器
        /// </summary>
        /// <param name="window">GLFW窗口实例指针</param>
        /// <param name="install_callbacks">是否安装对键盘鼠标等外设信息的响应回调</param>
        /// <param name="install_key_callback">是否响应对键盘输入并且安装回调</param>
        /// <param name="install_scroll_callback">T是否响应对鼠标滚轮的输入并且安装回调</param>
        /// <param name="install_char_callback">是否响应对字符信息的输入并且安装回调</param>
        /// <param name="install_mouse_btn_callback">是否响应对鼠标按键的输入并且安装回调</param>
        /// <returns>bool.</returns>
        static bool Init(GLFWwindow* window, bool install_callbacks, bool install_key_callback, bool install_scroll_callback,
            bool install_char_callback, bool install_mouse_btn_callback);

        /// <summary>
        /// Ims the gui_ implementation GLFW_ shutdown.
        /// </summary>
        static void Shutdown();

        /// <summary>
        /// Ims the gui_ implementation GLFW_ new frame.
        /// </summary>
        static void ImGui_ImplGlfw_NewFrame();

        /// <summary>
        /// Ims the gui_ implementation GLFW_ character callback.
        /// </summary>
        /// <param name="window">The window.</param>
        /// <param name="c">The c.</param>
        static void CharCallback(GLFWwindow* window, unsigned int c);

        /// <summary>
        /// Ims the gui_ implementation GLFW_ update mouse position and buttons.
        /// </summary>
        static void UpdateMousePosAndButtons();

        /// <summary>
        /// 更新鼠标光标
        /// </summary>
        static void UpdateMouseCursor();

        /// <summary>
        /// Ims the gui_ implementation GLFW_ install callbacks.
        /// </summary>
        /// <param name="window">The window.</param>
        static void InstallCallbacks(GLFWwindow* window);

        /// <summary>
        /// Ims the gui_ implementation GLFW_ key callback.
        /// </summary>
        /// <param name="">The .</param>
        /// <param name="key">The key.</param>
        /// <param name="">The .</param>
        /// <param name="action">The action.</param>
        /// <param name="mods">The mods.</param>
        static void KeyCallback(GLFWwindow*, int key, int, int action, int mods);

        /// <summary>
        /// Ims the gui_ implementation GLFW_ scroll callback.
        /// </summary>
        /// <param name="">The .</param>
        /// <param name="xoffset">The xoffset.</param>
        /// <param name="yoffset">The yoffset.</param>
        static void ScrollCallback(GLFWwindow*, double xoffset, double yoffset);

        /// <summary>
        /// Ims the gui_ implementation GLFW_ mouse button callback.
        /// </summary>
        /// <param name="">The .</param>
        /// <param name="button">The button.</param>
        /// <param name="action">The action.</param>
        /// <param name="">The .</param>
        static void MouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/);

        /// <summary>
        /// Ims the gui_ implementation GLFW_ set clipboard text.
        /// </summary>
        /// <param name="user_data">The user_data.</param>
        /// <param name="text">The text.</param>
        static void SetClipboardText(void* user_data, const char* text);

        /// <summary>
        /// Ims the gui_ implementation GLFW_ get clipboard text.
        /// </summary>
        /// <param name="user_data">The user_data.</param>
        /// <returns>const char *.</returns>
        static const char* GetClipboardText(void* user_data);
    };
}

#endif // kgl_imgui_glfw_bridge_h__
