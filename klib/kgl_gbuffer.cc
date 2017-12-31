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
#include "kgl_gbuffer.h"
#include "kgl_error.h"

/*
glBindFramebuffer:

https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindFramebuffer.xhtml
binds the framebuffer object with name framebuffer to the framebuffer target specified by target. 
target must be either GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER or GL_FRAMEBUFFER.
If a framebuffer object is bound to GL_DRAW_FRAMEBUFFER or GL_READ_FRAMEBUFFER, 
it becomes the target for rendering or readback operations, respectively, until it is deleted or 
another framebuffer is bound to the corresponding bind point. Calling glBindFramebuffer with target
set to GL_FRAMEBUFFER binds framebuffer to both the read and draw framebuffer targets. framebuffer 
is the name of a framebuffer object previously returned from a call to glGenFramebuffers, 
or zero to break the existing binding of a framebuffer object to target.
*/

namespace kgl
{
	GBuffer::GBuffer()
	{
		fbo_ = 0;
	}

	GBuffer::~GBuffer()
	{
		if (0 != fbo_)
		{
			glDeleteFramebuffers(1, &fbo_);
		}

		if (0 != position_info_texture_)
		{
			glDeleteTextures(1, &position_info_texture_);
			position_info_texture_ = 0;
		}

		if (0 != albedo_info_texture_)
		{
			glDeleteTextures(1, &albedo_info_texture_);
			albedo_info_texture_ = 0;
		}

		if (0 != normal_info_texture_)
		{
			glDeleteTextures(1, &normal_info_texture_);
			normal_info_texture_ = 0;
		}
	}

	void GBuffer::StartGeometryRenderPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GBuffer::EndGeometryRenderPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::StartLightingRenderPass()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, position_info_texture_);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal_info_texture_);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, albedo_info_texture_);
	}

	void GBuffer::Blit()
	{
		// ----------------------------------------------------------------------------------
		// 从Gbuffer的只读frame buffer object中读
		// 写入默认frame buffer object
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_); // 把G-buffer对应的frame bufferr object设置为读
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // 把默认frame buffer设置为写
		// 把颜色数据从G-buffer的frame buffer blit到默认frame buffer
		glBlitFramebuffer(
			0, // 源buffer的左上角x坐标
			0, // 源buffer的左上角y坐标
			window_width_, // 源buffer的右下角x坐标
			window_height_,// 源buffer的右下角y坐标
			0, // 目标buffer的左上角x坐标
			0, // 目标buffer的左上角y坐标
			window_width_, // 目标buffer的右下角x坐标
			window_height_, // 目标buffer的右下角y坐标
			GL_DEPTH_BUFFER_BIT, 
			GL_NEAREST); // 如果源和目标的大小不一致发生了拉伸的时候，指定的缩放的方式，值为GL_NEAREST或GL_LINEAR
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::Initialise(GLuint window_width, GLuint window_height)
	{
		window_height_ = window_height;
		window_width_ = window_width;

		// 创建一个frame buffer object，并且将其绑定为可写入可读取（draw/read）的frame buffer
		// 使用GL_FRAMEBUFFER表示可读写,GL_DRAW_FRAMEBUFFER表示可写，GL_READ_FRAMEBUFFER表示可读
		glGenFramebuffers(1, &fbo_);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

		/*
		如果要使用24位精度的深度缓冲区，不使用模板缓冲区的话，用这些参数组合
		GL_DEPTH_COMPONENT32F // 第3个internalFormat参数
		GL_DEPTH_COMPONENT    // 第7个format参数用这个
		GL_FLOAT              // 第8个type参数

		如果要使用24位精度的深度缓冲区，但可以提供一部分空间给模板缓冲区的话，用这些参数组合
		GL_DEPTH24_STENCIL8   // 第3个internalFormat参数用这个
		GL_DEPTH_STENCIL      // 第7个format参数用这个depth + stencil
		GL_UNSIGNED_INT_24_8  // 第8个type参数，24位的无符号整型数，用作深度缓冲区，低8位用作模板缓冲区

		如果要使用满32位精度的深度缓冲区，并且想使用模板缓冲区的话，用这些参数组合
		GL_DEPTH32F_STENCIL8               // 第3个internalFormat参数用这个
		GL_DEPTH_STENCIL                   // 第7个format参数用这个depth + stencil
		GL_FLOAT_32_UNSIGNED_INT_24_8_REV  // 第8个type参数，32位的float类型用于深度缓冲区，32位的无符号整型数，其中24位闲置，低8位留作模板缓冲区
		// 这就是每个texel有8字节的纹理
		*/
		
		// 存储了片元position信息的G-buffer的color buffer
		glGenTextures(1, &position_info_texture_);
		glBindTexture(GL_TEXTURE_2D, position_info_texture_);
		glTexImage2D(GL_TEXTURE_2D,0, 
					 GL_RGB16F, // 纹理中每个像素有三个分量，每个分量是一个16位的浮点数
					 window_width_, window_height_, 0, 
					 GL_RGB,
					 GL_FLOAT,
					 nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		/*
		第二个参数：GL_COLOR_ATTACHMENT0
		是告诉OpenGL把纹理对像绑定到FBO的0号绑定点（一个FBO在同一个时间内可以绑定多个颜色缓冲区，每个对应FBO的一个绑定点），
		
		第三个参数：GL_TEXTURE_2D是指定纹理的格式，
		第四个参数：指向一个之前就准备好了的texture object纹理可以是多重映射的图像，最后一个参数指定级级为0，指的是使用原图像。*/
		glFramebufferTexture2D(GL_FRAMEBUFFER, 
			GL_COLOR_ATTACHMENT0, 
			GL_TEXTURE_2D, position_info_texture_, 0);

		// 存储了片元normal信息的G-buffer的color buffer
		glGenTextures(1, &normal_info_texture_);
		glBindTexture(GL_TEXTURE_2D, normal_info_texture_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window_width_, window_height_, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_info_texture_, 0);

		// 存储了片元漫反射颜色信息的G-buffer的color buffer
		glGenTextures(1, &albedo_info_texture_);
		glBindTexture(GL_TEXTURE_2D, albedo_info_texture_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width_, window_height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedo_info_texture_, 0);

		GLenum attachments[3] =
		{ 
			GL_COLOR_ATTACHMENT0, 
			GL_COLOR_ATTACHMENT1, 
			GL_COLOR_ATTACHMENT2
		};
		
		// 指定了可以用来写入颜色的离屏缓冲区，这些缓冲区就是上
		// 面已经和frame buffer建立起关联的texture
		glDrawBuffers(3, attachments);

		// 创建一个render buffer用作深度缓冲区
		// render buffer使用native pixel，对于pixel的传输操作是经过优化的
		// 并且因为它是使用了内部格式，所以一个render buffer是不能被shader所
		//  进行sampler操作
		unsigned int depth_rbo;
		glGenRenderbuffers(1, &depth_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window_width_, window_height_);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw Error(L"Framebuffer not complete!", __FILE__, __LINE__);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}