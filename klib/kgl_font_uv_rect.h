// 文字包围盒类
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