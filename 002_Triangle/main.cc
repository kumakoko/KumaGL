// 展示一个简单的三角形

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"
#include "../klib/kgl_message_box.h"
#include "triangle_app.h"

int main()
{
	TriangleApp* app = nullptr;

	try
	{
#if defined(WIN32) || defined(_WIN32)
        int context_version_major = 3;
        int context_version_minor = 3;
        kgl::App::GLProfile profile = kgl::App::CORE;
#elif defined(__APPLE__) && defined(__MACH__)
        int context_version_major = 3;
        int context_version_minor = 3;
        kgl::App::GLProfile profile = kgl::App::CORE;
#endif
		app = new TriangleApp();
		app->InitWindow(800, 600, false, "002_Triangle",context_version_major, context_version_minor, profile );
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
		std::wstring exception_desc;
		kgl::StringConvertor::ANSItoUTF16LE(e.what(), exception_desc);
        kgl::MessageBox(std::wstring(L"002_Triangle : Unhandled Exception, aborting"),exception_desc);
	}

	delete app;
    
   
    return 0;
}
