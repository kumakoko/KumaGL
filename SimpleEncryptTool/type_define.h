/**************************************************************************
* @file TypeDefine.h
* @brief  ����������һЩ���壬MSVS2008�������µ�VS��C99��׼������
*		  ��̫�ã�������Ҫ�Լ�����
* @author Xiong Xinke
* @date 2012/1/1 2:04:44
* @version 
**************************************************************************/
#ifndef type_define_h__
#define type_define_h__

#if defined(_MSC_VER)								// VC������
	typedef signed __int8		int8_t;
	typedef signed __int16		int16_t;
	typedef signed __int32		int32_t;
	typedef signed __int64		int64_t;
	typedef unsigned __int8		uint8_t;
	typedef unsigned __int16	uint16_t;
	typedef unsigned __int32	uint32_t;
	typedef unsigned __int64	uint64_t;
#elif defined(__GNUC__)								// �����ı���������GCC�ȣ�
	#include <stdint.h>
#else
	#error Does not support other compilers
#endif 

#endif // type_define_h__