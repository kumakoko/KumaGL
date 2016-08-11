#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"

void main()
{
	kgl::App* app = nullptr;

	try
	{
		kgl::App* app = new kgl::App();
		app->InitWindow(800, 600, false, "001_EmptyWindow");
		app->InitRenderer();
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