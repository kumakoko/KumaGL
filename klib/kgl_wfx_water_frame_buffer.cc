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
#include "kgl_lib_pch.h"
#include "kgl_wfx_water_frame_buffer.h"

namespace kgl
{
    namespace water_effect
    {

        const int REFLECTION_WIDTH = 900, REFLECTION_HEIGHT = 675;
        const int REFRACTION_WIDTH = 900, REFRACTION_HEIGHT = 675;

        WaterFrameBuffers::WaterFrameBuffers() 
        {
            InitializeReflectionFrameBuffer();
            InitializeRefractionFrameBuffer();
        }

        void WaterFrameBuffers::BindReflectionFrameBuffer() const 
        {
            BindFrameBuffer(reflectionFbo_.get(), REFLECTION_WIDTH, REFLECTION_HEIGHT);
        }

        void WaterFrameBuffers::BindRefractionFrameBuffer() const 
        {
            BindFrameBuffer(refractionFbo_.get(), REFRACTION_WIDTH, REFRACTION_HEIGHT);
        }

        void WaterFrameBuffers::UnbindCurrentFrameBuffer() const
        {
        //    auto dimensions = DisplayManager::getWindow().getSize();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0,1024, 768);
        }

        GLuint WaterFrameBuffers::getReflectionTexture() const 
        {
            return reflectionTexture_.get();
        }

        GLuint WaterFrameBuffers::getRefractionTexture() const 
        {
            return refractionTexture_.get();
        }

        GLuint WaterFrameBuffers::getRefractionDepthTexture() const
        {
            return refractionDepthTexture_.get();
        }

        void WaterFrameBuffers::BindFrameBuffer(GLuint fbo, int width, int height) const
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glViewport(0, 0, width, height);
        }

        void WaterFrameBuffers::InitializeReflectionFrameBuffer() 
        {
            reflectionFbo_ = createFrameBuffer();
            reflectionTexture_ = createColorTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
            reflectionDepthBuffer_ = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
            UnbindCurrentFrameBuffer();
        }

        void WaterFrameBuffers::InitializeRefractionFrameBuffer()
        {
            refractionFbo_ = createFrameBuffer();
            refractionTexture_ = createColorTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
            refractionDepthTexture_ = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
            UnbindCurrentFrameBuffer();
        }

        FramebufferHandle WaterFrameBuffers::createFrameBuffer()
        {
            FramebufferHandle fbo;
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo.get());
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
            return fbo;
        }

        TextureHandle WaterFrameBuffers::createColorTextureAttachment(int width, int height)
        {
            TextureHandle tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex.get());
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.get(), 0);
            return tex;
        }

        TextureHandle WaterFrameBuffers::createDepthTextureAttachment(int width, int height) 
        {
            TextureHandle tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex.get());
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.get(), 0);
            return tex;
        }

        RenderbufferHandle WaterFrameBuffers::createDepthBufferAttachment(int width, int height) 
        {
            RenderbufferHandle buf;
            glGenRenderbuffers(1, &buf);
            glBindRenderbuffer(GL_RENDERBUFFER, buf.get());
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buf.get());
            return buf;
        }
    }
}