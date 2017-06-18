// 字符串编解码码转换器
#ifndef kgl_string_encoder_h__
#define kgl_string_encoder_h__

namespace kgl
{
    class StringConvertor
    {
    private:
		/// <summary>
		/// The k temporary buffer length
		/// </summary>
		static const size_t kTmpBufferLen = 16;

		/// <summary>
		/// The s_char_buffer_
		/// </summary>
        static char s_char_buffer_[kTmpBufferLen];

		/// <summary>
		/// The s_wchar_buffer_
		/// </summary>
		static char s_wchar_buffer_[kTmpBufferLen];
		
		/// <summary>
		/// ANSI字符集,将会根据当前系统的代码页去初始化对应的值
		/// </summary>
		static std::string s_ansi_char_set;
    private:
		/// <summary>
		/// Codes the convert.
		/// </summary>
		/// <param name="from_charset">The from_charset.</param>
		/// <param name="to_charset">The to_charset.</param>
		/// <param name="inbuf">The inbuf.</param>
		/// <param name="inlen">The inlen.</param>
		/// <param name="outbuf">The outbuf.</param>
		/// <param name="outlen">The outlen.</param>
		/// <returns>int.</returns>
		static int CodeConvert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t* outlen);
    public:
		/// <summary>
		/// 把UTF8编码的字符串作为源数据,返回的得到的utf16字符串首指针
		/// </summary>
		/// <param name="utf8_str">要转换的源数据字符串,utf8编码</param>
		/// <param name="utf16_str">The utf16_str.</param>
		/// <returns>int.</returns>
		static int UTF8toUTF16LE(const char* utf8_str, wchar_t** utf16_str);

		/// <summary>
		/// Uts the f8to ut F16 le.
		/// </summary>
		/// <param name="utf8_str">The utf8_str.</param>
		/// <param name="utf16_str">The utf16_str.</param>
		/// <returns>int.</returns>
		static int UTF8toUTF16LE(const char* utf8_str, std::wstring& utf16_str);

		/// <summary>
		/// Uts the f8to ut F16 le.
		/// </summary>
		/// <param name="utf8_str">The utf8_str.</param>
		/// <returns>std.wstring.</returns>
		static std::wstring UTF8toUTF16LE(const char* utf8_str);

		/// <summary>
		/// Uts the F16 l eto ut f8.
		/// </summary>
		/// <param name="utf16_str">The utf16_str.</param>
		/// <param name="utf8_str">The utf8_str.</param>
		/// <returns>int.</returns>
		static int UTF16LEtoUTF8(const wchar_t* utf16_str, char** utf8_str);

		/// <summary>
		/// Uts the F16 l eto ut f8.
		/// </summary>
		/// <param name="utf16_str">The utf16_str.</param>
		/// <param name="utf8_str">The utf8_str.</param>
		/// <returns>int.</returns>
		static int UTF16LEtoUTF8(const wchar_t* utf16_str, std::string& utf8_str);

		/// <summary>
		/// Uts the F16 l eto ut f8.
		/// </summary>
		/// <param name="utf16_str">The utf16_str.</param>
		/// <returns>std.string.</returns>
		static std::string UTF16LEtoUTF8(const wchar_t* utf16_str);

		/// <summary>
		/// Uts the f8to GBK.
		/// </summary>
		/// <param name="utf8_str">The utf8_str.</param>
		/// <param name="gbk_str">The GBK_STR.</param>
		/// <returns>int.</returns>
		static int UTF8toGBK(const char* utf8_str, char** gbk_str);

		/// <summary>
		/// Uts the f8to GBK.
		/// </summary>
		/// <param name="utf8_str">The utf8_str.</param>
		/// <param name="gbk_str">The GBK_STR.</param>
		/// <returns>int.</returns>
		static int UTF8toGBK(const char* utf8_str, std::string& gbk_str);

		/// <summary>
		/// Gbs the kto ut f8.
		/// </summary>
		/// <param name="gbk_str">The GBK_STR.</param>
		/// <param name="utf8_str">The utf8_str.</param>
		/// <returns>int.</returns>
		static int GBKtoUTF8(const char* gbk_str, char** utf8_str);

		/// <summary>
		/// Gbs the kto ut f8.
		/// </summary>
		/// <param name="gbk_str">The GBK_STR.</param>
		/// <param name="utf8_str">The utf8_str.</param>
		/// <returns>int.</returns>
		static int GBKtoUTF8(const char* gbk_str, std::string& utf8_str);

		/// <summary>
		/// Anses the ito ut F16 le.
		/// </summary>
		/// <param name="ansi_str">The ansi_str.</param>
		/// <param name="utf16_str">The utf16_str.</param>
		/// <returns>int.</returns>
		static int ANSItoUTF16LE(const char* ansi_str, std::wstring& utf16_str);

		/// <summary>
		/// Anses the ito ut F16 le.
		/// </summary>
		/// <param name="ansi_str">The ansi_str.</param>
		/// <returns>std.wstring.</returns>
		static std::wstring ANSItoUTF16LE(const char* ansi_str);

		/// <summary>
		/// Uts the F16 l eto ANSI.
		/// </summary>
		/// <param name="utf16_str">The utf16_str.</param>
		/// <param name="ansi_str">The ansi_str.</param>
		/// <returns>int.</returns>
		static int UTF16LEtoANSI(const wchar_t* utf16_str, std::string& ansi_str);

		/// <summary>
		/// Initializes the code page information.
		/// </summary>
		static void InitCodePageInfo();
    };
}
#endif // kgl_string_encoder_h__
