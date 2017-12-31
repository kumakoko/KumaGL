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
        // GLenum attachment_type;
        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture_id_);

        GLint internal_format = GL_RGB;
        GLenum texel_format = GL_RGB;
        GLenum texel_bit_type = GL_UNSIGNED_BYTE;

        switch (texel_type_)
        {
        default:
        case RenderedTexelType::RGB:
            internal_format = GL_RGB;
            texel_format = GL_RGB;
            texel_bit_type = GL_UNSIGNED_BYTE;
            break;
        case RenderedTexelType::RGBA:
            internal_format = GL_RGBA;
            texel_format = GL_RGBA;
            texel_bit_type = GL_UNSIGNED_BYTE;
            break;
        case RenderedTexelType::RGBA16F:
            internal_format = GL_RGBA16F;
            texel_format = GL_RGBA;
            texel_bit_type = GL_FLOAT; // 浮点纹理  
            break;
        }
        
        glTexImage2D( 
            GL_TEXTURE_2D,
            0,     
            internal_format, // 使用GL_RGB，表明指定texture中的颜色分量为3个
            width,           // 纹理的宽度
            height,          // 纹理的高度
            0,
            texel_format,    // 指定纹理像素格式为RGB
            texel_bit_type,  // 指明每个颜色分量使用一个字节
            nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
}