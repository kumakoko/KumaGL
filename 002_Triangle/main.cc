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
        const char* title = "002_Triangle";
        app = new TriangleApp();
        app->InitWindow(1024, 768, false, title, context_version_major, context_version_minor, profile);
        app->InitRenderer();
        app->InitGuiSystem();
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
