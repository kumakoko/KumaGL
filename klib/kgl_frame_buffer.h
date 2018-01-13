/*!
 * \file kgl_frame_buffer.h
 * \date 2017/12/31 13:43
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 封装了帧缓冲区的类
 *
 * TODO: long description
 *
 * \note
*/
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
#ifndef kgl_frame_buffer_h__
#define kgl_frame_buffer_h__

#include "kgl_rendered_texture.h"

namespace kgl
{
    class FrameBuffer
    {
    private:
        enum RenderBufferType
        {
            Color,
            Depth,
            Account // frame buffer object所绑定的render buffer的种类个数
        };
    public:
        /// <summary>
        /// 构造函数，创建一个 <see cref="FrameBuffer"/> 类的对象实例时被调用.
        /// </summary>
        FrameBuffer();

        /// <summary>
        /// 析构函数，删除一个<see cref="FrameBuffer"/>类的对象实例时被调用.
        /// </summary>
        ~FrameBuffer();

        /// <summary>
        /// 创建一个FrameBuffer实例对象
        /// </summary>
        /// <param name="frame_width">The 帧缓存的宽度.</param>
        /// <param name="frame_height">The 帧缓存的高度.</param>
        /// <param name="t">本帧缓存生成的颜色缓冲区的纹素类型</param>
        /// <param name="use_depth">同时对本帧缓存生成深度缓冲区的render buffer object吗</param>
        void Create(int32_t frame_width, int32_t frame_height, RenderedTexelType t = RenderedTexelType::RGBA,bool use_depth = true);
        
        /// <summary>
        /// 返回frame buffer object的ID
        /// </summary>
        /// <returns>返回frame buffer对象ID</returns>
        inline GLuint GetBufferID() const
        {
            return fbo_;
        }

        /// <summary>
        /// 绑定一个rendered texture到本frame buffer中，以便能被写入
        /// </summary>
        /// <param name="rendered_texture">待绑定的rendered texture</param>
        /// <param name="use_depth">同时对本帧缓存生成深度缓冲区的render buffer object吗</param>
        void AttachRenderedTexture(RenderedTextureSPtr rendered_texture, bool use_depth);

        /// <summary>
        /// 释放frame buffer对象
        /// </summary>
        void ReleaseBuffer();

        /// <summary>
        /// Starts the writing.
        /// </summary>
        /// <param name="clear_color">The clear_color.</param>
        /// <param name="clear_mask">The clear_mask.</param>
        void StartWriting(const glm::vec4& clear_color, GLbitfield clear_mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /// <summary>
        /// Ends the writtng.
        /// </summary>
        inline void EndWriting()
        {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        inline RenderedTextureSPtr GetTexture()
        {
            return rendered_texture_;
        }

        static const char* GetFrameBufferStatusInfo(GLenum check_result);
    protected:
        /// <summary>
        /// frame buffer object的ID值，由OpenGL管线创建
        /// </summary>
        GLuint fbo_ = 0;

		/// <summary>
		/// The rbo_depth_
		/// </summary>
		GLuint rbo_depth_ = 0;

		/// <summary>
		/// The rbo_stencil_
		/// </summary>
		GLuint rbo_stencil_ = 0;

        /// <summary>
        /// 
        /// </summary>
        RenderedTextureSPtr rendered_texture_;
    };
}

#endif // kgl_frame_buffer_h__