// 一些工具宏的定义
/*!
 * \file kgl_defines.h
 * \date 2017/12/31 13:36
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
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
#ifndef kgl_defines_h__
#define kgl_defines_h__

#include "kgl_error.h"

#define THROW_GL_EXCEPTION(error_desc_array,error_code_array,file_name,code_line) if (kgl::Error::GetGLErrorDesc(error_desc_array, error_code_array)){\
    throw kgl::Error(error_code_array, error_desc_array, file_name, code_line);}

#define KGL_SAFE_DELETE(obj) do{if(nullptr != obj){delete obj;obj = nullptr;}}while(0)

#define KGL_SAFE_DELETE_ARRAY(obj) do{if(nullptr != obj){delete [] obj;obj = nullptr;}}while(0)

typedef boost::multi_array<float, 2> Float2DArray;
typedef boost::multi_array<glm::vec4, 2> Color2DArray;
typedef boost::multi_array<glm::vec4, 2> Vec4Array2D;
typedef boost::multi_array<glm::vec3, 2> Vec3Array2D;

#define COLOR_TEXTURE_UNIT					GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX			0
#define SHADOW_TEXTURE_UNIT					GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX			1
#define NORMAL_TEXTURE_UNIT					GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX			2
#define RANDOM_TEXTURE_UNIT					GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX			3
#define DISPLACEMENT_TEXTURE_UNIT			GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX		4
#define MOTION_TEXTURE_UNIT					GL_TEXTURE5
#define MOTION_TEXTURE_UNIT_INDEX			5
#define CASCACDE_SHADOW_TEXTURE_UNIT0       SHADOW_TEXTURE_UNIT
#define CASCACDE_SHADOW_TEXTURE_UNIT0_INDEX SHADOW_TEXTURE_UNIT_INDEX
#define CASCACDE_SHADOW_TEXTURE_UNIT1       GL_TEXTURE6
#define CASCACDE_SHADOW_TEXTURE_UNIT1_INDEX 6
#define CASCACDE_SHADOW_TEXTURE_UNIT2       GL_TEXTURE7
#define CASCACDE_SHADOW_TEXTURE_UNIT2_INDEX 7

#endif // kgl_defines_h__