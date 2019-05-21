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

/*
actually, FreeImage has a fix color order, optionally specified by the FREEIMAGE_COLORORDER option at compile time. 
This option may be set to either FREEIMAGE_COLORORDER_BGR (0) or FREEIMAGE_COLORORDER_RGB (1). If not specified at compile time,
the actual color order setting is determined from the target system's endianess, which in turn is determined from your compiler's
built-in BYTE_ORDER macro.

So, by default, you'll have RGB color order on big-endian systems and BGR on little-endian systems.
Function FreeImage_ConvertToRawBits does not change color order during conversion, expect if the target
raw image is requested to be a 16-bit image (by setting parameter bpp to 16). Since FreeImage supports two 
16-bit image types (555 and 565 types), as with FreeImage_Allocate, the red, green and blue masks are not much
more than a 555 or 565 sub-type specifier for a 16-bit image.

If you actually need RGBA color ordering for all your images, making a special build with FREEIMAGE_COLORORDER set to 
FREEIMAGE_COLORORDER_RGB will make sense (personally, I do the same with one of my projects). However, for FreeImage it 
would be great to have some sort of color-reordering functions as well.

One function could actually reorder RGB channels inplace. Another good idea would be to modify FreeImage_ConvertToRawBits 
(and also FreeImage_ConvertFromRawBits[Ex] with parameter copySource set to TRUE only) so that it will honor the values passed 
to red_mask, green_mask and blue_mask.
RFC, has anyone some more comments on this?


Hi Carsten,
FreeImage already has an internal utility called 'SwapRedBlue32'.
We could export this function and extend it to any kind of RGB[A] format (thus providing RGB[A] / BGR[A] conversion support).
=> unless adding an internal flag to remember the internal color model, it would be up to the user to do the in-place conversion after loading or before saving.
*/

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

    std::array<unsigned char,4> ImageFileReader::GetPixel(int x, int y) const
    {
		int offset = 0;
		int copy_size = 0;
        // int bytes_per_pixel = 0; // 每个像素的字节数
        const unsigned char* pixel = this->GetImageData();
        std::array<unsigned char, 4> color;
        memset(color.data(), 0, sizeof(unsigned char) * 4);

        switch (bits_per_pixel_)
        {// 以A,R,G,B的格式存放
        case 32:
			copy_size = sizeof(unsigned char) * 4;
			offset = copy_size * (this->ImageWidth() * y + x);
			memcpy(color.data() + 0, pixel + offset, copy_size);
			break;
		case 24:
			copy_size = sizeof(unsigned char) * 3;
			offset = copy_size * (this->ImageWidth() * y + x);
			memcpy(color.data() + 1, pixel + offset, copy_size);
			break;
			//memcpy(color.data() + 1, pixel, sizeof(unsigned char) * 3 * (ImageWidth() * y + x)); break;
		case 16:
			copy_size = sizeof(unsigned char) * 2;
			offset = copy_size * (this->ImageWidth() * y + x);
			memcpy(color.data() + 2, pixel + offset, copy_size);
			break;
			//memcpy(color.data() + 2, pixel, sizeof(unsigned char) * 2 * (ImageWidth() * y + x)); break;
		case 8: 
			copy_size = sizeof(unsigned char) * 1;
			offset = copy_size * (this->ImageWidth() * y + x);
			memcpy(color.data() + 3, pixel + offset, copy_size);
			break;
			//memcpy(color.data() + 3, pixel, sizeof(unsigned char) * 1 * (ImageWidth() * y + x)); break;
        }

        return color;
    }
}