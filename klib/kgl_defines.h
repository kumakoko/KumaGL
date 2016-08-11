// 一些工具宏的定义
#ifndef kgl_defines_h__
#define kgl_defines_h__

#include "kgl_error.h"

#define THROW_GL_EXCEPTION(error_desc_array,error_code_array,file_name,code_line) if (kgl::Error::GetGLErrorDesc(error_desc_array, error_code_array)){\
	throw kgl::Error(error_code_array, error_desc_array, file_name, code_line);}

#define KGL_SAFE_DELETE(obj) do{if(nullptr != obj){delete obj;obj = nullptr;}}while(0)

#define KGL_SAFE_DELETE_ARRAY(obj) do{if(nullptr != obj){delete [] obj;obj = nullptr;}}while(0)

#endif // kgl_defines_h__