// 字体图元
#include "kgl_lib_pch.h"
#include "kgl_defines.h"
#include "kgl_font_primitive.h"
#include "kgl_font_veretx.h"

namespace kgl
{
	std::vector<VertexAttribute> FontPrimitive::s_vertex_attribute_array;
	bool FontPrimitive::s_init_attribute = false;

	FontPrimitive::FontPrimitive(int32_t char_count) :primitive_(nullptr), used_(false)
	{
		if (!s_init_attribute)
		{
			kgl::VertexAttribute va_position;
			va_position.Index = 0;
			va_position.Normalized = GL_FALSE;
			va_position.Type = GL_FLOAT;
			va_position.Size = 2; // 一个“顶点位置”的属性由3个分量组成
			va_position.Stride = 8 * sizeof(GLfloat); // 每个顶点的步长为 
			va_position.Pointer = nullptr;

			kgl::VertexAttribute va_color;
			va_color.Index = 1;
			va_color.Normalized = GL_FALSE;
			va_color.Type = GL_FLOAT;
			va_color.Size = 4; // 一个“顶点颜色”的属性由3个分量组成
			va_color.Stride = 8 * sizeof(GLfloat); // 每个位置的步长为 
			va_color.Pointer = reinterpret_cast<GLvoid*> (2 * sizeof(GLfloat));

			kgl::VertexAttribute va_texture_coord;
			va_texture_coord.Index = 2;
			va_texture_coord.Normalized = GL_FALSE;
			va_texture_coord.Type = GL_FLOAT;
			va_texture_coord.Size = 2; // 一个“顶点颜色”的属性由3个分量组成
			va_texture_coord.Stride = 8 * sizeof(GLfloat); // 每个位置的步长为 
			va_texture_coord.Pointer = reinterpret_cast<GLvoid*> (6 * sizeof(GLfloat));

			s_vertex_attribute_array.push_back(va_position);
			s_vertex_attribute_array.push_back(va_color);
			s_vertex_attribute_array.push_back(va_texture_coord);

			s_init_attribute = true;
		}

		actual_vertices_count_ = 6 * char_count;
		texture_ = nullptr;
	}

	FontPrimitive::~FontPrimitive()
	{
		KGL_SAFE_DELETE(primitive_);
		texture_ = nullptr;
	}

	void FontPrimitive::Create()
	{
		if (nullptr != primitive_)
			return;

		// 每个字如果采用索引式三角形列表的话，则需要4个顶点，6个索引
		// 共 4*4 + 6*2 = 16 + 12 = 28
		// 采用索引式三角形条带的话，需要4个顶点，4个索引，共4*4 + 4*2 = 24
		// 非索引式三角形列表的话，则需要6个顶点，共24字节，
		// 所以直接使用非索引式三角形列表最方便也最省
		std::vector<FontVertex> vertices;
		vertices.resize((actual_vertices_count_ ));

		// 按列优先的方式填写，即左上，左下，右上，右下的方式
		for (std::size_t t = 0; t < vertices.size(); ++t)
		{
			FontVertex& v = vertices[t];
			v.Reset();
		}

		primitive_ = new Primitive;
		GLvoid* vertices_data = &(vertices[0]);
		primitive_->Create(GL_TRIANGLES,
			vertices_data,
			sizeof(FontVertex)* (actual_vertices_count_),
			actual_vertices_count_,
			GL_DYNAMIC_DRAW, s_vertex_attribute_array);
	}

	void FontPrimitive::Draw()
	{
		if (used_)
			primitive_->Draw();
	}

	void FontPrimitive::UpdateData(const void* src_data, std::size_t src_data_byte_count, std::int32_t drawn_vertices_count)
	{
		primitive_->UpdateAllVertexData(src_data, src_data_byte_count);
		primitive_->SetDrawnVerticesCount(drawn_vertices_count);
	}
}