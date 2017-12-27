#include "kgl_lib_pch.h"
#include "kgl_gbuffer.h"


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
		m_depthTexture = 0;
		m_finalTexture = 0;
		memset(texture_buffers_, 0, sizeof(GLuint)*GBUFFER_NUM_TEXTURES);
	}

	GBuffer::~GBuffer()
	{
		if (0 != fbo_)
		{
			glDeleteFramebuffers(1, &fbo_);
		}

		if ( 0 != texture_buffers_[0])
		{
			glDeleteTextures(GBUFFER_NUM_TEXTURES, texture_buffers_);
		}

		if ( 0 != m_depthTexture )
		{
			glDeleteTextures(1, &m_depthTexture);
		}

		if ( 0 != m_finalTexture )
		{
			glDeleteTextures(1, &m_finalTexture);
		}
	}

	bool GBuffer::Initialise(GLuint window_width, GLuint window_height)
	{
		//  创建一个frame buffer object，并且将其绑定为可写入（draw）的frame buffer
		glGenFramebuffers(1, &fbo_);

		// 把fbo_绑定为可写
		// 如果绑定为GL_FRAMEBUFFER，表示同时绑定为可写可读
		// 即GL_DRAW_FRAMEBUFFER | GL_READ_FRAMEBUFFER
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);

		// 创建G-buffer用的纹理,生成三个texture，将其texture
		// id一次存储进buffer_textures_数组里面
		glGenTextures(GBUFFER_NUM_TEXTURES, texture_buffers_);

		// 创建存储深度值的纹理
		glGenTextures(1, &m_depthTexture);

		// 创建一个用于最终输出的纹理
		glGenTextures(1, &m_finalTexture);

		for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) 
		{
			glBindTexture(GL_TEXTURE_2D, texture_buffers_[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, 
				GL_RGB32F, // 表示创建出来的颜色分量，每个分量都是一个32位float类型，因为要存储的是诸如position
							// normal等原本就是32位float精度的数据，所以这里用GL_RGB32F
				window_width, window_height, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			// 把新创建出来的texture，绑定到m_fbo指定的纹理里面，绑定的slot是从GL_COLOR_ATTACHMENT0开始
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture_buffers_[i], 0);
		}

		// 创建一个深度缓冲纹理，大小为窗口的大小，纹理格式为32位的浮点纹理，模板缓冲区8位的深度
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
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_DEPTH32F_STENCIL8, // 使用32位精度的深度缓冲区，并且还用模板缓冲区
			window_width, 
			window_height,
			0,
			GL_DEPTH_STENCIL, // 有深度缓冲区，有模板缓冲区
			GL_FLOAT_32_UNSIGNED_INT_24_8_REV, // 32位的float类型用于深度缓冲区，32位的无符号整型数，其中24位闲置，低8位留作模板缓冲区
			nullptr);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

		// depth
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_DEPTH32F_STENCIL8,
			window_width, 
			window_height, 
			0, 
			GL_DEPTH_STENCIL,
			GL_FLOAT_32_UNSIGNED_INT_24_8_REV,
			nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

		// final
		glBindTexture(GL_TEXTURE_2D, m_finalTexture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0, 
			GL_RGBA,
			window_width,
			window_height, 
			0,
			GL_RGB, 
			GL_FLOAT,
			NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_finalTexture, 0);

		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("FB error, status: 0x%x\n", Status);
			return false;
		}

		// 完成，解除绑定
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		return true;
	}

	void GBuffer::StartFrame()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GBuffer::BindForGeomPass()
	{
		const int DRAW_BUFFER_COUNT = 3;

		GLenum DrawBuffers[DRAW_BUFFER_COUNT] =
		{ 
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2 
		};

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
		glDrawBuffers(DRAW_BUFFER_COUNT, DrawBuffers);
	}

	void GBuffer::BindForStencilPass()
	{
		// must disable the draw buffers 
		glDrawBuffer(GL_NONE);
	}

	void GBuffer::BindForLightPass()
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT4);

		for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texture_buffers_[GBUFFER_TEXTURE_TYPE_POSITION + i]);
		}
	}

	void GBuffer::BindForFinalPass()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_);
		glReadBuffer(GL_COLOR_ATTACHMENT4);
	}
}