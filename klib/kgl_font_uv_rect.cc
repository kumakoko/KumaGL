// 文字包围盒类
#include "kgl_lib_pch.h"
#include "kgl_font_uv_rect.h"

namespace kgl
{
    FontUVRect::FontUVRect(float l, float t, float r, float b) : Left(l), Top(t), Right(r), Bottom(b)
    {
    }

    FontUVRect::FontUVRect() : Left(0.f), Top(0.f), Right(0.f), Bottom(0.f)
    {
    }

    const FontUVRect& FontUVRect::ZeroRect()
    {
        static FontUVRect zero;
        return zero;
    }
}