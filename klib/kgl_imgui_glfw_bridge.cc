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
 * \file kgl_imgui_glfw_bridge.cc
 * \date 2018/11/03 0:44
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief
 *
 * TODO: long description
 *
 * \note
 */
#include "kgl_lib_pch.h"
#include "kgl_imgui_glfw_bridge.h"

namespace kgl
{
#define GLFW_HAS_WINDOW_TOPMOST     (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ GLFW_FLOATING
#define GLFW_HAS_WINDOW_HOVERED     (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ GLFW_HOVERED
#define GLFW_HAS_WINDOW_ALPHA       (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwSetWindowOpacity
#define GLFW_HAS_PER_MONITOR_DPI    (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetMonitorContentScale
#define GLFW_HAS_VULKAN             (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ glfwCreateWindowSurface
    GLFWwindow* ImGuiGlfwBridge::glfw_wnd_ = nullptr;
    GLFWcursor* ImGuiGlfwBridge::mouse_cursors_[ImGuiMouseCursor_COUNT] = { 0 };
    double ImGuiGlfwBridge::time_ = 0.0;

    /// <summary>
    /// The s_mouse_just_pressed_{CC2D43FA-BBC4-448A-9D0B-7B57ADF2655C}
    /// </summary>
    bool ImGuiGlfwBridge::s_mouse_just_pressed_[5] = { false, false, false, false, false };

    bool ImGuiGlfwBridge::s_install_key_callback_ = true;
    bool ImGuiGlfwBridge::s_install_scroll_callback_ = true;
    bool ImGuiGlfwBridge::s_install_mouse_btn_callback_ = true;
    bool ImGuiGlfwBridge::s_install_char_callback_ = true;

    const char* ImGuiGlfwBridge::GetClipboardText(void* user_data)
    {
        return glfwGetClipboardString((GLFWwindow*)user_data);
    }

    void ImGuiGlfwBridge::SetClipboardText(void* user_data, const char* text)
    {
        glfwSetClipboardString((GLFWwindow*)user_data, text);
    }

    void ImGuiGlfwBridge::MouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/)
    {
        if (action == GLFW_PRESS && button >= 0 && button < IM_ARRAYSIZE(s_mouse_just_pressed_))
            s_mouse_just_pressed_[button] = true;
    }

    void ImGuiGlfwBridge::ScrollCallback(GLFWwindow*, double xoffset, double yoffset)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += (float)xoffset;
        io.MouseWheel += (float)yoffset;
    }

    void ImGuiGlfwBridge::KeyCallback(GLFWwindow*, int key, int, int action, int mods)
    {
        ImGuiIO& io = ImGui::GetIO();

        if (action == GLFW_PRESS)
            io.KeysDown[key] = true;
        if (action == GLFW_RELEASE)
            io.KeysDown[key] = false;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
    }

    void ImGuiGlfwBridge::CharCallback(GLFWwindow*, unsigned int c)
    {
        ImGuiIO& io = ImGui::GetIO();
        if (c > 0 && c < 0x10000)
            io.AddInputCharacter((unsigned short)c);
    }

    void ImGuiGlfwBridge::InstallCallbacks(GLFWwindow* window)
    {
        if (s_install_mouse_btn_callback_)
            glfwSetMouseButtonCallback(window, MouseButtonCallback);
        if (s_install_scroll_callback_)
            glfwSetScrollCallback(window, ScrollCallback);
        if (s_install_key_callback_)
            glfwSetKeyCallback(window, KeyCallback);
        if (s_install_char_callback_)
            glfwSetCharCallback(window, CharCallback);
    }


    bool ImGuiGlfwBridge::Init(GLFWwindow* window, bool install_callbacks, bool install_key_callback, bool install_scroll_callback,
        bool install_char_callback,bool install_mouse_btn_callback)
    {
        glfw_wnd_ = window;
        time_ = 0.0;
        s_install_char_callback_ = install_char_callback;
        s_install_key_callback_ = install_key_callback;
        s_install_mouse_btn_callback_ = install_mouse_btn_callback;
        s_install_scroll_callback_ = install_scroll_callback;

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;        
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // 把GLFW定义的键位码映射到ImGuiIO的key map中
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        // 设置操作剪贴板的getter/setter函数
        io.SetClipboardTextFn = SetClipboardText;
        io.GetClipboardTextFn = GetClipboardText;
        io.ClipboardUserData = glfw_wnd_;
#if defined(_WIN32) || defined(WIN32)
        // win32 平台下取得输入法的窗口句柄
        io.ImeWindowHandle = reinterpret_cast<void*>(glfwGetWin32Window(glfw_wnd_));
#endif

        // 创建各个不同状态下的光标
        mouse_cursors_[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        mouse_cursors_[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        mouse_cursors_[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
        mouse_cursors_[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        mouse_cursors_[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        mouse_cursors_[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
        mouse_cursors_[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
        mouse_cursors_[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

        if (install_callbacks)
            InstallCallbacks(window);

        return true;
    }

    // 关闭ImGui和Glfw的桥接器
    void ImGuiGlfwBridge::Shutdown()
    {
        for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
        {
            glfwDestroyCursor(mouse_cursors_[cursor_n]);
            mouse_cursors_[cursor_n] = NULL;
        }
    }

    void ImGuiGlfwBridge::UpdateMousePosAndButtons()
    {
        // Update buttons
        ImGuiIO& io = ImGui::GetIO();
        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
        {
            // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
            io.MouseDown[i] = s_mouse_just_pressed_[i] || glfwGetMouseButton(glfw_wnd_, i) != 0;
            s_mouse_just_pressed_[i] = false;
        }

        // Update mouse position
        const ImVec2 mouse_pos_backup = io.MousePos;
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
#ifdef __EMSCRIPTEN__
        const bool focused = true; // Emscripten
#else
        const bool focused = glfwGetWindowAttrib(glfw_wnd_, GLFW_FOCUSED) != 0;
#endif
        if (focused)
        {
            if (io.WantSetMousePos)
            {
                glfwSetCursorPos(glfw_wnd_, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
            }
            else
            {
                double mouse_x, mouse_y;
                glfwGetCursorPos(glfw_wnd_, &mouse_x, &mouse_y);
                io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
            }
        }
    }

    void ImGuiGlfwBridge::UpdateMouseCursor()
    {
        ImGuiIO& io = ImGui::GetIO();
        if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(glfw_wnd_, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            return;

        ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
        if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
        {
            // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
            glfwSetInputMode(glfw_wnd_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else
        {
            // Show OS mouse cursor
            // FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
            glfwSetCursor(glfw_wnd_, mouse_cursors_[imgui_cursor] ? mouse_cursors_[imgui_cursor] : mouse_cursors_[ImGuiMouseCursor_Arrow]);
            glfwSetInputMode(glfw_wnd_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    void ImGuiGlfwBridge::ImGui_ImplGlfw_NewFrame()
    {
        ImGuiIO& io = ImGui::GetIO();
        IM_ASSERT(io.Fonts->IsBuilt());     // Font atlas needs to be built, call renderer _NewFrame() function e.g. ImGui_ImplOpenGL3_NewFrame() 

        // Setup display size
        int w, h;
        int display_w, display_h;
        glfwGetWindowSize(glfw_wnd_, &w, &h);
        glfwGetFramebufferSize(glfw_wnd_, &display_w, &display_h);
        io.DisplaySize = ImVec2((float)w, (float)h);
        io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

        // Setup time step
        double current_time = glfwGetTime();
        io.DeltaTime = time_ > 0.0 ? (float)(current_time - time_) : (float)(1.0f / 60.0f);
        time_ = current_time;

        UpdateMousePosAndButtons();
        UpdateMouseCursor();

        // Gamepad navigation mapping [BETA]
        memset(io.NavInputs, 0, sizeof(io.NavInputs));
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)
        {
            // Update gamepad inputs
#define MAP_BUTTON(NAV_NO, BUTTON_NO)       { if (buttons_count > BUTTON_NO && buttons[BUTTON_NO] == GLFW_PRESS) io.NavInputs[NAV_NO] = 1.0f; }
#define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float v = (axes_count > AXIS_NO) ? axes[AXIS_NO] : V0; v = (v - V0) / (V1 - V0); if (v > 1.0f) v = 1.0f; if (io.NavInputs[NAV_NO] < v) io.NavInputs[NAV_NO] = v; }
            int axes_count = 0, buttons_count = 0;
            const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
            const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
            MAP_BUTTON(ImGuiNavInput_Activate, 0);     // Cross / A
            MAP_BUTTON(ImGuiNavInput_Cancel, 1);     // Circle / B
            MAP_BUTTON(ImGuiNavInput_Menu, 2);     // Square / X
            MAP_BUTTON(ImGuiNavInput_Input, 3);     // Triangle / Y
            MAP_BUTTON(ImGuiNavInput_DpadLeft, 13);    // D-Pad Left
            MAP_BUTTON(ImGuiNavInput_DpadRight, 11);    // D-Pad Right
            MAP_BUTTON(ImGuiNavInput_DpadUp, 10);    // D-Pad Up
            MAP_BUTTON(ImGuiNavInput_DpadDown, 12);    // D-Pad Down
            MAP_BUTTON(ImGuiNavInput_FocusPrev, 4);     // L1 / LB
            MAP_BUTTON(ImGuiNavInput_FocusNext, 5);     // R1 / RB
            MAP_BUTTON(ImGuiNavInput_TweakSlow, 4);     // L1 / LB
            MAP_BUTTON(ImGuiNavInput_TweakFast, 5);     // R1 / RB
            MAP_ANALOG(ImGuiNavInput_LStickLeft, 0, -0.3f, -0.9f);
            MAP_ANALOG(ImGuiNavInput_LStickRight, 0, +0.3f, +0.9f);
            MAP_ANALOG(ImGuiNavInput_LStickUp, 1, +0.3f, +0.9f);
            MAP_ANALOG(ImGuiNavInput_LStickDown, 1, -0.3f, -0.9f);
#undef MAP_BUTTON
#undef MAP_ANALOG
            if (axes_count > 0 && buttons_count > 0)
                io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
            else
                io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
        }
    }
}