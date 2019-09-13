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
#include "kgl_source_texture.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_defines.h"
#include "kgl_image_file_reader.h"

//#if defined(__APPLE__) && defined(__MACH__)
//#include "SOIL.h"
//#endif

namespace kgl
{
    SourceTexture::SourceTexture()
    {
        type_ = TextureType::SOURCE_2D_TEXTURE;
    }

    SourceTexture::~SourceTexture()
    {
    }

    void SourceTexture::Create(int32_t width, int32_t height, const TextureParams& texture_params)
    {
        params_ = texture_params;
        width_ = width;
        height_ = height;

        std::vector<std::string> error_desc_array;
        std::vector<GLenum> error_code_array;

        // 创建纹理
        glGenTextures(1, &texture_id_);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 绑定纹理
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 设置纹理参数,包装模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_params.wrap_s_mode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_params.wrap_t_mode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 设置纹理参数,包装模式过滤模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_params.min_filter_mode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_params.mag_filter_mode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 装载并生成纹理的mipmap（如果有的话）
        unsigned char* image = nullptr;

        if (GL_RGB == texture_params.internal_format)
        {
            switch ( texture_params.src_img_px_component_type)
            {
            case GL_UNSIGNED_BYTE:
                image = new unsigned char[width*height * 3];
                memset(image, 0, sizeof(unsigned char) * width * height * 3);
            default:break;
            }
        }
        else if (GL_RGBA == texture_params.internal_format)
        {
            switch ( texture_params.src_img_px_component_type)
            {
            case GL_UNSIGNED_BYTE:
                image = new unsigned char[width*height * 4];
                memset(image, 0, sizeof(unsigned char) * width * height * 4);
            default:break;
            }
        }
        else if (GL_LUMINANCE == texture_params.internal_format)
        {
            switch ( texture_params.src_img_px_component_type)
            {
            case GL_UNSIGNED_BYTE:break;
                image = new unsigned char[width*height];
                memset(image, 0, sizeof(unsigned char) * width * height);
            default:break;
            }
        }
        else if (GL_LUMINANCE_ALPHA == texture_params.internal_format)
        {
            switch (texture_params.src_img_px_component_type)
            {
            case GL_UNSIGNED_BYTE:break;
                image = new unsigned char[width*height * 2];
                memset(image, 0, sizeof(unsigned char) * width * height * 2);
            default:break;
            }
        }
        else
        {
            THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
        }

        if (nullptr == image)
        {
            std::wstringstream wss;
            wss << L"Can not create texture ";
            throw Error(wss.str(), __FILE__, __LINE__);
        }

        // glTexImage2D函数的参数季节，参见TechNode/glTexImage2D函数的参数详解.txt
        glTexImage2D(GL_TEXTURE_2D, 0, texture_params.internal_format, width_, height_, 0, texture_params.src_img_format, texture_params.src_img_px_component_type, image);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        if (texture_params.used_mipmap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
            THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
        }

        // 创建完毕之后要解绑定
        glBindTexture(GL_TEXTURE_2D, 0);
        KGL_SAFE_DELETE_ARRAY(image);
        valid_ = true;
    }

    void SourceTexture::CreateFromFile(const std::string& file_name, const TextureParams& texture_params)
    {
        params_ = texture_params;

        std::vector<std::string> error_desc_array;
        std::vector<GLenum> error_code_array;

        // 创建纹理
        glGenTextures(1, &texture_id_);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 绑定纹理
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 设置纹理参数,包装模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_params.wrap_s_mode);    // Set texture wrapping to GL_REPEAT
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_params.wrap_t_mode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 设置纹理参数,包装模式过滤模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_params.min_filter_mode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_params.mag_filter_mode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        ImageFileReader img_reader;
        img_reader.LoadFromFile(file_name);
        // GLBitmapSPtr bmp = img_reader.GetGLBitmapFromFile(file_name, texture_params.internal_format);
        width_ = img_reader.ImageWidth();
        height_ = img_reader.ImageHeight();
        
        glTexImage2D(GL_TEXTURE_2D, 0, texture_params.internal_format, width_, height_, 0,
            texture_params.src_img_format, texture_params.src_img_px_component_type, img_reader.ImageData());
        
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        if (texture_params.used_mipmap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
            THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
        }

        glBindTexture(GL_TEXTURE_2D, 0); // 创建完毕之后要解绑定
        valid_ = true;
    }

    int SourceTexture::GetWidth() const
    {
        return width_;
    }

    int SourceTexture::GetHeight() const
    {
        return height_;
    }

    bool SourceTexture::IsMipmap() const
    {
        return params_.used_mipmap;
    }

   
    void SourceTexture::ActiveBind(GLuint slot_index)
    {
        GLuint i = glm::clamp(slot_index, static_cast<GLuint>(0), static_cast<GLuint>(8));
        GLenum texture_unit = GL_TEXTURE0 + i;
        glActiveTexture(texture_unit);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
    }

    /*
    In modern OpenGL there are 4 different methods to update 2D textures:

    1) glTexImage2D - the slowest one, recreates internal data structures.

    2) glTexSubImage2D - a bit faster, but can't change the parameters (size, pixel format) of the image.

	3) Render-to-texture with FBO - update texture entirely on GPU, very fast. Refer to this answer for more details: http://stackoverflow.com/a/10702468/1065190

	4) Pixel Buffer Object PBO - for fast uploads from CPU to GPU, not supported (yet) on OpenGL ES.
	*/
	void SourceTexture::Update(GLint x_offset, GLint y_offset, GLsizei width, GLsizei height, const GLvoid* data)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id_);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, width, height, params_.internal_format, params_.src_img_px_component_type, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
