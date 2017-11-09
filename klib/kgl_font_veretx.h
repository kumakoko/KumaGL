// 字体顶点
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
