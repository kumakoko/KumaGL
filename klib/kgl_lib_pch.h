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
/*!
* \file kgl_lib_pch.h
* \date 2017/12/31 13:45
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: 预编译头文件
*
* \note
*/
#ifndef kgl_lib_pch_h__
#define kgl_lib_pch_h__

#if defined(WIN32) || defined(_WIN32)
    #pragma execution_character_set("utf-8")
    #define U8
#else
    #define U8 u8
#endif
// 这个预处理在C++11里已经不再需要了，C++11可以指定字符串字面量的执行字符集了，u8”我”。就这么简单。但是vs2013并不支持这个功能。这篇
// 文章讲述的内容，并不在于如何把一个UTF8格式的C++字面量输出到控制台。而是在于通过这个例子来了解MSVC C++是如何处理UTF8中文字符的。

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <array>
#include <memory>
#include <exception>
#include <iosfwd>
#include <sstream>
#include <fstream>
#include <bitset>
#include <cstdint>
#include <functional>
#include <set>
#include <array>
#include <cstdlib>
#include <utility>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#if defined(WIN32) || defined(_WIN32)
    #include "GL/wglew.h"
    #include <Windows.h>
    #if defined(DEBUG) || defined(_DEBUG)
        #include "vld.h"
    #endif
    #pragma execution_character_set("utf-8")
    #define U8
#else
    #define U8 u8
#endif

#include "tinyxml2.h"

#ifndef  GLM_FORCE_SSE2
    #define  GLM_FORCE_SSE2
#endif

#ifndef  GLM_FORCE_INLINE
    #define  GLM_FORCE_INLINE
#endif

/*
#ifndef  GLM_MESSAGES
    #define  GLM_MESSAGES
#endif
*/

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "imgui.h"

extern "C"
{
#include "ft2build.h"
#include "freetype.h"
#include "iconv.h"
#include "FreeImage.h"
}

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

#include "boost/noncopyable.hpp"
#include "boost/format.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/multi_array.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/filesystem.hpp"

namespace ublas = boost::numeric::ublas;
namespace bfs = boost::filesystem;

#if defined(WIN32) || defined(_WIN32)
    #undef APIENTRY
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include "GLFW/glfw3native.h"   // for glfwGetWin32Window
#endif

#endif // kgl_lib_pch_h__