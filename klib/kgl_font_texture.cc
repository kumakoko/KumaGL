// 字体纹理类
#include "kgl_lib_pch.h"
#include "kgl_defines.h"
#include "kgl_font_texture.h"
#include "kgl_string_convertor.h"

namespace kgl
{
	const FontUVRect& FontTexture::GetGlyphTexCoords(uint32_t id) const
	{
		CodePointMap::const_iterator i = codepoint_map_.find(id);

		if (i != codepoint_map_.end())
		{
			return i->second.UVRect;
		}
		else
		{
			return FontUVRect::ZeroRect();
		}
	}

	void FontTexture::SetGlyphTexCoords(uint32_t id, uint32_t u1_pixel, uint32_t v1_pixel, uint32_t u2_pixel, uint32_t v2_pixel, float texture_aspect,
		int32_t bearing_x, int32_t bearing_y, uint32_t advance)
	{
		float u1 = static_cast<float>(u1_pixel) / static_cast<float>(width_);
		float v1 = static_cast<float>(v1_pixel) / static_cast<float>(height_);
		float u2 = static_cast<float>(u2_pixel) / static_cast<float>(width_);
		float v2 = static_cast<float>(v2_pixel) / static_cast<float>(height_);

		FontTexture::CodePointMap::iterator i = codepoint_map_.find(id);

		if (i != codepoint_map_.end())
		{
			i->second.UVRect.Left = u1;
			i->second.UVRect.Top = v1;
			i->second.UVRect.Right = u2;
			i->second.UVRect.Bottom = v2;
			i->second.AspectRatio = texture_aspect * (u2 - u1) / (v2 - v1);
			i->second.BmpWidth = u2_pixel - u1_pixel;
			i->second.BmpHeight = v2_pixel - v1_pixel;
			i->second.Advance = advance;
			i->second.BearingX = bearing_x;
			i->second.BearingY = bearing_y;
		}
		else
		{
			FontGlyphInfo i;
			i.UVRect.Left = u1;
			i.UVRect.Top = v1;
			i.UVRect.Right = u2;
			i.UVRect.Bottom = v2;
			i.AspectRatio = texture_aspect * (u2 - u1) / (v2 - v1);
			i.BmpWidth = u2_pixel - u1_pixel;
			i.BmpHeight = v2_pixel - v1_pixel;
			i.Advance = advance;
			i.BearingX = bearing_x;
			i.BearingY = bearing_y;
			codepoint_map_.insert(CodePointMap::value_type(id,i));
		}
	}

	float FontTexture::GetGlyphAspectRatio(uint32_t id) const
	{
		CodePointMap::const_iterator i = codepoint_map_.find(id);
		return i != codepoint_map_.end() ? i->second.AspectRatio : 1.f;
	}

	void FontTexture::SetGlyphAspectRatio(uint32_t id, float ratio)
	{
		CodePointMap::iterator i = codepoint_map_.find(id);

		if (i != codepoint_map_.end())
		{
			i->second.AspectRatio = ratio;
		}
	}

	FontTexture::FontTexture(const char* texture_name, 
	int32_t font_size, uint32_t texture_width, uint32_t texture_height) :
	ttf_max_bearing_y_(0), ttf_resolution_(0),
	antialias_color_(true), left_blank_num_(0),
	image_data_(nullptr),
	img_char_top_(0), img_char_left_(0)
	{
		width_ = texture_width;
		height_ = texture_height;
		ttf_size_ = static_cast<float>(font_size);
		ttf_resolution_ = 96;
		char_spacer_ = 0;
		texture_name_ = texture_name;
	}

	FontTexture::~FontTexture()
	{
		this->Unload();
	}

	bool FontTexture::IsGlyphInfoExist(uint32_t id) const
	{
		return codepoint_map_.find(id) == codepoint_map_.end() ? false : true;
	}

	void FontTexture::Load(const char* name)
	{
		font_name_ = name;
		FT_Library ft_lib;
		std::wostringstream wss;

		//初始化库 
		if (FT_Init_FreeType(&ft_lib))
		{
			wss << L"Init freetype lib failed" << std::endl;
			throw Error(wss.str(), __FILE__, __LINE__);
		}

		if (FT_New_Face(ft_lib, font_name_.c_str(), 0, &ft_face_))
		{
			wss << L"Can not open the tff file named by:" << std::endl;
			wss << StringConvertor::ANSItoUTF16LE(name) << std::endl;
			throw Error(wss.str(), __FILE__, __LINE__);
		}

		uint32_t max_face_num = ft_face_->num_faces;
		FT_F26Dot6 ft_size = (FT_F26Dot6)(ttf_size_ * (1 << 6));

		// Set size to load glyphs as
		// FT_Set_Pixel_Sizes(ft_face_, 0, ttf_size_);
		
		if (FT_Set_Char_Size(ft_face_, ft_size, 0, ttf_resolution_, ttf_resolution_))
		{
			wss << L"Can not est character size.The size is" << ft_size << std::endl;
			throw Error(wss.str(), __FILE__, __LINE__);
		}
		
		max_height_ = 0, max_width_ = 0;

		if (codepoint_range_list_.empty())
		{
			codepoint_range_list_.push_back(CodePointRange(33, 166));
			codepoint_range_list_.push_back(CodePointRange(19968, 40869));
		}

		for (CodePointRangeList::const_iterator r = codepoint_range_list_.begin();
			r != codepoint_range_list_.end(); ++r)
		{
			const CodePointRange& range = *r;
			for (uint32_t cp = range.first; cp <= range.second; ++cp)
			{
				FT_Load_Char(ft_face_, cp, FT_LOAD_RENDER);

				if ((2 * (ft_face_->glyph->bitmap.rows << 6) - ft_face_->glyph->metrics.horiBearingY) > max_height_)
				{
					// 计算得到所有字符中的字模最高的那个
					max_height_ = (2 * (ft_face_->glyph->bitmap.rows << 6) - ft_face_->glyph->metrics.horiBearingY);
				}

				if (ft_face_->glyph->metrics.horiBearingY > ttf_max_bearing_y_)
				{
					ttf_max_bearing_y_ = ft_face_->glyph->metrics.horiBearingY;
				}

				if ((ft_face_->glyph->advance.x >> 6) + (ft_face_->glyph->metrics.horiBearingX >> 6) > max_width_)
				{
					max_width_ = (ft_face_->glyph->advance.x >> 6) + (ft_face_->glyph->metrics.horiBearingX >> 6);
				}
			}
		}

		texture_aspect_ = 1.0f;

		pixel_bytes_ = 4;
		char_data_width_ = (max_width_ + char_spacer_) * pixel_bytes_;
		data_size_ = width_ * height_ * pixel_bytes_;
		max_char_size_ = ((max_height_ >> 6) + char_spacer_) * char_data_width_;
		max_char_num_ = data_size_ / max_char_size_;
		left_blank_num_ = max_char_num_;

		image_data_ = new uint8_t[data_size_];
		memset(image_data_, 0, sizeof(uint8_t)*data_size_);

		for (size_t i = 0; i < data_size_; i += pixel_bytes_) // 将纹理设置白色,全透明
		{
			image_data_[i + 0] = 0x00; // luminance R
			image_data_[i + 1] = 0x00; // luminance G
			image_data_[i + 2] = 0x00; // luminance B
			image_data_[i + 3] = 0x00; // alpha
		}

		kgl::TextureParams texture_param;
		texture_param.WrapSMode = GL_CLAMP_TO_EDGE;
		texture_param.WrapTMode = GL_CLAMP_TO_EDGE;
		texture_param.MagFilterMode = GL_LINEAR;
		texture_param.MinFilterMode = GL_LINEAR;
		texture_param.InternalFormat = GL_RGBA;
		texture_param.SrcImgPixelComponentType = GL_UNSIGNED_BYTE;
		texture_param.SrcImgFormat = GL_RGBA;
		texture_param.LoadChannel = SOIL_LOAD_RGBA;
		texture_param.UseMipmap = false;

		font_texture_ = std::make_shared<kgl::SourceTexture>();
		font_texture_->Create(static_cast<int>(width_), static_cast<int>(height_), texture_param);
	}

	void FontTexture::Unload()
	{
		font_texture_.reset();
		// FT_Done_FreeType(ft_libaray_);
	}

	const FontGlyphInfo* FontTexture::GetGlyphInfo(uint32_t id) const
	{
		CodePointMap::const_iterator i = codepoint_map_.find(id);
		if (i == codepoint_map_.end())
		{
			return nullptr;
		}
		return &i->second;
	}

	void FontTexture::RenderGlyphIntoTexture(uint32_t id)
	{
		if (this->IsGlyphInfoExist(id))
			return;

		FT_Error ft_result = FT_Load_Char(ft_face_, id, FT_LOAD_RENDER); // 载入指定的字符对应的字形

		if (ft_result)
		{
		}

		FT_Int advance = ft_face_->glyph->advance.x >> 6; // 获取到该字符的步长值
		unsigned char* buffer = ft_face_->glyph->bitmap.buffer;

		if (!buffer)
		{

		}

		int32_t face_height = ft_face_->glyph->bitmap.rows;
		int32_t face_width = ft_face_->glyph->bitmap.width;
		int32_t sOffsetY = ft_face_->glyph->metrics.horiBearingY / 64;
		int32_t sAdvance = ft_face_->glyph->metrics.horiAdvance / 64;

		for (int32_t j = 0; j < ft_face_->glyph->bitmap.rows; ++j)
		{
			// 找到本字符的左上角Y坐标在缓冲区中的行数
			size_t row = j + img_char_top_/* + face_height*/;
			uint8_t* pDest = &image_data_[(row * width_ * pixel_bytes_) + (img_char_left_/* + face_width*/)* pixel_bytes_];

			for (int32_t k = 0; k < ft_face_->glyph->bitmap.width; ++k)
			{
				if (antialias_color_)
				{
					// 用灰度值作为颜色的RGB分量
					*pDest++ = *buffer; // R
					*pDest++ = *buffer; // G
					*pDest++ = *buffer; // B
				}
				else
				{
					// 如果不用颜色反走样的话,就是用纯黑或者纯白,不加入任何的灰度级别信息了
					*pDest++ = 0xFF;
					*pDest++ = 0xFF;
					*pDest++ = 0xFF;
				}

				*pDest++ = *buffer++; // 用直接用灰度值作为alpha值
			}
		}

		// 保存字符的纹理映射坐标
		this->SetGlyphTexCoords(id,
			img_char_left_,  // u1
			img_char_top_,  // v1
			img_char_left_ + (ft_face_->glyph->metrics.horiAdvance >> 6),//u2
			img_char_top_ + (ft_face_->glyph->metrics.vertAdvance >> 6),// v2
			texture_aspect_, 
			ft_face_->glyph->bitmap_left,
			ft_face_->glyph->bitmap_top,
			ft_face_->glyph->advance.x>>6
			);

		// 在同一行中，从左到右依次渲染到下一个位置，计算出待插入的字符的左上角位置
		img_char_left_ += (advance + char_spacer_);

		// 如果满一行了，就切换到新的一行里面去
		if (width_ - 1 < img_char_left_ + (advance))
		{
			// 得到新的一行的Y坐标，
			img_char_top_ += (max_height_ >> 6) + char_spacer_;
			img_char_left_ = 0;
		}
		--left_blank_num_;

		font_texture_->Update(0, 0, width_, height_, image_data_);
	}

	void FontTexture::InsertGlyphInfo(uint32_t id)
	{
		if (!HasBlankInTexture())// 如果纹理没有空位置了,就去掉一些未用到的
		{
			RemoveGlyph(GetLessUseChar());
		}

		RenderGlyphIntoTexture(id);
	}

	uint32_t FontTexture::GetLessUseChar()
	{
		CodePointMap::iterator i = codepoint_map_.begin(), iend = codepoint_map_.end(), iless = codepoint_map_.begin();
		while (i != iend)
		{
			if (i->second.UseCount < iless->second.UseCount)
				iless = i;
			++i;
		}
		return iless->second.CodePoint;
	}

	// 移除指定的字符
	void FontTexture::RemoveGlyph(uint32_t id)
	{
		CodePointMap::iterator it = codepoint_map_.find(id);
		if (it != codepoint_map_.end())
		{
			img_char_left_ = it->second.UVRect.Left;
			img_char_top_ = it->second.UVRect.Top;
			codepoint_map_.erase(it);
			++left_blank_num_;
		}
		else
		{
			throw Error(L"Can not find uint32_t to remove in void Font::RemoveGlyph(uint32_t id)", __FILE__, __LINE__);
		}
	}
}