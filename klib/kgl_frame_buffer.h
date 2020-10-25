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
* \file kgl_frame_buffer.h
* \date 2017/12/31 13:43
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief 封装了帧缓冲区的类
*/
#ifndef kgl_frame_buffer_h__
#define kgl_frame_buffer_h__

#include "kgl_rendered_texture.h"
#include "kgl_rectangle.h"

namespace kgl
{
    class FrameBuffer
    {
    public:
        /*********************************************************
        构造函数       
        *********************************************************/
        FrameBuffer();

        /*********************************************************
        析构函数     
        *********************************************************/
        ~FrameBuffer();

        /*********************************************************
        创建一个FrameBuffer实例对象
        @param  int32_t frame_width 帧缓存的宽度
        @param  int32_t frame_height 帧缓存的高度
        @param  RenderedTexelType t 本帧缓存生成的颜色缓冲区的纹素类型
        @param  bool use_depth 同时对本帧缓存生成深度缓冲区的render buffer object吗
        @param  const TextureParams * texture_params 创建纹理时的各个参数      
        *********************************************************/
        void Create(int32_t frame_width, int32_t frame_height, RenderedTexelType t = RenderedTexelType::RGBA, bool use_depth = true, const TextureParams* texture_params = nullptr);
        
        /*********************************************************
        返回frame buffer object的ID
        @return  返回frame buffer对象ID       
        *********************************************************/
        inline GLuint GetBufferID() const
        {
            return fbo_;
        }

        /*********************************************************
        绑定一个rendered texture到本frame buffer中，以便能被写入
        @param  RenderedTextureSPtr rendered_texture 待绑定的rendered texture
        @param  bool use_depth 同时对本帧缓存生成深度缓冲区的render buffer object吗       
        *********************************************************/
        void AttachRenderedTexture(RenderedTextureSPtr rendered_texture, bool use_depth);

        /*********************************************************
        释放frame buffer对象       
        *********************************************************/
        void ReleaseBuffer();

        /*********************************************************
        开始往本frame buffer中写入
        @param  const glm::vec4 & clear_color 整体清空frame buffer所用到的颜色
        @param  GLbitfield clear_mask 颜色掩码
        @param  const IRect * rect 要清空的视口的大小       
        *********************************************************/
        void StartWriting(const glm::vec4& clear_color, GLbitfield clear_mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, const IRect* rect = nullptr);
        
        /*********************************************************
        结束往本frame buffer中写入
        @param  const IRect * rect 要清空的视口的大小
        *********************************************************/
        void EndWriting(const IRect* rect = nullptr);

        inline RenderedTextureSPtr GetTexture()
        {
            return rendered_texture_;
        }

        static const char* GetFrameBufferStatusInfo(GLenum check_result);
    protected:
		/** * @brief frame buffer object的ID值，由OpenGL管线创建 */
        GLuint fbo_ = 0;

		/** * @brief  */
        GLuint rbo_depth_ = 0;

		/** * @brief */
        GLuint rbo_stencil_ = 0;

		/** * @brief */
        RenderedTextureSPtr rendered_texture_;
    };
}

#endif // kgl_frame_buffer_h__