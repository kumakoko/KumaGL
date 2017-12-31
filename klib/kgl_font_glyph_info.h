// 字体面类
/*!
 * \file kgl_font_glyph_info.h
 * \date 2017/12/31 13:38
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
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
#ifndef kgl_font_glyph_info_h__
#define kgl_font_glyph_info_h__

#include "kgl_font_uv_rect.h"

namespace kgl
{
    struct FontGlyphInfo
    {
        /// <summary>
        /// 字符的unicode码
        /// </summary>
        uint32_t    code_point;

        /// <summary>
        /// 纹理区域
        /// </summary>
        FontUVRect  texture_uv_rect;

        /// <summary>
        /// The aspect_ratio
        /// </summary>
        float aspect_ratio;

        /// <summary>
        /// 字符的使用次数
        /// </summary>
        uint16_t used_count;

        /// <summary>
        /// 字符图的宽度，基于像素值
        /// </summary>
        uint32_t bmp_width;

        /// <summary>
        /// 字符图的宽度，基于像素值
        /// </summary>
        uint32_t bmp_height;

        /// <summary>
        /// 字符glyph左上角和左baseline的偏移值，单位是像素
        /// </summary>
        int32_t bearing_x;

        /// <summary>
        /// 字符glyph左上角和下baseline的偏移值，单位是像素
        /// </summary>
        int32_t bearing_y;
        
        /// <summary>
        /// 字符的步长值，单位是像素
        /// </summary>
        uint32_t advance;

        /// <summary>
        /// 创建一个 <see cref="FontGlyphInfo"/> 结构体的实例对象.
        /// </summary>
        FontGlyphInfo(){}

        /// <summary>
        /// 根據給定的各個參數，创建一个 <see cref="FontGlyphInfo"/> 结构体的实例对象.
        /// </summary>
        /// <param name="code">字符的Unicode码</param>
        /// <param name="rect">字符的源纹理矩形.</param>
        /// <param name="aspect">比率</param>
        /// <param name="w">宽</param>
        /// <param name="h">高</param>
        /// <param name="bearing_x">字符glyph左上角和左baseline的偏移值，单位是像素</param>
        /// <param name="bearing_y">字符glyph左上角和下baseline的偏移值，单位是像素</param>
        /// <param name="advance">字符的步长值，单位是像素</param>
        FontGlyphInfo(uint32_t code, const FontUVRect& rect, float aspect, uint32_t w, uint32_t h, int32_t _bearing_x, int32_t _bearing_y, uint32_t _advance);
    };
}
#endif // kgl_font_glyph_info_h__