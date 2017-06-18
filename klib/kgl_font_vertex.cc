// 字体顶点
#include "kgl_lib_pch.h"
#include "kgl_font_veretx.h"

namespace kgl
{
    FontVertex::FontVertex() :pos_x(0.0f), pos_y(0.0f), red(0.0f), green(0.0f), blue(0.0f), alpha(0.0f), texture_u(0.0f), texture_v(0.0f)
    {
        
    }

    FontVertex::FontVertex(float x, float y, float r, float g, float b, float a, float u, float v) :
        pos_x(x), pos_y(y), red(r),green(g),blue(b),alpha(a), texture_u(u), texture_v(v){}

    void FontVertex::Reset()
    {
        pos_x = 0.0f;
        pos_y = 0.0f;
        red = 0.0f;
        green = 0.0f;
        blue = 0.0f;
        alpha = 0.0f;
        texture_u = 0.0f;
        texture_v = 0.0f;
    }
}