// 图像读取器

#include "kgl_lib_pch.h"
#include "kgl_image_file_reader.h"

namespace kgl
{
	ImageFileReader::ImageFileReader()
	{
		FreeImage_Initialise(TRUE);
	}

	ImageFileReader::~ImageFileReader()
	{
		this->ReleaseImageData();
		FreeImage_DeInitialise();
	}

	void ImageFileReader::ReleaseImageData()
	{
		if (nullptr != fi_bitmap_)
		{
			FreeImage_Unload(fi_bitmap_);
			fi_bitmap_ = nullptr;
			image_data_ = nullptr;
			image_format_ = FIF_UNKNOWN;
			image_width_ = 0;
			image_height_ = 0;
			bits_per_pixel_ = 0;
			image_pitch_ = 0;
		}
	}

	bool ImageFileReader::LoadFromFile(const std::string& file_name)
	{
		this->ReleaseImageData();

		GLuint tex = 0;
		
		// 获取到文件格式
		image_format_ = FreeImage_GetFileType(file_name.c_str(), 0);

		if (FIF_UNKNOWN == image_format_)
		{
			image_format_ = FreeImage_GetFIFFromFilename(file_name.c_str());

			if (FIF_UNKNOWN == image_format_)
				return false;
		}

		if (FreeImage_FIFSupportsReading(image_format_))
			fi_bitmap_ = FreeImage_Load(image_format_, file_name.c_str(), 0);

		image_pitch_ = FreeImage_GetPitch(fi_bitmap_);// 从freeimage bitmap info中取出图片跨度
		image_data_ = FreeImage_GetBits(fi_bitmap_); // 从freeimage bitmap info中取出字节数据
		bits_per_pixel_ = FreeImage_GetBPP(fi_bitmap_);// 从freeimage bitmap info中取出字节数据取出bit per pixel数据
		image_width_ = FreeImage_GetWidth(fi_bitmap_);
		image_height_ = FreeImage_GetHeight(fi_bitmap_);
		return true;
	}

	/*
	GLBitmapSPtr ImageFileReader::GetGLBitmapFromFile(const std::string& file_name, GLuint desired_rgb_mode)
	{
		GLuint tex = 0;
		int i,j,k;
		int w, h;
		int bpp;
		unsigned char* bits = nullptr;
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* fibmp = nullptr;

		// 获取到文件格式
		fif = FreeImage_GetFileType(file_name.c_str(), 0);

		if (FIF_UNKNOWN == fif)
		{
			fif = FreeImage_GetFIFFromFilename(file_name.c_str());

			if (FIF_UNKNOWN == fif)
				return std::make_shared<GLBitmap>(); // 返回一个空指针
		}

		if (FreeImage_FIFSupportsReading(fif))
			fibmp = FreeImage_Load(fif, file_name.c_str(), 0);

		int pitch = FreeImage_GetPitch(fibmp);// 从freeimage bitmap info中取出图片跨度
		bits = FreeImage_GetBits(fibmp); // 从freeimage bitmap info中取出字节数据
		bpp = FreeImage_GetBPP(fibmp);// 从freeimage bitmap info中取出字节数据取出bit per pixel数据
		RGBQUAD *palette = nullptr;

		w = FreeImage_GetWidth(fibmp);
		h = FreeImage_GetHeight(fibmp);

		switch (bpp)
		{
		case 8:
		{
			if (!(palette = FreeImage_GetPalette(fibmp)))
				return std::make_shared<GLBitmap>();

			GLBitmapSPtr gl_bmp = std::make_shared<GLBitmap>(w, h, desired_rgb_mode);
			GLint actual_mode = gl_bmp->RgbMode();

			if (GL_RGB == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						int index = (i*w + j) * 3;
						gl_bmp->WriteRGB(index, palette[i*pitch + j].rgbRed,
							palette[i*pitch + j].rgbGreen,palette[i*pitch + j].rgbBlue);
					}
				}
			}
			else if (GL_RGBA == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						k = bits[i*pitch + j];
						int index = (i*w + j) * 4;
						gl_bmp->WriteRGBA(index, palette[k].rgbRed, palette[k].rgbGreen, palette[k].rgbBlue, 0xFF);
					}
				}
			}

			FreeImage_Unload(fibmp);
			return gl_bmp;
		}
		break;
		case 24:
		{
			GLBitmapSPtr gl_bmp = std::make_shared<GLBitmap>(w, h, desired_rgb_mode);
			GLint actual_mode = gl_bmp->RgbMode();

			if (GL_RGB == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						int index = (i*w + j) * 3;
						gl_bmp->WriteRGB(index, bits[i*pitch + j * 3 + 2],
							bits[i*pitch + j * 3 + 1], bits[i*pitch + j * 3 + 0]);
					}
				}
			}
			else if (GL_RGBA == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						int index = (w + j) * 4;
						gl_bmp->WriteRGBA(index, bits[i*pitch + j * 3 + 2],
							bits[i*pitch + j * 3 + 1], bits[i*pitch + j * 3 + 0], 0xFF);
					}
				}
			}

			FreeImage_Unload(fibmp);
			return gl_bmp;
		}
		break;
		case 32:
		{
			GLBitmapSPtr gl_bmp = std::make_shared<GLBitmap>(w, h, desired_rgb_mode);
			GLint actual_mode = gl_bmp->RgbMode();

			if (GL_RGB == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						int index = (i*w + j) * 3;
						gl_bmp->WriteRGB(index, bits[i*pitch + j * 4 + 2],
							bits[i*pitch + j * 4 + 1], bits[i*pitch + j * 4 + 0]);
					}
				}
			}
			else if (GL_RGBA == actual_mode)
			{
				for (i = 0; i < h; ++i)
				{
					for (j = 0; j < w; ++j)
					{
						int index = (i*w + j) * 4;
						gl_bmp->WriteRGBA(index,
							bits[i*pitch + j * 4 + 2], bits[i*pitch + j * 4 + 1],
							bits[i*pitch + j * 4 + 0], bits[i*pitch + j * 4 + 3]);
					}
				}
			}

			FreeImage_Unload(fibmp);
			return gl_bmp;
		}
		break;
		default:
			FreeImage_Unload(fibmp);
			return  std::make_shared<GLBitmap>();;
		}

		FreeImage_Unload(fibmp);
		return std::make_shared<GLBitmap>();;
	}

	*/
}