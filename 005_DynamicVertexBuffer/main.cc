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
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"
#include "dynamic_vertex_buffer_app.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"FreeImage.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glfw3dll.lib")
#pragma comment(lib,"assimp-vc142-mtd.lib")
#pragma comment(lib,"vld.lib")
#pragma comment(lib,"libiconv.lib")
#pragma comment(lib,"freetype.lib")
#pragma comment(lib,"imgui_d.lib")
#pragma comment(lib,"klib_d.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")
#else
#pragma comment(lib,"FreeImage.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glfw3dll.lib")
#pragma comment(lib,"assimp-vc142-mt.lib")
#pragma comment(lib,"vld.lib")
#pragma comment(lib,"libiconv.lib")
#pragma comment(lib,"freetype.lib")
#pragma comment(lib,"imgui.lib")
#pragma comment(lib,"klib.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")
#endif

void main()
{
    DynamicVertexBufferApp* app = nullptr;

    try
    {
        app = new DynamicVertexBufferApp();
        app->InitWindow(1024, 768, false, "005_DynamicVertexBuffer");
        app->InitRenderer();
        app->InitScene();
        app->Run();
    }
    catch (kgl::Error e)
    {
        e.Notify();
    }
    catch (std::exception e)
    {
        std::wstring excepiton_desc;
        kgl::StringConvertor::ANSItoUTF16LE(e.what(), excepiton_desc);
#ifdef WIN32
        ::MessageBox(NULL, excepiton_desc.c_str(), L"Unhandled Exception, aborting", MB_OK | MB_ICONERROR);
#endif
    }

    delete app;
}