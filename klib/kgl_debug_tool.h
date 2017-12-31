// 调试工具
/*!
 * \file kgl_debug_tool.h
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
#ifndef kgl_debug_tool_h__
#define kgl_debug_tool_h__

namespace kgl
{
    class DebugTool
    {
    public:
        /// <summary>
        /// 在IDE控制台中打印调试信息
        /// </summary>
        /// <param name="message">待打印出来的调试信息.</param>
        static void OutputDebugMessage(const char* message);

        static void PrintMatrix(const char* matrix_name, const glm::mat4& matrix);

        static void PrintMatrix(const char* matrix_name, const aiMatrix4x4& matrix);

        static void PrintGLMMatrix(const char* matrix_name, const glm::mat4& matrix, bool column_major = true);

        static void PrintVector3(const char* vector_name, const glm::vec3& vector);
    };
}
#endif // kgl_debug_tool_h__