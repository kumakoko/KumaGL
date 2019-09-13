// 各种图像操作
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"
#include "image_effect_app.h"

void main()
{
    ImageEffectApp* app = nullptr;

    try
    {
        app = new ImageEffectApp();
        app->InitWindow(1024, 768, false, "040 Image Effect");
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