// 简单的纹理应用示例
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"
#include "texture_app.h"


int main()
{
	TextureApp* app = nullptr;

	try
	{
        kgl::App::GLProfile profile = kgl::App::CORE;
#if defined(WIN32) || defined(_WIN32)
        int context_version_major = 3;
        int context_version_minor = 3;
#elif defined(__APPLE__) && defined(__MACH__)
        int context_version_major = 3;
        int context_version_minor = 3;
#endif
        app = new TextureApp();
        app->InitWindow(800, 600, false, "003_Texture",context_version_major, context_version_minor, profile );
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
    return 0;
}
