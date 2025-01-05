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
* \file kgl_error.h
* \date 2017/12/31 13:38
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: 继承自xgf::Error的异常类,由各调用模块抛出
*
* \note
*/
#ifndef kgl_error_h__
#define kgl_error_h__

namespace kgl
{
    class Error : public std::exception
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Error"/> class.
        /// </summary>
        /// <param name="message">The message.</param>
        Error(const std::wstring& message);

        /// <summary>
        /// Initializes a new instance of the <see cref="Error"/> class.
        /// </summary>
        /// <param name="message">错误消息字符串</param>
        /// <param name="file">一般传__FILE__值进去</param>
        /// <param name="line">一般传__LINE__值进去</param>
        Error(const std::wstring& message, const char* file, uint32_t line);

        /// <summary>
        /// Initializes a new instance of the <see cref="Error"/> class.
        /// </summary>
        /// <param name="error_code_array">The error_code_array.</param>
        /// <param name="error_desc_array">The error_desc_array.</param>
        /// <param name="file">The file.</param>
        /// <param name="line">The line.</param>
        Error(const std::vector<GLenum>& error_code_array, const std::vector<std::string>& error_desc_array, const char* file, uint32_t line);

        /// <summary>
        /// Notifies this instance.
        /// </summary>
        virtual void Notify() const;

        /// <summary>
        ///  通知用户出错了,提供一个二选一对话框给用户选择
        /// </summary>
        virtual void Prompt() const;

        /**************************************************************************************
        返回OpenGL的错误代码描述
        @name: kgl::Error::GetGLErrorDesc
        @return: bool 有错误的话返回true，没返回false
        @param: std::vector<std::string> & error_desc_array 返回OpenGL错误的描述的字符串的数组
        @param: std::vector<GLenum> & error_code_array 返回OpenGL错误代码的数组
        *************************************************************************************/
        static bool GetGLErrorDesc(std::vector<std::string>& error_desc_array, std::vector<GLenum>& error_code_array);

        /**************************************************************************************

        @name: kgl::Error::CheckGLReturnValue
        @return: void
        @param: const char * file_path
        @param: uint32_t line
        *************************************************************************************/
        static void CheckGLReturnValue(const char* file_path, uint32_t line);

        /**************************************************************************************

        @name: kgl::Error::CheckGLReturnValueSimple
        @return: void
        @param: const char * file_path
        @param: uint32_t line
        *************************************************************************************/
        static void CheckGLReturnValueSimple(const char* file_path, uint32_t line);

    protected:
        /// <summary>
        /// 组装错误消息,返回错误消息字符串
        /// </summary>
        /// <returns>错误消息字符串</returns>
        virtual std::wstring AssembleOutput() const;

        enum EXCEPTION_TYPE
        {
            EXCEPTION_MESSAGE,  ///< 返回错误消息
            EXCEPTION_REGULAR,  ///< 返回错误消息,出错的代码文件和行
            EXCEPTION_OPENGL    ///< 返回错误消息,出错的代码文件和行以及渲染器给的错误代码
        };

        /// <summary>
        ///错误类型
        /// </summary>
        EXCEPTION_TYPE type_;

        /// <summary>
        /// 对话框窗口标题
        /// </summary>
        std::wstring title_;

        /// <summary>
        /// 错误消息字符串
        /// </summary>
        std::wstring message_;

        /// <summary>
        /// 发生错误的代码文件名
        /// </summary>
        std::string file_;

        /// <summary>
        /// 发生错误的代码行
        /// </summary>
        uint32_t line_;
    };
}

#endif // kgl_error_h__
