// 继承自xgf::Error的异常类,由各调用模块抛出
#ifndef kgl_error_h__
#define kgl_error_h__

namespace kgl
{
    class Error : public std::exception
    {
    public:
        Error(const std::wstring& message);

        /************************************
         Method:    Error
         FullName:  kgl::Error::Error
         Access:    public 
         Returns:   
         Qualifier:
         Parameter: const std::wstring & message 错误消息字符串
         Parameter: const wchar_t * file 一般传__FILE__值进去
         Parameter: uint32_t line 一般传__LINE__值进去
         ************************************/
        Error(const std::wstring& message, const char* file, uint32_t line);

        /************************************
         Method:    Error
         FullName:  kgl::Error::Error
         Access:    public 
         Returns:   
         Qualifier:
         Parameter: const std::wstring & message 错误消息字符串
         Parameter: const wchar_t * file 一般传__FILE__值进去
         Parameter: uint32_t line 一般传__LINE__值进去
         Parameter: HRESULT hr DIRECTX返回的错误码
         ************************************/
        Error(const std::wstring& message, const char* file, uint32_t line, HRESULT hr);


        //************************************
        // Method:    Error
        // FullName:  kgl::Error::Error
        // Access:    public 
        // Returns:   
        // Qualifier:
        // Parameter: const std::vector<GLenum>& error_code_array
        // Parameter: const std::vector<std::string> & error_desc_array
        // Parameter: const char * file
        // Parameter: uint32_t line
        //************************************
        Error(const std::vector<GLenum>& error_code_array, const std::vector<std::string>& error_desc_array, const char* file, uint32_t line);
        
        /************************************
         Method:    Notify
         FullName:  kgl::Error::Notify
         Access:    virtual public 
         Returns:   void
         Qualifier: const
         ************************************/
        virtual void Notify() const;
        
        
        /************************************
         Method:    Prompt
         FullName:  kgl::Error::Prompt
         Access:    virtual public 
         Returns:   void
         Qualifier: 通知用户出错了,提供一个二选一对话框给用户选择
         ************************************/
        virtual void Prompt() const;
        
        //************************************
        // Method:    GetGLErrorDesc
        // FullName:  kgl::Error::GetGLErrorDesc
        // Access:    public static 
        // Returns:   bool 有错误的话返回true，没返回false
        // Qualifier:
        // Parameter: std::vector<std::string> & error_desc_array 返回OpenGL错误的描述的字符串的数组
        // Parameter: std::vector<GLenum> & error_code_array 返回OpenGL错误代码的数组
        //************************************
        static bool GetGLErrorDesc(std::vector<std::string>& error_desc_array, std::vector<GLenum>& error_code_array);

    protected:
        /************************************
         Method:    AssembleOutput
         FullName:  kgl::Error::AssembleOutput
         Access:    virtual private 
         Returns:   std::wstring
         Qualifier: 组装错误消息,返回错误消息字符串
         ************************************/
        virtual std::wstring AssembleOutput() const;

        enum EXCEPTION_TYPE
        {
            EXCEPTION_MESSAGE,  ///< 返回错误消息
            EXCEPTION_REGULAR,  ///< 返回错误消息,出错的代码文件和行
            EXCEPTION_OPENGL    ///< 返回错误消息,出错的代码文件和行以及渲染器给的错误代码
        };

        EXCEPTION_TYPE type_;   ///< 错误类型
        std::wstring title_;    ///< 对话框窗口标题
        std::wstring message_;  ///< 错误消息字符串
        std::string file_;      ///< 发生错误的代码文件名
        uint32_t line_;         ///< 发生错误的代码行
    };
}

#endif // kgl_error_h__
