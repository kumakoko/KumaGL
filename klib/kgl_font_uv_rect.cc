// 文字包围盒类
#include "kgl_lib_pch.h"
#include "kgl_font_uv_rect.h"

namespace kgl
{
    FontUVRect::FontUVRect(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b)
    {
    }

    FontUVRect::FontUVRect() : left(0.f), top(0.f), right(0.f), bottom(0.f)
    {
    }

    const FontUVRect& FontUVRect::ZeroRect()
    {
        static FontUVRect zero;
        return zero;
    }
}