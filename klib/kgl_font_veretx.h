// 字体顶点
/*!
 * \file kgl_font_veretx.h
 * \date 2017/12/31 13:42
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
#ifndef kgl_font_veretx_h__
#define kgl_font_veretx_h__

namespace kgl
{
    struct FontVertex   // 顶点结构描述
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="FontVertex"/> struct.
        /// </summary>
        FontVertex();
        
        /// <summary>
        /// Initializes a new instance of the <see cref="FontVertex"/> struct.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <param name="r">The r.</param>
        /// <param name="g">The g.</param>
        /// <param name="b">The b.</param>
        /// <param name="a">The a.</param>
        /// <param name="u">The u.</param>
        /// <param name="v">The v.</param>
        FontVertex(float x, float y, float r, float g, float b, float a, float u, float v);

        /// <summary>
        /// 重置对象的各个成员属性
        /// </summary>
        void Reset();

        /// <summary>
        /// 字体图元的顶点坐标的x值，基于NDC坐标
        /// </summary>
        float pos_x;

        /// <summary>
        /// 字体图元的顶点坐标的y值，基于NDC坐标
        /// </summary>
        float pos_y;

        /// <summary>
        /// 字体图元的顶点颜色的red分量
        /// </summary>
        float red;

        /// <summary>
        /// 字体图元的顶点颜色的green分量
        /// </summary>
        float green;

        /// <summary>
        /// 字体图元的顶点颜色的blue分量
        /// </summary>
        float blue;

        /// <summary>
        /// 字体图元的顶点颜色的alpha分量
        /// </summary>
        float alpha;
        
        /// <summary>
        /// 字体图元的顶点纹理贴图坐标的u坐标（s坐标）
        /// </summary>
        float texture_u;
        
        /// <summary>
        ///  字体图元的顶点纹理贴图坐标的v坐标（t坐标）
        /// </summary>
        float texture_v;
    };
}

#endif // kgl_font_veretx_h__
