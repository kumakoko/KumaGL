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
#include "kgl_lib_pch.h"
#include "kgl_defines.h"
#include "kgl_font_texture.h"
#include "kgl_string_convertor.h"
#include "kgl_error.h"

namespace kgl
{
    const FontUVRect& FontTexture::GetGlyphTexCoords(uint32_t id) const
    {
        CodePointMap::const_iterator i = codepoint_map_.find(id);

        if (i != codepoint_map_.end())
        {
            return i->second.texture_uv_rect;
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
            i->second.texture_uv_rect.left = u1;
            i->second.texture_uv_rect.top = v1;
            i->second.texture_uv_rect.right = u2;
            i->second.texture_uv_rect.bottom = v2;
            i->second.aspect_ratio = texture_aspect * (u2 - u1) / (v2 - v1);
            i->second.bmp_width = u2_pixel - u1_pixel;
            i->second.bmp_height = v2_pixel - v1_pixel;
            i->second.advance = advance;
            i->second.bearing_x = bearing_x;
            i->second.bearing_y = bearing_y;
        }
        else
        {
            FontGlyphInfo i;
            i.texture_uv_rect.left = u1;
            i.texture_uv_rect.top = v1;
            i.texture_uv_rect.right = u2;
            i.texture_uv_rect.bottom = v2;
            i.aspect_ratio = texture_aspect * (u2 - u1) / (v2 - v1);
            i.bmp_width = u2_pixel - u1_pixel;
            i.bmp_height = v2_pixel - v1_pixel;
            i.advance = advance;
            i.bearing_x = bearing_x;
            i.bearing_y = bearing_y;
            codepoint_map_.insert(CodePointMap::value_type(id,i));
        }
    }

    float FontTexture::GetGlyphAspectRatio(uint32_t id) const
    {
        CodePointMap::const_iterator i = codepoint_map_.find(id);
        return i != codepoint_map_.end() ? i->second.aspect_ratio : 1.f;
    }

    void FontTexture::SetGlyphAspectRatio(uint32_t id, float ratio)
    {
        CodePointMap::iterator i = codepoint_map_.find(id);

        if (i != codepoint_map_.end())
        {
            i->second.aspect_ratio = ratio;
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
        
        std::wostringstream wss;

        //初始化库 
        if (FT_Init_FreeType(&ft_libaray_))
        {
            wss << L"Init freetype lib failed" << std::endl;
            throw Error(wss.str(), __FILE__, __LINE__);
        }

        if (FT_New_Face(ft_libaray_, font_name_.c_str(), 0, &ft_face_))
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
        texture_param.wrap_s_mode = GL_CLAMP_TO_EDGE;
        texture_param.wrap_t_mode = GL_CLAMP_TO_EDGE;
        texture_param.mag_filter_mode = GL_LINEAR;
        texture_param.min_filter_mode = GL_LINEAR;
        texture_param.internal_format = GL_RGBA;
        texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
        texture_param.src_img_format = GL_RGBA;
        texture_param.used_mipmap = false;

        font_texture_ = std::make_shared<kgl::SourceTexture>();
        font_texture_->Create(static_cast<int>(width_), static_cast<int>(height_), texture_param);
    }

    void FontTexture::Unload()
    {
        KGL_SAFE_DELETE_ARRAY(image_data_);
        font_texture_.reset();
        FT_Done_FreeType(ft_libaray_);
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
            std::wstringstream wss;
            wss << "In FontTexture::RenderGlyphIntoTexture(uint32_t id) " << std::endl;
            wss << "FT_Load_Char(ft_face_," << id << ", FT_LOAD_RENDER) ERROR. FT_Error is " << ft_result << std::endl;
            throw Error(wss.str(), __FILE__, __LINE__);
        }

        FT_Int advance = ft_face_->glyph->advance.x >> 6; // 获取到该字符的步长值
        unsigned char* buffer = ft_face_->glyph->bitmap.buffer;

        if (!buffer)
        {
            //return;
            /*
            std::wstringstream wss;
            wss << "In FontTexture::RenderGlyphIntoTexture(uint32_t id) " << std::endl;
            wss << "Get font face glyph bitmap buffer error. the buffer is empty" << std::endl;
            throw Error(wss.str(), __FILE__, __LINE__);
            */
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
                    *pDest++ = buffer == 0 ? 0x00 : *buffer; // R
                    *pDest++ = buffer == 0 ? 0x00 : *buffer; // G
                    *pDest++ = buffer == 0 ? 0x00 : *buffer; // B
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
            if (i->second.used_count < iless->second.used_count)
                iless = i;
            ++i;
        }
        return iless->second.code_point;
    }

    // 移除指定的字符
    void FontTexture::RemoveGlyph(uint32_t id)
    {
        CodePointMap::iterator it = codepoint_map_.find(id);
        if (it != codepoint_map_.end())
        {
            img_char_left_ = static_cast<uint32_t>(it->second.texture_uv_rect.left);
            img_char_top_ = static_cast<uint32_t>(it->second.texture_uv_rect.top);
            codepoint_map_.erase(it);
            ++left_blank_num_;
        }
        else
        {
            throw Error(L"Can not find uint32_t to remove in void Font::RemoveGlyph(uint32_t id)", __FILE__, __LINE__);
        }
    }
}
