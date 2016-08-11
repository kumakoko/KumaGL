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
		FontPrimitive(int32_t char_count);
		~FontPrimitive();
		void Create();
		void Draw();

		inline void SetUse(bool used)
		{
			used_ = used;
		}

		inline bool IsUsed() const
		{
			return used_;
		}

		inline std::int32_t ActualVerticesCount() const
		{
			return actual_vertices_count_;
		}

		void UpdateData(const void* src_data, std::size_t src_data_byte_count, std::int32_t drawn_vertices_count);

		inline FontTexture* GetFontTexture()
		{
			return texture_;
		}

		inline void SetFontTexture(FontTexture* t)
		{
			texture_ = t;
		}

	private:
		static std::vector<VertexAttribute> s_vertex_attribute_array;
		static bool			s_init_attribute;
		Primitive*			primitive_;
		FontTexture*		texture_;
		bool				used_;
		std::int32_t		actual_vertices_count_; // 实际的顶点数
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