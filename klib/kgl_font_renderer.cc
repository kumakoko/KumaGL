// 文字渲染器
#include "kgl_lib_pch.h"
#include "kgl_defines.h"
#include "kgl_font_renderer.h"
#include "kgl_font_veretx.h"
#include "kgl_string_convertor.h"
#include "kgl_error.h"

namespace kgl
{
	FontRenderer::FontRenderer()
	{
		current_font_texture_ = nullptr;
		font_shader_ = new kgl::GPUProgram;
		font_shader_->CreateFromFile("resources/shader/font_vs.glsl", "resources/shader/font_fs.glsl", nullptr);
		view_port_width_ = 800.0f;
		view_port_height_ = 600.0f;
	}

	void FontRenderer::CreateFontTexture(const char* font_file_name, const char* texture_name,
		int32_t font_size, uint32_t texture_width, uint32_t texture_height)
	{
		FontTexture* t = new FontTexture(texture_name,font_size, texture_width,texture_height);
		t->Load(font_file_name);
		font_texture_map_[t->GetName()] = t;
	}

	void FontRenderer::SetCurrentFont(const std::string& texture_name)
	{
		std::map<std::string, FontTexture*>::iterator it = font_texture_map_.find(texture_name);

		if (it == font_texture_map_.end())
		{
			std::wstringstream wss;
			wss << L"Can not open the font texture named:" << std::endl;
			wss << StringConvertor::ANSItoUTF16LE(texture_name.c_str()) << std::endl;
			throw Error(wss.str(), __FILE__, __LINE__);
		}
		else
		{
			current_font_texture_ = it->second;
		}
	}

	void FontRenderer::ScreenToNDC(int32_t x, int32_t y, float* ndc_x, float* ndc_y)
	{
		*ndc_x = (static_cast<float>(x) * 2.0f - view_port_width_) / view_port_width_;
		*ndc_y = (view_port_height_ - static_cast<float>(y)* 2.0f) / view_port_height_;
	}

	FontRenderer::~FontRenderer()
	{
		std::set<FontPrimitive*, FontPrimitiveGreater>::iterator it = unused_primitive_set_.begin();

		for (; it != unused_primitive_set_.end(); ++it)
		{
			FontPrimitive* p = *(it);
			KGL_SAFE_DELETE(p);
		}

		unused_primitive_set_.clear();
		used_primitive_array_.clear();

		std::map<std::string, FontTexture*>::iterator i = font_texture_map_.begin();

		for (; i != font_texture_map_.end(); ++i)
		{
			FontTexture* t = i->second;
			KGL_SAFE_DELETE(t);
		}

		KGL_SAFE_DELETE(font_shader_);
	}

	void FontRenderer::AddToRendered(const std::wstring& text, int32_t start_x, int32_t start_y, const glm::vec4& left_top_color,
		const glm::vec4& left_bottom_color,const glm::vec4& right_top_color, const glm::vec4& right_bottom_color,float scale )
	{
		int32_t char_count = (int32_t)text.size();

		float font_w, font_h = 0.f;
		float x , y = 0.0f;
		this->ScreenToNDC(start_x, start_y, &x, &y);
		
		std::vector<FontVertex> font_vertices;
		font_vertices.resize(text.size()*6);

		/* 采用三角形列表的方式做索引
		0 2 4 6 8 10
		1 3 5 7 9 11
		*/

		for (int32_t t = 0; t < char_count; ++t)
		{
			current_font_texture_->RenderGlyphIntoTexture(uint32_t(text[t]));
			
			const FontGlyphInfo* glyph_info = current_font_texture_->GetGlyphInfo((uint32_t)text[t]);
			const FontUVRect uv_rect = glyph_info->UVRect;
			
			GLfloat lt_x = x + glyph_info->BearingX / view_port_width_* scale;
			GLfloat lt_y = y - (glyph_info->BmpHeight - glyph_info->BearingY) / view_port_height_ * scale;

			font_h = glyph_info->BmpHeight / view_port_height_ * scale;
			font_w = glyph_info->BmpWidth / view_port_width_  * scale;

			// 纹理坐标是上下对调的，即左下角用左上角的纹理坐标，右下角用右上角的纹理坐标
			font_vertices[t * 6 + 0].PosX = lt_x; // 左下
			font_vertices[t * 6 + 0].PosY = lt_y - font_h;
			font_vertices[t * 6 + 0].R = left_bottom_color.r;
			font_vertices[t * 6 + 0].G = left_bottom_color.g;
			font_vertices[t * 6 + 0].B = left_bottom_color.b;
			font_vertices[t * 6 + 0].A = left_bottom_color.a;
			font_vertices[t * 6 + 0].U = uv_rect.Left;
			font_vertices[t * 6 + 0].V = uv_rect.Bottom;// font_h;

			font_vertices[t * 6 + 1].PosX = lt_x; // 左上
			font_vertices[t * 6 + 1].PosY = lt_y;
			font_vertices[t * 6 + 1].R = left_top_color.r;
			font_vertices[t * 6 + 1].G = left_top_color.g;
			font_vertices[t * 6 + 1].B = left_top_color.b;
			font_vertices[t * 6 + 1].A = left_top_color.a;
			font_vertices[t * 6 + 1].U = uv_rect.Left;
			font_vertices[t * 6 + 1].V = uv_rect.Top;

			font_vertices[t * 6 + 2].PosX = lt_x + font_w;// 右上
			font_vertices[t * 6 + 2].PosY = lt_y;
			font_vertices[t * 6 + 2].R = right_top_color.r;
			font_vertices[t * 6 + 2].G = right_top_color.g;
			font_vertices[t * 6 + 2].B = right_top_color.b;
			font_vertices[t * 6 + 2].A = right_top_color.a;
			font_vertices[t * 6 + 2].U = uv_rect.Right;
			font_vertices[t * 6 + 2].V = uv_rect.Top;

			font_vertices[t * 6 + 3].PosX = lt_x + font_w; // 右上
			font_vertices[t * 6 + 3].PosY = lt_y;
			font_vertices[t * 6 + 3].R = right_top_color.r;
			font_vertices[t * 6 + 3].G = right_top_color.g;
			font_vertices[t * 6 + 3].B = right_top_color.b;
			font_vertices[t * 6 + 3].A = right_top_color.a;
			font_vertices[t * 6 + 3].U = uv_rect.Right;
			font_vertices[t * 6 + 3].V = uv_rect.Top;

			font_vertices[t * 6 + 4].PosX = lt_x + font_w; // 右下
			font_vertices[t * 6 + 4].PosY = lt_y - font_h;
			font_vertices[t * 6 + 4].R = right_bottom_color.r;
			font_vertices[t * 6 + 4].G = right_bottom_color.g;
			font_vertices[t * 6 + 4].B = right_bottom_color.b;
			font_vertices[t * 6 + 4].A = right_bottom_color.a;
			font_vertices[t * 6 + 4].U = uv_rect.Right;
			font_vertices[t * 6 + 4].V = uv_rect.Bottom;

			font_vertices[t * 6 + 5].PosX = lt_x; // 左下
			font_vertices[t * 6 + 5].PosY = lt_y - font_h;
			font_vertices[t * 6 + 5].R = left_bottom_color.r;
			font_vertices[t * 6 + 5].G = left_bottom_color.g;
			font_vertices[t * 6 + 5].B = left_bottom_color.b;
			font_vertices[t * 6 + 5].A = left_bottom_color.a;
			font_vertices[t * 6 + 5].U = uv_rect.Left;
			font_vertices[t * 6 + 5].V = uv_rect.Bottom;

			x += (glyph_info->Advance) / view_port_width_ * scale;
		}

		FontPrimitive* p = this->GetUnusedFontPrimitive(char_count);
		std::size_t sz = sizeof(FontVertex);
		p->SetUse(true);
		p->SetFontTexture(current_font_texture_);
		p->UpdateData(&(font_vertices[0]), char_count * sz * 6, char_count * 6);
		used_primitive_array_.push_back(p);
	}

	void FontRenderer::Draw()
	{
		font_shader_->Use();
		std::size_t sz = used_primitive_array_.size();

		for (std::size_t t = 0; t < sz; ++t)
		{
			FontPrimitive* p = used_primitive_array_[t];
			font_shader_->ApplyTexture(p->GetFontTexture()->GetTexture(), "font_texture", 0);
			p->Draw();
			p->SetUse(false);
			unused_primitive_set_.insert(p);
		}

		used_primitive_array_.clear();
	}

	FontPrimitive* FontRenderer::GetUnusedFontPrimitive(int32_t char_count)
	{
		FontPrimitive* p = nullptr;

		if (unused_primitive_set_.empty())
		{
			p = new FontPrimitive(char_count);
			p->Create();
			unused_primitive_set_.insert(p);
			return p;
		}

		p = *(unused_primitive_set_.begin());

		if (p->ActualVerticesCount() < char_count * 6)
		{
			p = new FontPrimitive(char_count);
			p->Create();
			unused_primitive_set_.insert(p);
			return p;
		}

		unused_primitive_set_.erase(p);
		return p;
	}
}