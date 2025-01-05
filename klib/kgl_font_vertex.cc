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