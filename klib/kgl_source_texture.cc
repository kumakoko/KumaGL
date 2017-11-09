// 纹理类的基类
#include "kgl_lib_pch.h"
#include "kgl_source_texture.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_defines.h"
#include "kgl_image_file_reader.h"

#if defined(__APPLE__) && defined(__MACH__)
#include "SOIL.h"
#endif

namespace kgl
{
    SourceTexture::SourceTexture() : texture_id_(0), width_(0), height_(0)
    {
        type_ = SOURCE_2D_TEXTURE;
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
    }

    void SourceTexture::CreateFromFileObsolete(const std::string& file_name, const TextureParams& texture_params)
	{/*
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

        // 装载并生成纹理的mipmap（如果有的话）
        unsigned char* image = SOIL_load_image(file_name.c_str(), &width_, &height_, 0, texture_params.load_channel);

        if (nullptr == image)
        {
            std::wstringstream wss;
            wss << L"Can not load image file from " << StringConvertor::ANSItoUTF16LE(file_name.c_str());
            throw Error(wss.str(), __FILE__, __LINE__);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, texture_params.internal_format, width_, height_, 0, texture_params.src_img_format, texture_params.src_img_px_component_type, image);
        THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

        if (texture_params.used_mipmap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
            THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
        }

        SOIL_free_image_data(image);

        // 创建完毕之后要解绑定
        glBindTexture(GL_TEXTURE_2D, 0);*/
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

		/*
		FreeImage_Initialise(TRUE);
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN; // 定义图片格式为未知
		fif = FreeImage_GetFileType(file_name.c_str(), 0); // 获取图片格式
		FIBITMAP* bitmap = FreeImage_Load(fif, file_name.c_str(), 0); // 根据获取格式读取图片数据

		
		
		int i, j, k;
		int pitch = FreeImage_GetPitch(fibmp);
		unsigned char *bits = FreeImage_GetBits(fibmp);
		int bpp = FreeImage_GetBPP(fibmp);
		GLBITMAP *glbmp = (GLBITMAP *)malloc(sizeof(GLBITMAP));
		RGBQUAD *palette = NULL;

		if (!glbmp) return NULL;

		glbmp->w = FreeImage_GetWidth(fibmp);
		glbmp->h = FreeImage_GetHeight(fibmp);

		switch (bpp) {
		case 8:
			if (!(palette = FreeImage_GetPalette(fibmp))) return NULL;
			if (!(glbmp->buf = (unsigned char *)malloc(glbmp->w*glbmp->h * 3))) return NULL;
			glbmp->rgb_mode = GL_RGB;
			for (i = 0; i < glbmp->h; ++i) {
				for (j = 0; j < glbmp->w; ++j) {
					k = bits[i*pitch + j];
					glbmp->buf[(i*glbmp->w + j) * 3 + 0] = palette[k].rgbRed;
					glbmp->buf[(i*glbmp->w + j) * 3 + 1] = palette[k].rgbGreen;
					glbmp->buf[(i*glbmp->w + j) * 3 + 2] = palette[k].rgbBlue;
				}
			}
			break;
		case 24:
			if (!(glbmp->buf = (unsigned char *)malloc(glbmp->w*glbmp->h * 3))) return NULL;
			glbmp->rgb_mode = GL_RGB;
			for (i = 0; i < glbmp->h; ++i) {
				for (j = 0; j < glbmp->w; ++j) {
					glbmp->buf[(i*glbmp->w + j) * 3 + 0] = bits[i*pitch + j * 3 + 2];
					glbmp->buf[(i*glbmp->w + j) * 3 + 1] = bits[i*pitch + j * 3 + 1];
					glbmp->buf[(i*glbmp->w + j) * 3 + 2] = bits[i*pitch + j * 3 + 0];
				}
			}
			break;
		case 32:
			if (!(glbmp->buf = (unsigned char *)malloc(glbmp->w*glbmp->h * 4))) return NULL;
			glbmp->rgb_mode = GL_RGBA;
			for (i = 0; i < glbmp->h; ++i) {
				for (j = 0; j < glbmp->w; ++j) {
					glbmp->buf[(i*glbmp->w + j) * 4 + 0] = bits[i*pitch + j * 4 + 2];
					glbmp->buf[(i*glbmp->w + j) * 4 + 1] = bits[i*pitch + j * 4 + 1];
					glbmp->buf[(i*glbmp->w + j) * 4 + 2] = bits[i*pitch + j * 4 + 0];
					glbmp->buf[(i*glbmp->w + j) * 4 + 3] = bits[i*pitch + j * 4 + 3];
				}
			}
			break;
		default: return NULL;
		}

		return glbmp;
		
		if (!bitmap)
		{
			throw Error(L"Can not load image file", __FILE__, __LINE__);
			return;
		}

		int x, y;
		RGBQUAD m_rgb;

		// 获取图片长和宽
		width_ = (GLint)FreeImage_GetWidth(bitmap);
		height_ = (GLint)FreeImage_GetHeight(bitmap);

		unsigned char* image = new unsigned char[width_ * height_ * 3];

		// 获取图片数据 按RGBA保存
		for (y = 0; y < height_; ++y)
		{
			for (x = 0; x < width_; ++x)
			{
				// 获取像素值
				FreeImage_GetPixelColor(bitmap, x, y, &m_rgb);

				// 将RGB存入数组
				image[y * width_ * 3 + x * 3 + 0] = m_rgb.rgbRed;
				image[y * width_ * 3 + x * 3 + 1] = m_rgb.rgbGreen;
				image[y * width_ * 3 + x * 3 + 2] = m_rgb.rgbBlue;

		
				// 判断alpha值
				if (FreeImage_IsTransparent(bitmap))
					image[y * width_ * 4 + x * 4 + 3] = m_rgb.rgbReserved;
				else
					image[y * width_ * 4 + x * 4 + 3] = 255;
				
			}
		}
		*/

		ImageFileReader img_reader;
		GLBitmapSPtr bmp = img_reader.GetGLBitmapFromFile(file_name, texture_params.internal_format);
		width_ = bmp->width();
		height_ = bmp->height();
		glTexImage2D(GL_TEXTURE_2D, 0, texture_params.internal_format, width_, height_, 0,
			texture_params.src_img_format, texture_params.src_img_px_component_type, bmp->Buffer());
		
		THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);

		if (texture_params.used_mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
		}

		// 释放
		//FreeImage_Unload(bitmap);
		
		glBindTexture(GL_TEXTURE_2D, 0); // 创建完毕之后要解绑定
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
