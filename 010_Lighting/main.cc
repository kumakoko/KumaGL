// 简单的纹理应用示例
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"
#include "lighting_app.h"


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