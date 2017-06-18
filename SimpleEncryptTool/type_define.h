/**************************************************************************
* @file TypeDefine.h
* @brief  对整形数的一些定义，MSVS2008及其以下的VS对C99标准兼容性
*		  不太好，所以需要自己定义
* @author Xiong Xinke
* @date 2012/1/1 2:04:44
* @version 
**************************************************************************/
#ifndef type_define_h__
#define type_define_h__

#if defined(_MSC_VER)								// VC编译器
	typedef signed __int8		int8_t;
	typedef signed __int16		int16_t;
	typedef signed __int32		int32_t;
	typedef signed __int64		int64_t;
	typedef unsigned __int8		uint8_t;
	typedef unsigned __int16	uint16_t;
	typedef unsigned __int32	uint32_t;
	typedef unsigned __int64	uint64_t;
#elif defined(__GNUC__)								// 其他的编译器，如GCC等，
	#include <stdint.h>
#else
	#error Does not support other compilers
#endif 

#endif // type_define_h__