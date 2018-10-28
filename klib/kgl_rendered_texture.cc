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
#include "kgl_rendered_texture.h"
#include "kgl_error.h"

namespace kgl
{
    RenderedTexture::RenderedTexture(RenderedTexelType used_type)
    {
        type_ = RENDERED_TEXTURE;
        texel_type_ = used_type;
    }

    RenderedTexture::~RenderedTexture()
    {
    }
    
    void RenderedTexture::Create(int32_t width, int32_t height, const TextureParams& texture_params)
    {
        // https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture_id_);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            texture_params.internal_format,         // 指定在显卡中的内部格式
            width,                                  // 纹理的宽度
            height,                                 // 纹理的高度
            0,
            texture_params.src_img_format,             // 指定纹理像素格式为RGB
            texture_params.src_img_px_component_type,  // 指明每个颜色分量使用一个字节
            nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_params.min_filter_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_params.mag_filter_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_params.wrap_s_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_params.wrap_t_mode);

        if (RenderedTexelType::DEPTH_COMPONENT == texel_type_)
        {
            float border_color[] = { 1.0, 1.0, 1.0, 1.0 };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        width_ = width;
        height_ = height;
        valid_ = true;
    }

    int RenderedTexture::GetWidth() const
    {
        return width_;
    }

    int RenderedTexture::GetHeight() const
    {
        return height_;
    }

    bool RenderedTexture::IsMipmap() const
    {
        return false;
    }

    void RenderedTexture::ActiveBind(GLuint slot_index)
    {
        GLuint i = glm::clamp(slot_index, static_cast<GLuint>(0), static_cast<GLuint>(8));
        GLenum texture_unit = GL_TEXTURE0 + i;
        glActiveTexture(texture_unit);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
    }

    GLenum RenderedTexture::GetAttachmentForFrameBuffer()
    {
        if (texel_type_ == RenderedTexelType::DEPTH_COMPONENT)
            return GL_DEPTH_ATTACHMENT;
        else
            return GL_COLOR_ATTACHMENT0;
    }
}