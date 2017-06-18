// 文字包围盒类
#ifndef kgl_font_uv_rect_h__
#define kgl_font_uv_rect_h__

namespace kgl
{
    struct FontUVRect
    {
        float Top;
        float Bottom;
        float Left;
        float Right;
        FontUVRect(float l, float t, float r, float b);
        FontUVRect();
        static const FontUVRect& ZeroRect();
    };
}
#endif // kgl_font_uv_rect_h__