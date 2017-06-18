// 调试工具
#include "kgl_lib_pch.h"
#include "kgl_debug_tool.h"
#include "kgl_string_convertor.h"

namespace kgl
{
	void DebugTool::OutputDebugMessage(const char* message)
	{
#if defined(WIN32) || defined(_WIN32)
		::OutputDebugStringW(StringConvertor::ANSItoUTF16LE(message).c_str());
#else
#endif
	}

	void DebugTool::PrintMatrix(const char* matrix_name,const glm::mat4& matrix)
	{
#if defined(WIN32) || defined(_WIN32)
		const wchar_t* msg_format = L"===> GLM Matrix %s :\n"
									L"    %f,%f,%f,%f\n"
									L"    %f,%f,%f,%f\n"
									L"    %f,%f,%f,%f\n"
									L"    %f,%f,%f,%f\n";
		boost::wformat f(msg_format);
		f % StringConvertor::ANSItoUTF16LE(matrix_name)
		  % matrix[0][0] % matrix[0][1] % matrix[0][2] % matrix[0][3]
		  % matrix[1][0] % matrix[1][1] % matrix[1][2] % matrix[1][3]
		  % matrix[2][0] % matrix[2][1] % matrix[2][2] % matrix[2][3]
		  % matrix[3][0] % matrix[3][1] % matrix[3][2] % matrix[3][3];
		::OutputDebugStringW(f.str().c_str());
#else
#endif
	}

	void DebugTool::PrintMatrix(const char* matrix_name, const aiMatrix4x4& matrix)
	{
#if defined(WIN32) || defined(_WIN32)
		const wchar_t* msg_format = L"===> AssImp Matrix %s :\n"
									L"    %f,%f,%f,%f\n"
									L"    %f,%f,%f,%f\n"
									L"    %f,%f,%f,%f\n"
									L"    %f,%f,%f,%f\n";
		boost::wformat f(msg_format);
		f % StringConvertor::ANSItoUTF16LE(matrix_name)
			% matrix[0][0] % matrix[0][1] % matrix[0][2] % matrix[0][3]
			% matrix[1][0] % matrix[1][1] % matrix[1][2] % matrix[1][3]
			% matrix[2][0] % matrix[2][1] % matrix[2][2] % matrix[2][3]
			% matrix[3][0] % matrix[3][1] % matrix[3][2] % matrix[3][3];
		::OutputDebugStringW(f.str().c_str());
#else
#endif
	}

	void DebugTool::PrintGLMMatrix(const char* matrix_name, const glm::mat4& matrix, bool column_major)
	{
#if defined(WIN32) || defined(_WIN32)
		const wchar_t* msg_format = L"===> AssImp Matrix %s :\n"
									L"    %f,%f,%f,%f\n"
									L"    %f,%f,%f,%f\n"
									L"    %f,%f,%f,%f\n"
									L"    %f,%f,%f,%f\n";
		boost::wformat f(msg_format);

		if (column_major)
		{
			f % StringConvertor::ANSItoUTF16LE(matrix_name)
			  % matrix[0][0] % matrix[1][0] % matrix[2][0] % matrix[3][0]
			  % matrix[0][1] % matrix[1][1] % matrix[2][1] % matrix[3][1]
			  % matrix[0][2] % matrix[1][2] % matrix[2][2] % matrix[3][2]
			  % matrix[0][3] % matrix[1][3] % matrix[2][3] % matrix[3][3];
		}
		else
		{
			f % StringConvertor::ANSItoUTF16LE(matrix_name)
			  % matrix[0][0] % matrix[0][1] % matrix[0][2] % matrix[0][3]
			  % matrix[1][0] % matrix[1][1] % matrix[1][2] % matrix[1][3]
			  % matrix[2][0] % matrix[2][1] % matrix[2][2] % matrix[2][3]
			  % matrix[3][0] % matrix[3][1] % matrix[3][2] % matrix[3][3];
		}

		::OutputDebugStringW(f.str().c_str());
#else
#endif
	}
}