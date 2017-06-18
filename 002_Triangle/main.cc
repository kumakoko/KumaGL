// 展示一个简单的三角形

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"
#include "triangle_app.h"

void main()
{
	TriangleApp* app = nullptr;

	try
	{
		app = new TriangleApp();
		app->InitWindow(800, 600, false, "002_Triangle");
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