// 纹理类的基类
#include "kgl_lib_pch.h"
#include "kgl_source_texture.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_defines.h"

namespace kgl
{
    SourceTexture::SourceTexture() : texture_id_(0), width_(0), height_(0)
    {
        type_ = SOURCE_2D_TEXTURE;
    }

    SourceTexture::~SourceTexture()
    {

    }

    void SourceTexture::Create(int width, int height, const TextureParams& texture_params)
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_params.WrapSMode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_params.WrapTMode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 设置纹理参数,包装模式过滤模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_params.MinFilterMode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_params.MagFilterMode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 装载并生成纹理的mipmap（如果有的话）
        unsigned char* image = nullptr;

        if (GL_RGB == texture_params.InternalFormat)
        {
            switch ( texture_params.SrcImgPixelComponentType)
            {
            case GL_UNSIGNED_BYTE:
                image = new unsigned char[width*height * 3];
                memset(image, 0, sizeof(unsigned char) * width * height * 3);
            default:break;
            }
        }
        else if (GL_RGBA == texture_params.InternalFormat)
        {
            switch ( texture_params.SrcImgPixelComponentType)
            {
            case GL_UNSIGNED_BYTE:
                image = new unsigned char[width*height * 4];
                memset(image, 0, sizeof(unsigned char) * width * height * 4);
            default:break;
            }
        }
        else if (GL_LUMINANCE == texture_params.InternalFormat)
        {
            switch ( texture_params.SrcImgPixelComponentType)
            {
            case GL_UNSIGNED_BYTE:break;
                image = new unsigned char[width*height];
                memset(image, 0, sizeof(unsigned char) * width * height);
            default:break;
            }
        }
        else if (GL_LUMINANCE_ALPHA == texture_params.InternalFormat)
        {
            switch (texture_params.SrcImgPixelComponentType)
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

        glTexImage2D(GL_TEXTURE_2D, 0, texture_params.InternalFormat, width_, height_, 0, texture_params.SrcImgFormat, texture_params.SrcImgPixelComponentType, image);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        if (texture_params.UseMipmap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
            THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
        }

        // 创建完毕之后要解绑定
        glBindTexture(GL_TEXTURE_2D, 0);

        KGL_SAFE_DELETE_ARRAY(image);
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_params.WrapSMode);    // Set texture wrapping to GL_REPEAT
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_params.WrapTMode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 设置纹理参数,包装模式过滤模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_params.MinFilterMode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_params.MagFilterMode);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        // 装载并生成纹理的mipmap（如果有的话）
        unsigned char* image = SOIL_load_image(file_name.c_str(), &width_, &height_, 0, texture_params.LoadChannel);

        if (nullptr == image)
        {
            std::wstringstream wss;
            wss << L"Can not load image file from " << StringConvertor::ANSItoUTF16LE(file_name.c_str());
            throw Error(wss.str(), __FILE__, __LINE__);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, texture_params.InternalFormat, width_, height_, 0, texture_params.SrcImgFormat, texture_params.SrcImgPixelComponentType, image);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        if (texture_params.UseMipmap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
            THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
        }

        SOIL_free_image_data(image);

        // 创建完毕之后要解绑定
        glBindTexture(GL_TEXTURE_2D, 0);
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
        return params_.UseMipmap;
    }

    //************************************
    // Method:    ActiveBind
    // FullName:  kgl::Texture::ActiveBind
    // Access:    virtual public 
    // Returns:   void
    // Qualifier: 激活并绑定本纹理到OpenGL管线
    // Parameter: int slot_index
    //************************************
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
		glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, width, height, params_.InternalFormat, params_.SrcImgPixelComponentType, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}