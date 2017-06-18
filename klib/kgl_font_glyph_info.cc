// 字体面类
#include "kgl_lib_pch.h"
#include "kgl_font_glyph_info.h"

namespace kgl
{
    FontGlyphInfo::FontGlyphInfo(uint32_t code,
        const FontUVRect& rect, float aspect, uint32_t w, uint32_t h,
        int32_t bearing_x, int32_t bearing_y, uint32_t advance) :
        CodePoint(code), UVRect(rect), AspectRatio(aspect), UseCount(0),
        BmpWidth(w), BmpHeight(h), BearingX(bearing_x), BearingY(bearing_y), Advance(advance)
    {
    }
}