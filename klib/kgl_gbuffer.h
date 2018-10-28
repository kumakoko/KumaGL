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
* \file kgl_gbuffer.h
* \date 2017/12/27 11:26
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef kgl_gbuffer_h__
#define kgl_gbuffer_h__

namespace kgl
{
    class GBuffer
    {
    public:
        /// <summary>
        /// <see cref="GBuffer"/> 类的构造函数
        /// </summary>
        GBuffer();

        /// <summary>
        /// Finalizes an instance of the <see cref="GBuffer"/> 类的析构函数
        /// </summary>
        ~GBuffer();
        
        /// <summary>
        /// 根据给定的窗口高宽，初始化G-buffer对象及其对应的缓冲区
        /// </summary>
        /// <param name="window_width">窗口宽度</param>
        /// <param name="window_height">窗口高度</param>
        /// <returns>创建成功返回true，失败返回false</returns>
        void Initialise(GLuint window_width, GLuint window_height);
        
        /// <summary>
        /// Starts the geometry render pass.
        /// </summary>
        void StartGeometryRenderPass();

        /// <summary>
        /// Starts the lighting render pass.
        /// </summary>
        void StartLightingRenderPass();

        /// <summary>
        /// Blits this instance.
        /// </summary>
        void Blit();

        /// <summary>
        /// Ends the geometry render pass.
        /// </summary>
        void EndGeometryRenderPass();
    private:
        /// <summary>
        /// frame buffer object id
        /// </summary>
        GLuint fbo_;

        /// <summary>
        /// The position_info_texture_
        /// </summary>
        GLuint position_info_texture_ = 0;

        /// <summary>
        /// The normal_info_texture_
        /// </summary>
        GLuint normal_info_texture_ = 0;

        /// <summary>
        /// The albedo_info_texture_
        /// </summary>
        GLuint albedo_info_texture_ = 0;

        /// <summary>
        /// The window_width_
        /// </summary>
        GLuint window_width_ = 0;

        /// <summary>
        /// The window_height_
        /// </summary>
        GLuint window_height_ = 0;      
    };
}

#endif // kgl_gbuffer_h__
