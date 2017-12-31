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
// 字体面类
#include "kgl_lib_pch.h"
#include "kgl_font_glyph_info.h"

namespace kgl
{
    FontGlyphInfo::FontGlyphInfo(uint32_t code,
        const FontUVRect& rect, float aspect, uint32_t w, uint32_t h,
        int32_t _bearing_x, int32_t _bearing_y, uint32_t _advance) :
        code_point(code), texture_uv_rect(rect), aspect_ratio(aspect), used_count(0),
        bmp_width(w), bmp_height(h), bearing_x(_bearing_x), bearing_y(_bearing_y), advance(_advance)
    {
    }
}