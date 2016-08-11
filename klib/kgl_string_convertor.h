// 字符串编解码码转换器
#ifndef kgl_string_encoder_h__
#define kgl_string_encoder_h__

namespace kgl
{
	class StringConvertor
	{
	private:
		static const size_t kTmpBufferLen = 16;
		static char s_char_buffer_[kTmpBufferLen];
		static char s_wchar_buffer_[kTmpBufferLen];
		static std::string s_ansi_char_set; // ANSI字符集,将会根据当前系统的代码页去初始化对应的值
	private:
		/************************************
		 Method:    CodeConvert
		 FullName:  kgl::StringConvertor::CodeConvert
		 Access:    private static 
		 Returns:   int
		 Qualifier:
		 Parameter: const char * from_charset
		 Parameter: const char * to_charset
		 Parameter: const char * inbuf
		 Parameter: size_t inlen
		 Parameter: char * outbuf
		 Parameter: size_t outlen
		 ************************************/
		static int CodeConvert( const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t* outlen );
	public:
		/************************************
		 Method:    UTF8toUTF16
		 FullName:  kgl::StringConvertor::UTF8toUTF16LE
		 Access:    private static 
		 Returns:   int
		 Qualifier: 把UTF8编码的字符串作为源数据,返回的得到的utf16字符串首指针
		 Parameter: const char * utf8_str  要转换的源数据字符串,utf8编码
		 Parameter: wchar_t** utf16_str
		 ************************************/
		static int UTF8toUTF16LE(const char* utf8_str,wchar_t** utf16_str);

		/************************************
		 Method:    UTF8toUTF16LE
		 FullName:  kgl::StringConvertor::UTF8toUTF16LE
		 Access:    public static 
		 Returns:   int
		 Qualifier:
		 Parameter: const char * utf8_str
		 Parameter: std::wstring & utf16_str
		 ************************************/
		static int UTF8toUTF16LE(const char* utf8_str,std::wstring& utf16_str);

		static std::wstring UTF8toUTF16LE(const char* utf8_str);

		/************************************
		 Method:    UTF16LEtoUTF8
		 FullName:  kgl::StringConvertor::UTF16LEtoUTF8
		 Access:    public static 
		 Returns:   int
		 Qualifier:
		 Parameter: const wchar_t * utf16_str
		 Parameter: char * * utf8_str
		 ************************************/
		static int UTF16LEtoUTF8(const wchar_t* utf16_str,char** utf8_str);

		/************************************
		 Method:    UTF16LEtoUTF8
		 FullName:  kgl::StringConvertor::UTF16LEtoUTF8
		 Access:    public static 
		 Returns:   int
		 Qualifier:
		 Parameter: const wchar_t * utf16_str
		 Parameter: std::string & utf8_str
		 ************************************/
		static int UTF16LEtoUTF8(const wchar_t* utf16_str,std::string& utf8_str);

		static std::string UTF16LEtoUTF8(const wchar_t* utf16_str);

		/************************************
		 Method:    UTF8toGBK
		 FullName:  kgl::StringConvertor::UTF8toGBK
		 Access:    public static 
		 Returns:   int
		 Qualifier:
		 Parameter: const char * utf8_str
		 Parameter: char * * gbk_str
		 ************************************/
		static int UTF8toGBK(const char* utf8_str,char** gbk_str);

		/************************************
		 Method:    UTF8toGBK
		 FullName:  kgl::StringConvertor::UTF8toGBK
		 Access:    public static 
		 Returns:   int
		 Qualifier:
		 Parameter: const char * utf8_str
		 Parameter: std::string & gbk_str
		 ************************************/
		static int UTF8toGBK(const char* utf8_str,std::string& gbk_str);

		/************************************
		 Method:    GBKtoUTF8
		 FullName:  kgl::StringConvertor::GBKtoUTF8
		 Access:    public static 
		 Returns:   int
		 Qualifier:
		 Parameter: const char * gbk_str
		 Parameter: char * * utf8_str
		 ************************************/
		static int GBKtoUTF8(const char* gbk_str,char** utf8_str);

		/************************************
		 Method:    GBKtoUTF8
		 FullName:  kgl::StringConvertor::GBKtoUTF8
		 Access:    public static 
		 Returns:   int
		 Qualifier:
		 Parameter: const char * gbk_str
		 Parameter: std::string & utf8_str
		 ************************************/
		static int GBKtoUTF8(const char* gbk_str,std::string& utf8_str);

		/************************************************************
		 Method:    ANSItoUTF16LE
		 FullName:  kgl::StringConvertor::ANSItoUTF16LE
		 Access:    public static 
		 Returns:   int
		 Qualifier:
		 Parameter: const char * ansi_str
		 Parameter: std::wstring & utf16_str
		************************************************************/
		static int ANSItoUTF16LE(const char* ansi_str,std::wstring& utf16_str);

		static std::wstring ANSItoUTF16LE(const char* ansi_str);

		/************************************************************
		 Method:    UTF16LEtoANSI
		 FullName:  kgl::StringConvertor::UTF16LEtoANSI
		 Access:    public static 
		 Returns:   int
		 Qualifier:
		 Parameter: const wchar_t * utf16_str
		 Parameter: std::string & ansi_str
		************************************************************/
		static int UTF16LEtoANSI(const wchar_t* utf16_str,std::string& ansi_str);

		static void InitCodePageInfo();
	};
}
#endif // kgl_string_encoder_h__
