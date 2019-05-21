/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

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
 * \file kgl_stb_image.h
 * \date 2019/01/30 11:02
 *
 * \author Xiong Xinke
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: 
 *
 * \note 对stb库中的stb_image.h提供的文件做一个包装
*/
#ifndef kgl_stb_image_h__
#define kgl_stb_image_h__

namespace kgl
{
    namespace stb
    {
        class Image 
        {
        private:
            // 只声明不实现，禁止拷贝构造
            Image(const Image& rhs);
            Image& operator = (const Image& rhs);
        public:

            /// <summary>
            /// 类 <see cref="Image"/> 的显式构造函数
            /// </summary>
            /// <param name="filename">图片文件名</param>
            /// <param name="num_components">The num_components.</param>
            explicit Image(const char* filename, int num_components = 0);

            /// <summary>
            /// 类 <see cref="Image"/> 的显式构造函数
            /// </summary>
            /// <param name="filename">The filename.</param>
            /// <param name="num_components">The num_components.</param>
            explicit Image(const std::string& filename, int num_components = 0);

            ~Image();

            inline int width() const
            {
                return width_;
            }

            inline int height() const
            {
                return height_;
            }

            int num_components() const
            {
                return bytes_per_pixel_;
            }

            std::uint32_t get_rgb(int x, int y) const;

            inline const std::uint8_t* data() const 
            { 
                return data_;/*.get()*/;
            }

            inline explicit operator bool() 
            { 
                return data_ != nullptr;
            }

        private:
            /**
            struct deleter 
            {
                void operator()(unsigned char* data) const;
            };*/

            /// <summary>
            /// 图片的宽度
            /// </summary>
            int width_ = 0;

            /// <summary>
            /// 图片的高度
            /// </summary>
            int height_ = 0;

            /// <summary>
            /// 每个像素点的字节数
            /// </summary>
            int bytes_per_pixel_ = 0;

            std::uint8_t* data_ = nullptr;
        };
    }
}


#endif // kgl_stb_image_h__
