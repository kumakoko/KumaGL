// 字体面类
#ifndef kgl_font_glyph_info_h__
#define kgl_font_glyph_info_h__

#include "kgl_font_uv_rect.h"

namespace kgl
{
	struct FontGlyphInfo
	{
		uint32_t	CodePoint;	 //字符的unicode码
		FontUVRect	UVRect;		 //纹理区域
		float		AspectRatio;
		uint16_t	UseCount;	 //字符的使用次数
		uint32_t	BmpWidth;    // 字符图的宽度，基于像素值
		uint32_t	BmpHeight;   // 字符图的宽度，基于像素值
		int32_t		BearingX;    // 字符glyph左上角和左baseline的偏移值，单位是像素
		int32_t     BearingY;    // 字符glyph左上角和下baseline的偏移值，单位是像素
		uint32_t    Advance;     // 字符的步长值，单位是像素
		FontGlyphInfo(){}
		FontGlyphInfo(uint32_t code, const FontUVRect& rect, float aspect, uint32_t w, uint32_t h,int32_t bearing_x, int32_t bearing_y, uint32_t advance);
	};
}
#endif // kgl_font_glyph_info_h__