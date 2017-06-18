// 字体顶点
#include "kgl_lib_pch.h"
#include "kgl_font_veretx.h"

namespace kgl
{
    FontVertex::FontVertex() :PosX(0.0f), PosY(0.0f), R(0.0f), G(0.0f), B(0.0f), A(0.0f), U(0.0f), V(0.0f)
    {
        
    }

    FontVertex::FontVertex(float x, float y, float r, float g, float b, float a, float u, float v) :
        PosX(x), PosY(y), R(r),G(g),B(b),A(a), U(u), V(v){}

    void FontVertex::Reset()
    {
        PosX = 0.0f;
        PosY = 0.0f;
        R = 0.0f;
        G = 0.0f;
        B = 0.0f;
        A = 0.0f;
        U = 0.0f;
        V = 0.0f;
    }
}