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