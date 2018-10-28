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
#include "kgl_font_renderer.h"
#include "kgl_font_veretx.h"
#include "kgl_string_convertor.h"
#include "kgl_error.h"

namespace kgl
{
    FontRenderer::FontRenderer()
    {
        rs_depth_not_enabled_.SetEnable(GL_FALSE); // 绘制文字时不使用深度测试，直接写入，同时不写入深度值到深度缓冲区
        rs_cull_draw_.SetEnable(GL_FALSE);
    }

    void FontRenderer::Initialize()
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
        FontTexture* t = new FontTexture(texture_name, font_size, texture_width, texture_height);
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
        *ndc_x = (static_cast<float>(x)* 2.0f - view_port_width_) / view_port_width_;
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

    void FontRenderer::AddToRendered(const std::wstring& text, int32_t start_x, int32_t start_y,
        const glm::vec4& color, float scale)
    {
        this->AddToRendered(text, start_x, start_y, color, color, color, color, scale);
    }

    void FontRenderer::AddToRendered(const std::wstring& text, int32_t start_x, int32_t start_y, const glm::vec4& left_top_color,
        const glm::vec4& left_bottom_color, const glm::vec4& right_top_color, const glm::vec4& right_bottom_color, float scale)
    {
        int32_t char_count = (int32_t)text.size();

        float font_w, font_h = 0.f;
        float x, y = 0.0f;
        this->ScreenToNDC(start_x, start_y, &x, &y);

        std::vector<FontVertex> font_vertices;
        font_vertices.resize(text.size() * 6);

        /* 采用三角形列表的方式做索引
        0 2 4 6 8 10
        1 3 5 7 9 11
        */

        for (int32_t t = 0; t < char_count; ++t)
        {
            current_font_texture_->RenderGlyphIntoTexture(uint32_t(text[t]));

            const FontGlyphInfo* glyph_info = current_font_texture_->GetGlyphInfo((uint32_t)text[t]);
            const FontUVRect uv_rect = glyph_info->texture_uv_rect;

            GLfloat lt_x = x + glyph_info->bearing_x / view_port_width_* scale;
            GLfloat lt_y = y - (glyph_info->bmp_height - glyph_info->bearing_y) / view_port_height_ * scale;

            font_h = glyph_info->bmp_height / view_port_height_ * scale;
            font_w = glyph_info->bmp_width / view_port_width_  * scale;

            // 纹理坐标是上下对调的，即左下角用左上角的纹理坐标，右下角用右上角的纹理坐标
            font_vertices[t * 6 + 0].pos_x = lt_x; // 左下
            font_vertices[t * 6 + 0].pos_y = lt_y - font_h;
            font_vertices[t * 6 + 0].red = left_bottom_color.r;
            font_vertices[t * 6 + 0].green = left_bottom_color.g;
            font_vertices[t * 6 + 0].blue = left_bottom_color.b;
            font_vertices[t * 6 + 0].alpha = left_bottom_color.a;
            font_vertices[t * 6 + 0].texture_u = uv_rect.left;
            font_vertices[t * 6 + 0].texture_v = uv_rect.bottom;// font_h;

            font_vertices[t * 6 + 1].pos_x = lt_x; // 左上
            font_vertices[t * 6 + 1].pos_y = lt_y;
            font_vertices[t * 6 + 1].red = left_top_color.r;
            font_vertices[t * 6 + 1].green = left_top_color.g;
            font_vertices[t * 6 + 1].blue = left_top_color.b;
            font_vertices[t * 6 + 1].alpha = left_top_color.a;
            font_vertices[t * 6 + 1].texture_u = uv_rect.left;
            font_vertices[t * 6 + 1].texture_v = uv_rect.top;

            font_vertices[t * 6 + 2].pos_x = lt_x + font_w;// 右上
            font_vertices[t * 6 + 2].pos_y = lt_y;
            font_vertices[t * 6 + 2].red = right_top_color.r;
            font_vertices[t * 6 + 2].green = right_top_color.g;
            font_vertices[t * 6 + 2].blue = right_top_color.b;
            font_vertices[t * 6 + 2].alpha = right_top_color.a;
            font_vertices[t * 6 + 2].texture_u = uv_rect.right;
            font_vertices[t * 6 + 2].texture_v = uv_rect.top;

            font_vertices[t * 6 + 3].pos_x = lt_x + font_w; // 右上
            font_vertices[t * 6 + 3].pos_y = lt_y;
            font_vertices[t * 6 + 3].red = right_top_color.r;
            font_vertices[t * 6 + 3].green = right_top_color.g;
            font_vertices[t * 6 + 3].blue = right_top_color.b;
            font_vertices[t * 6 + 3].alpha = right_top_color.a;
            font_vertices[t * 6 + 3].texture_u = uv_rect.right;
            font_vertices[t * 6 + 3].texture_v = uv_rect.top;

            font_vertices[t * 6 + 4].pos_x = lt_x + font_w; // 右下
            font_vertices[t * 6 + 4].pos_y = lt_y - font_h;
            font_vertices[t * 6 + 4].red = right_bottom_color.r;
            font_vertices[t * 6 + 4].green = right_bottom_color.g;
            font_vertices[t * 6 + 4].blue = right_bottom_color.b;
            font_vertices[t * 6 + 4].alpha = right_bottom_color.a;
            font_vertices[t * 6 + 4].texture_u = uv_rect.right;
            font_vertices[t * 6 + 4].texture_v = uv_rect.bottom;

            font_vertices[t * 6 + 5].pos_x = lt_x; // 左下
            font_vertices[t * 6 + 5].pos_y = lt_y - font_h;
            font_vertices[t * 6 + 5].red = left_bottom_color.r;
            font_vertices[t * 6 + 5].green = left_bottom_color.g;
            font_vertices[t * 6 + 5].blue = left_bottom_color.b;
            font_vertices[t * 6 + 5].alpha = left_bottom_color.a;
            font_vertices[t * 6 + 5].texture_u = uv_rect.left;
            font_vertices[t * 6 + 5].texture_v = uv_rect.bottom;

            x += (glyph_info->advance) / view_port_width_ * scale;
        }


        FontPrimitive* p = this->GetUnusedFontPrimitive(char_count);
        std::size_t sz = sizeof(FontVertex);
        p->SetUse(true);
        p->SetFontTexture(current_font_texture_);
        p->UpdateData(&(font_vertices[0]), char_count * sz * 6, char_count * 6);
        used_primitive_array_.push_back(p);
        // 如果直接使用unused_primitive_set_.erase(p)的话，就会删除掉所有的
        // 和待删除的p的ActualVerticesCount相等的font primitive
        unused_primitive_set_.erase(unused_primitive_set_.find(p)); 
    }

    void FontRenderer::Draw()
    {
        // 绘制文字前用到的深度缓冲区状态对象，拣选模式，文字绘制完毕之后会重新使用回它
        RenderStateCullMode cull_mode_before_draw;
        RenderStateDepth depth_test_before_draw;
        RenderStateBlend blend_mode_before_draw;
        RenderStateBlend::TakeSnapshotState(blend_mode_before_draw);
        RenderStateCullMode::TakeSnapshotState(cull_mode_before_draw);
        RenderStateDepth::TakeSnapshotState(depth_test_before_draw);

        draw_mode_.SetCurrentDrawMode(DM_FILL);
        rs_depth_not_enabled_.Use();
        rs_blend_.Use();
        rs_cull_draw_.Use();

        font_shader_->Use();

        std::size_t sz = used_primitive_array_.size();

        for (std::size_t t = 0; t < sz; ++t)
        {
            FontPrimitive* p = used_primitive_array_[t];
            font_shader_->ApplyTexture(p->GetFontTexture()->GetTexture(), "font_texture", 0);
            p->Draw();
            p->SetUse(false);
            /*
             因为在FontPrimitiveGreater类中是利用了FontPrimitve的ActualVerticesCount
             值作为大小的比较所以如果FontPrimitive的ActualVerticesCount值和已有的font
             primitive的ActualVerticesCount值相等的话就算这里的p值是独一无二的，都因为
             unused_primitive_set_是set类型，且ActualVertcesCount值相等而不能插入新的
             font primitive到set里面。所以unused_primitive_set_需要改成multi set*/
            unused_primitive_set_.insert(p);
        }

        used_primitive_array_.clear();
        depth_test_before_draw.Use();
        cull_mode_before_draw.Use();
        blend_mode_before_draw.Use();
    }

    FontPrimitive* FontRenderer::GetUnusedFontPrimitive(int32_t char_count)
    {
        FontPrimitive* p = nullptr;

        if (unused_primitive_set_.empty())
        {
            p = new FontPrimitive(char_count);
            p->Create();
            unused_primitive_set_.insert(p);
            //OutputDebugStringA("GetUnusedFontPrimitive unused_primitive_set_.empty\n");
            return p;
        }

        p = *(unused_primitive_set_.begin());
        
        if (p->ActualVerticesCount() < char_count * 6)
        {
            p = new FontPrimitive(char_count);
            p->Create();
            unused_primitive_set_.insert(p);
            //OutputDebugStringA("GetUnusedFontPrimitive p->ActualVerticesCount() < char_count * 6\n");
            return p;
        }

        return p;
    }
}