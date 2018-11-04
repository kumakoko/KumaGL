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
 * \file kgl_imgui_opengl_bridge.h
 * \date 2018/11/03 13:55
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: ImGui和OpenGL的桥接器
 *
 * \note
*/
#ifndef kgl_imgui_opengl_bridge_h__
#define kgl_imgui_opengl_bridge_h__

#if !defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)     \
 && !defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)     \
 && !defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)     \
 && !defined(IMGUI_IMPL_OPENGL_LOADER_CUSTOM)
#define IMGUI_IMPL_OPENGL_LOADER_GL3W
#endif

namespace kgl
{
    class ImGuiOpenGLBridge
    {
    private:
        // ImGui所使用的GLSL的版本字符串
        static std::string  s_glsl_versin_str_;

        static GLuint       g_FontTexture;
        static GLuint       g_ShaderHandle;
        static GLuint       g_VertHandle;
        static GLuint       g_FragHandle;
        static int          g_AttribLocationTex;
        static int          g_AttribLocationProjMtx;
        static int          g_AttribLocationPosition;
        static int          g_AttribLocationUV;
        static int          g_AttribLocationColor;
        static unsigned int g_VboHandle;
        static unsigned int g_ElementsHandle;
    public:
        /// <summary>
        /// 指定ImGui所使用的glsl版本字符串
        /// </summary>
        /// <param name="glsl_version">ImGui所使用的glsl版本字符串</param>
        /// <returns>一直返回true</returns>
        static bool Init(const char* glsl_version = nullptr);

        /// <summary>
        /// 关闭ImGui和OpenGL的桥接器，释放着色器，释放字体纹理
        /// </summary>
        static void Shutdown();

        /// <summary>
        /// Ims the gui_ implementation open g L3_ new frame.
        /// </summary>
        static void ImGui_ImplOpenGL3_NewFrame();

        /// <summary>
        /// Ims the gui_ implementation open g L3_ render draw data.
        /// </summary>
        /// <param name="draw_data">The draw_data.</param>
        static void RenderDrawData(ImDrawData* draw_data);

        /// <summary>
        /// 创建渲染GUI界面时所用到的字体纹理
        /// </summary>
        /// <returns>bool.</returns>
        static bool CreateFontsTexture();

        /// <summary>
        /// Ims the gui_ implementation open g L3_ destroy fonts texture.
        /// </summary>
        static void DestroyFontsTexture();

        /// <summary>
        /// Ims the gui_ implementation open g L3_ create device objects.
        /// </summary>
        /// <returns>bool.</returns>
        static bool CreateDeviceObjects();
    };
}



#endif // kgl_imgui_opengl_bridge_h__