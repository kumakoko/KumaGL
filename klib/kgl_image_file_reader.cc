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
}