// 字体面类
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