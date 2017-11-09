// 继承自xgf::Error的异常类,由各调用模块抛出
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

        /// <summary>
        /// 返回OpenGL的错误代码描述
        /// </summary>
        /// <param name="error_desc_array">返回OpenGL错误的描述的字符串的数组</param>
        /// <param name="error_code_array">返回OpenGL错误代码的数组</param>
        /// <returns>有错误的话返回true，没返回false</returns>
        static bool GetGLErrorDesc(std::vector<std::string>& error_desc_array, std::vector<GLenum>& error_code_array);

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
