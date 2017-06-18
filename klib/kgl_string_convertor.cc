// 字符串编解码码转换器
#include "kgl_lib_pch.h"
#include "kgl_string_convertor.h"

namespace kgl
{
    char StringConvertor::s_char_buffer_[kTmpBufferLen];
    std::string StringConvertor::s_ansi_char_set;

    void StringConvertor::InitCodePageInfo()
    {
        /*
        for(int i=0; i<=65001; i++)
        {
            CPINFOEX cpinfo;

            if (IsValidCodePage(i))
            {
                if (GetCPInfoEx(i, 0, &cpinfo))
                {
                    std::wstringstream wss;
                    wss<<L"cpInfo.CodePage = "<<cpinfo.CodePage<<L" , cpInfo.CodePageName ="<<cpinfo.CodePageName<<std::endl;
                    OutputDebugString(wss.str().c_str());
                }
            }
        }
        */

        switch(::GetACP()) // 获取当前的代码页
        {
        case 936:s_ansi_char_set = "gbk";break; //　简体中文
        case 932:s_ansi_char_set = "shift-jis";break; //　日语
        case 949:s_ansi_char_set = "";break; // 朝鲜语
        case 950:s_ansi_char_set = "big5";break; // 繁体中文
        default:s_ansi_char_set = "unknown";break;
        }
    }

    int StringConvertor::CodeConvert( const char* from_charset, const char* to_charset, const char* in_buf, size_t in_len, char* out_buf, size_t* out_len )
    {
        //const char *temp = in_buf;
        const char** pin = &in_buf;
        char** pout = &out_buf;
        memset(out_buf,0,*out_len); // 清空输出缓冲区的内容

        iconv_t cd = iconv_open(to_charset,from_charset);
        
        if(cd == 0)  // 类型相同,转换失败
            return -1;
        
        if(iconv(cd,pin,&in_len,pout,out_len)==-1) 
            return -1;
        
        iconv_close(cd);
        return 0;
    }

    int StringConvertor::UTF16LEtoUTF8(const wchar_t* utf16_str,char** utf8_str)
    {
        // 首先计算出utf8的字符数,然后得出字节数
        size_t utf16_char_count = wcslen(utf16_str);
        size_t utf16_str_len = utf16_char_count * 2;
        size_t utf8_str_len = utf16_char_count * 3; // 假定一个utf16字符转成一个3字节的uft8字符

        char* target_buf = new char[utf16_str_len];
        memset(target_buf,0,sizeof(char)*utf16_str_len);
        CodeConvert("UTF-16LE","UTF-8",reinterpret_cast<const char*>(utf16_str),utf16_str_len,target_buf,&utf8_str_len);
        *utf8_str = target_buf;
        return 0;
    }

    std::string StringConvertor::UTF16LEtoUTF8(const wchar_t* utf16_str)
    {
        std::string u8_str;
        StringConvertor::UTF16LEtoUTF8(utf16_str,u8_str);
        return u8_str;
    }

    int StringConvertor::UTF16LEtoUTF8(const wchar_t* utf16_str,std::string& utf8_str)
    {
        // 首先计算出utf8的字符数,然后得出字节数
        size_t utf16_char_count = wcslen(utf16_str);
        size_t utf16_str_len = utf16_char_count * 2;
        size_t utf8_str_len = utf16_char_count * 3; // 假定一个utf16字符转成一个3字节的uft8字符

        char* target_buf = nullptr;
        bool use_cache = false;

        if (utf16_str_len <= kTmpBufferLen) // 用缓冲区
        {
            target_buf = s_char_buffer_;
            use_cache = true;
            memset(target_buf,0,sizeof(char)*kTmpBufferLen);
        }
        else
        {
            target_buf = new char[utf8_str_len];
            memset(target_buf,0,sizeof(char)*utf8_str_len);
        }

        CodeConvert("UTF-16LE","UTF-8",reinterpret_cast<const char*>(utf16_str),utf16_str_len,target_buf,&utf8_str_len);
        utf8_str.clear();
        utf8_str = target_buf;

        if (!use_cache)
        {
            delete [] target_buf;
        }

        return 0;
    }

    int StringConvertor::UTF8toUTF16LE(const char* utf8_str,wchar_t** utf16_str)
    {
        // 首先,算出uft8的长度,然后知道最多需要多少个字符放置utf16字符串
        size_t utf8_str_len = strlen(utf8_str);
        // 就算一个字节就转成一个宽字符,所需要的utf16字符串字节空间就是2 * utf8_str_len
        size_t utf16_str_len = utf8_str_len * 2 + 2;
        wchar_t* target_buf = new wchar_t[utf16_str_len/2];
        CodeConvert("UTF-8","UTF-16LE",utf8_str,utf8_str_len,
            reinterpret_cast<char*>(target_buf),&utf16_str_len);
        *utf16_str = reinterpret_cast<wchar_t*>(target_buf);
        return 0;
    }

    std::wstring StringConvertor::UTF8toUTF16LE(const char* utf8_str)
    {
        std::wstring u16le_str;
        StringConvertor::UTF8toUTF16LE(utf8_str,u16le_str);
        return u16le_str;
    }

    int StringConvertor::UTF8toUTF16LE(const char* utf8_str,std::wstring& utf16_str)
    {
        // 首先,算出uft8的长度,然后知道最多需要多少个字符放置utf16字符串
        size_t utf8_str_len = strlen(utf8_str);
        // 就算一个字节就转成一个宽字符,所需要的utf16字符串字节空间就是2 * utf8_str_len
        size_t utf16_str_len = utf8_str_len * 2 + 2;
        wchar_t* target_buf = nullptr;
        bool use_cache = false;

        if (utf16_str_len < kTmpBufferLen) // 用缓冲区
        {
            memset(s_char_buffer_,0,sizeof(char)*kTmpBufferLen);
            target_buf = reinterpret_cast<wchar_t*>(s_char_buffer_);
            use_cache = true;
        }
        else
        {
            target_buf = new wchar_t[utf16_str_len/2];
            memset(target_buf,0,sizeof(char)*utf16_str_len/2);
        }

        CodeConvert("UTF-8","UTF-16LE",utf8_str,utf8_str_len,
            reinterpret_cast<char*>(target_buf),&utf16_str_len);
        utf16_str.clear();
        utf16_str = target_buf;

        if (!use_cache)
        {
            delete [] target_buf;
        }

        return 0;
    }

    int StringConvertor::UTF8toGBK(const char* utf8_str,char** gbk_str)
    {
        // 首先,算出uft8的长度,然后知道最多需要多少个字符放置utf16字符串
        size_t utf8_str_len = strlen(utf8_str);
        // 就算一个字节就转成一个宽字符,所需要的utf16字符串字节空间就是2 * utf8_str_len
        size_t gbk_str_len = utf8_str_len * 2;
        char* target_buf = new char[gbk_str_len];
        memset(target_buf,0,sizeof(char)*gbk_str_len);
        CodeConvert("UTF-8","GBK",utf8_str,utf8_str_len,target_buf,&gbk_str_len);
        *gbk_str = target_buf;
        return 0;
    }

    int StringConvertor::UTF8toGBK(const char* utf8_str,std::string& gbk_str)
    {
        // 首先,算出uft8的长度,然后知道最多需要多少个字符放置utf16字符串
        size_t utf8_str_len = strlen(utf8_str);
        // 就算一个字节就转成一个宽字符,所需要的utf16字符串字节空间就是2 * utf8_str_len
        size_t gbk_str_len = utf8_str_len * 2;
        char* target_buf = nullptr;
        bool use_cache = false;

        if (gbk_str_len <= kTmpBufferLen) // 用缓冲区
        {
            target_buf = s_char_buffer_;
            memset(target_buf,0,sizeof(char)*kTmpBufferLen);
            use_cache = true;
        }
        else
        {
            target_buf = new char[gbk_str_len];
            memset(target_buf,0,sizeof(char)*gbk_str_len);
        }

        CodeConvert("UTF-8","GBK",utf8_str,utf8_str_len,target_buf,&gbk_str_len);
        gbk_str.clear();
        gbk_str = target_buf;

        if (!use_cache)
        {
            delete [] target_buf;
        }

        return 0;
    }

    int StringConvertor::GBKtoUTF8(const char* gbk_str,char** utf8_str)
    {
        // 首先计算出utf8的字符数,然后得出字节数
        size_t gbk_str_len = strlen(gbk_str);
        size_t utf8_str_len = gbk_str_len * 3; // 假定每一个字节对应一个gbk字符,那么最多转成一个3*gbk_str_len字节就够放utf8了

        char* target_buf = new char[utf8_str_len];
        memset(target_buf,0,sizeof(char)*utf8_str_len);
        CodeConvert("GBK","UTF-8",gbk_str,gbk_str_len,target_buf,&utf8_str_len);
        *utf8_str = target_buf;
        return 0;
    }

    int StringConvertor::GBKtoUTF8(const char* gbk_str,std::string& utf8_str)
    {
        // 首先,算出uft8的长度,然后知道最多需要多少个字符放置utf16字符串
        size_t gbk_str_len = strlen(gbk_str);
        size_t utf8_str_len = gbk_str_len * 3;

        char* target_buf = nullptr;
        bool use_cache = false;

        if (gbk_str_len <= kTmpBufferLen) // 用缓冲区
        {
            target_buf = s_char_buffer_;
            memset(target_buf,0,sizeof(char)*kTmpBufferLen);
            use_cache = true;
        }
        else
        {
            target_buf = new char[gbk_str_len];
            memset(target_buf,0,sizeof(char)*gbk_str_len);
        }

        CodeConvert("GBK","UTF-8",gbk_str,gbk_str_len,target_buf,&gbk_str_len);
        utf8_str.clear();
        utf8_str = target_buf;

        if (!use_cache)
        {
            delete [] target_buf;
        }

        return 0;
    }

    std::wstring StringConvertor::ANSItoUTF16LE(const char* ansi_str)
    {
        std::wstring utf16_str;
        ANSItoUTF16LE(ansi_str, utf16_str);
        return utf16_str;
    }
 
    int StringConvertor::ANSItoUTF16LE(const char* ansi_str,std::wstring& utf16_str)
    {
        size_t ansi_str_len = strlen(ansi_str);
        size_t utf16_str_len = ansi_str_len * 2 + 2;

        wchar_t* target_buf = nullptr;
        bool use_cache = false;

        if (utf16_str_len < kTmpBufferLen) // 用缓冲区
        {
            memset(s_char_buffer_,0,sizeof(char)*kTmpBufferLen);
            target_buf = reinterpret_cast<wchar_t*>(s_char_buffer_);
            use_cache = true;
        }
        else
        {
            target_buf = new wchar_t[utf16_str_len/2];
            memset(target_buf,0,sizeof(char)*utf16_str_len/2);
        }

        CodeConvert(s_ansi_char_set.c_str(),"UTF-16LE",ansi_str,ansi_str_len,(char*)target_buf,&utf16_str_len);
        utf16_str.clear();
        utf16_str = target_buf;

        if (!use_cache)
        {
            delete [] target_buf;
        }

        return 0;
    }
    
    int StringConvertor::UTF16LEtoANSI(const wchar_t* utf16_str,std::string& ansi_str)
    {
        size_t utf16_char_count = wcslen(utf16_str);
        size_t utf16_str_len = utf16_char_count * 2;
        
        char* target_buf = nullptr;
        bool use_cache = false;

        if (utf16_str_len <= kTmpBufferLen) // ANSI最多也只和wstring等长。所以直接用utf16_str_len
        {
            target_buf = s_char_buffer_;
            memset(target_buf,0,sizeof(char)*kTmpBufferLen);
            use_cache = true;
        }
        else
        {
            target_buf = new char[utf16_str_len];
            memset(target_buf,0,sizeof(char)*utf16_str_len);
        }

        CodeConvert("UTF-16LE",s_ansi_char_set.c_str(),
            reinterpret_cast<const char*>(utf16_str),utf16_str_len,target_buf,&utf16_str_len);
        ansi_str.clear();
        ansi_str = target_buf;

        if (!use_cache)
        {
            delete [] target_buf;
        }

        return 0;
    }


}