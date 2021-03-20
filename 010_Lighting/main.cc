// 简单的纹理应用示例
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"
#include "lighting_app.h"

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
    LightingApp* app = nullptr;

    try
    {
        app = new LightingApp();
        app->InitWindow(1024, 768, false, "010_Lighting");
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