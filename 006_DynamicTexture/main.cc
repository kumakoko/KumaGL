// 动态改变顶点缓冲区数据的应用示例
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"
#include "dynamic_texture_app.h"


void main()
{
	DynamicTextureApp* app = nullptr;

	try
	{
		app = new DynamicTextureApp();
		app->InitWindow(800, 600, false, "006_DynamicTexture");
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