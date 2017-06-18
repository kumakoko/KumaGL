// 字体图元
#ifndef kgl_font_primitive_h__
#define kgl_font_primitive_h__

#include "kgl_primitive.h"
#include "kgl_vertex_attribute.h"
#include "kgl_font_texture.h"

namespace kgl
{
    class FontPrimitive
    {
    public:
		/// <summary>
		/// Initializes a new instance of the <see cref="FontPrimitive"/> class.
		/// </summary>
		/// <param name="char_count">The char_count.</param>
		FontPrimitive(int32_t char_count);

		/// <summary>
		/// Finalizes an instance of the <see cref="FontPrimitive"/> class.
		/// </summary>
		~FontPrimitive();

		/// <summary>
		/// Creates this instance.
		/// </summary>
		void Create();

		/// <summary>
		/// Draws this instance.
		/// </summary>
		void Draw();

		/// <summary>
		/// Sets the use.
		/// </summary>
		/// <param name="used">The used.</param>
		inline void SetUse(bool used)
        {
            used_ = used;
        }

		/// <summary>
		/// Determines whether this instance is used.
		/// </summary>
		/// <returns>bool.</returns>
		inline bool IsUsed() const
        {
            return used_;
        }

		/// <summary>
		/// Actuals the vertices count.
		/// </summary>
		/// <returns>std.int32_t.</returns>
		inline std::int32_t ActualVerticesCount() const
        {
            return actual_vertices_count_;
        }

		/// <summary>
		/// Updates the data.
		/// </summary>
		/// <param name="src_data">The src_data.</param>
		/// <param name="src_data_byte_count">The src_data_byte_count.</param>
		/// <param name="drawn_vertices_count">The drawn_vertices_count.</param>
		void UpdateData(const void* src_data, std::size_t src_data_byte_count, std::int32_t drawn_vertices_count);

		/// <summary>
		/// Gets the font texture.
		/// </summary>
		/// <returns>kgl.FontTexture *.</returns>
		inline FontTexture* GetFontTexture()
        {
            return texture_;
        }

		/// <summary>
		/// Sets the font texture.
		/// </summary>
		/// <param name="t">The t.</param>
		inline void SetFontTexture(FontTexture* t)
        {
            texture_ = t;
        }

    private:
        static std::vector<VertexAttribute> s_vertex_attribute_array;

        static bool s_init_attribute;

		/// <summary>
		/// The primitive_
		/// </summary>
		Primitive* primitive_;
		/// <summary>
		/// The texture_
		/// </summary>
		FontTexture* texture_;

		/// <summary>
		/// The used_
		/// </summary>
		bool used_;

		/// <summary>
		/// 实际的顶点数
		/// </summary>
		std::int32_t actual_vertices_count_;
    };

    struct FontPrimitiveGreater
    {
        bool operator() (const FontPrimitive* p1, const FontPrimitive* p2) const
        {
            if (p1->ActualVerticesCount() > p2->ActualVerticesCount())
                return true;

            return false;
        }
    };
}
#endif // kgl_font_primitive_h__