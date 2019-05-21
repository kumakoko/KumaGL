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
 * \file kgl_water_frame_buffer.h
 * \date 2019/01/26 18:53
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef kgl_water_frame_buffer_h__
#define kgl_water_frame_buffer_h__

#include "kgl_wfx_resource_handle.h"

namespace kgl
{
    namespace water_effect
    {
        class WaterFrameBuffers 
        {
        public:
            WaterFrameBuffers();

            /// <summary>
            /// Binds the reflection frame buffer.
            /// </summary>
            void BindReflectionFrameBuffer() const;

            /// <summary>
            /// Binds the refraction frame buffer.
            /// </summary>
            void BindRefractionFrameBuffer() const;

            /// <summary>
            /// Unbinds the current frame buffer.
            /// </summary>
            void UnbindCurrentFrameBuffer() const;

            GLuint getReflectionTexture() const;
            GLuint getRefractionTexture() const;
            GLuint getRefractionDepthTexture() const;

        private:
            /// <summary>
            /// Initializes the reflection frame buffer.
            /// </summary>
            void InitializeReflectionFrameBuffer();

            /// <summary>
            /// Initializes the refraction frame buffer.
            /// </summary>
            void InitializeRefractionFrameBuffer();

            /// <summary>
            /// Binds the frame buffer.
            /// </summary>
            /// <param name="fbo">The fbo.</param>
            /// <param name="width">The width.</param>
            /// <param name="height">The height.</param>
            void BindFrameBuffer(GLuint fbo, int width, int height) const;

            FramebufferHandle  createFrameBuffer();
            TextureHandle      createColorTextureAttachment(int width, int height);
            TextureHandle      createDepthTextureAttachment(int width, int height);
            RenderbufferHandle createDepthBufferAttachment(int width, int height);

            FramebufferHandle  reflectionFbo_;
            TextureHandle      reflectionTexture_;
            RenderbufferHandle reflectionDepthBuffer_;

            FramebufferHandle  refractionFbo_;
            TextureHandle      refractionTexture_;
            TextureHandle      refractionDepthTexture_;
        };

        typedef std::shared_ptr<WaterFrameBuffers> WaterFrameBuffersSPtr;
    }
}

#endif // kgl_water_frame_buffer_h__