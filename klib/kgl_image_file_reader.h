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
/*!
* \file kgl_image_file_reader.h
* \date 2017/12/31 13:44
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: 图像读取器
*
* \note
*/
#ifndef kgl_image_file_reader_h__
#define kgl_image_file_reader_h__

namespace kgl
{
    class ImageFileReader
    {
    public:
        ImageFileReader();
        ~ImageFileReader();
        
        /// <summary>
        /// Loads from file.
        /// </summary>
        /// <param name="file_name">The file_name.</param>
        /// <returns>bool.</returns>
        bool LoadFromFile(const std::string& file_name);

        /// <summary>
        /// Gets the image data.
        /// </summary>
        /// <returns>const unsigned char *.</returns>
        const unsigned char* GetImageData() const
        {
            return image_data_;
        }
        
        /// <summary>
        /// Images the format.
        /// </summary>
        /// <returns>FREE_IMAGE_FORMAT.</returns>
        inline FREE_IMAGE_FORMAT ImageFormat() const
        {
            return image_format_;
        }

        /// <summary>
        /// Images the data.
        /// </summary>
        /// <returns>unsigned char *.</returns>
        inline unsigned char* ImageData() const
        {
            return image_data_;
        }

        /// <summary>
        /// Images the width.
        /// </summary>
        /// <returns>int.</returns>
        inline int ImageWidth() const
        {
            return image_width_;
        }

        /// <summary>
        /// Images the height.
        /// </summary>
        /// <returns>int.</returns>
        inline int ImageHeight() const
        {
            return image_height_;
        }

        /// <summary>
        /// 每个像素占据的字节数
        /// </summary>
        /// <returns>int.</returns>
        inline int BPP() const
        {
            return bits_per_pixel_;
        }

		std::vector<unsigned char> GetPixel(int x, int y) const;

        /// <summary>
        /// Images the pitch.
        /// </summary>
        /// <returns>int.</returns>
        inline int ImagePitch() const
        {
            return image_pitch_;
        }

        /// <summary>
        /// Releases this instance.
        /// </summary>
        void ReleaseImageData();

    protected:
        /// <summary>
        /// The image_format_
        /// </summary>
        FREE_IMAGE_FORMAT image_format_ = FIF_UNKNOWN;

        /// <summary>
        /// The image_data_
        /// </summary>
        unsigned char* image_data_ = nullptr;

        /// <summary>
        /// The image_width_
        /// </summary>
        int image_width_ = 0;

        /// <summary>
        /// The image_height_
        /// </summary>
        int image_height_ = 0;

        /// <summary>
        /// The bits_per_pixel_
        /// </summary>
        int bits_per_pixel_ = 0;

        /// <summary>
        /// The image_pitch_
        /// </summary>
        int image_pitch_ = 0;

        /// <summary>
        /// The fi_bitmap_
        /// </summary>
        FIBITMAP* fi_bitmap_ = nullptr;
    };
}

#endif // kgl_image_file_reader_h__