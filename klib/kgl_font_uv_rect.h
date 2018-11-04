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
/*!
* \file kgl_font_uv_rect.h
* \date 2017/12/31 13:42
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: l文字包围盒类
*
* \note
*/
#ifndef kgl_font_uv_rect_h__
#define kgl_font_uv_rect_h__

namespace kgl
{
    struct FontUVRect
    {
        /// <summary>
        /// The top
        /// </summary>
        float top;

        /// <summary>
        /// The bottom
        /// </summary>
        float bottom;

        /// <summary>
        /// The left
        /// </summary>
        float left;

        /// <summary>
        /// The right
        /// </summary>
        float right;

        /// <summary>
        /// Initializes a new instance of the <see cref="FontUVRect"/> struct.
        /// </summary>
        /// <param name="l">The l.</param>
        /// <param name="t">The t.</param>
        /// <param name="r">The r.</param>
        /// <param name="b">The b.</param>
        FontUVRect(float l, float t, float r, float b);

        /// <summary>
        /// Initializes a new instance of the <see cref="FontUVRect"/> struct.
        /// </summary>
        FontUVRect();

        /// <summary>
        /// Zeroes the rect.
        /// </summary>
        /// <returns>const FontUVRect &.</returns>
        static const FontUVRect& ZeroRect();
    };
}
#endif // kgl_font_uv_rect_h__