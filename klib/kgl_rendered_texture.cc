// 可渲染的纹理
#include "kgl_lib_pch.h"
#include "kgl_rendered_texture.h"
#include "kgl_error.h"

namespace kgl
{
    RenderedTexture::RenderedTexture(RenderedTextureUsedType used_type)
    {
        type_ = RENDERED_TEXTURE;
        used_type_ = used_type;
    }

    RenderedTexture::~RenderedTexture()
    {
        if (fbo_ != 0)
            glDeleteFramebuffers(1, &fbo_);
    }
    
    void RenderedTexture::CreateFromFile(const std::string& file_name, const TextureParams& texture_params)
    {

    }

    
    void RenderedTexture::Create(int width, int height, const TextureParams& texture_params)
    {

    }

    int RenderedTexture::GetWidth() const
    {
        return Texture::GetWidth();
    }

    int RenderedTexture::GetHeight() const
    {
        return Texture::GetHeight();
    }

    
    bool RenderedTexture::IsMipmap() const
    {
        return false;
    }

    
    void RenderedTexture::ActiveBind(GLuint slot_index)
    {
        // Texture::ActiveBind(slot_index);

		GLuint i = glm::clamp(slot_index, static_cast<GLuint>(0), static_cast<GLuint>(8));
		GLenum texture_unit = GL_TEXTURE0 + i;
		glActiveTexture(texture_unit);
		glBindTexture(GL_TEXTURE_2D, texture_id_);
    }

    void RenderedTexture::Create(int32_t texture_width, int32_t texture_height)
    {
        // https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml

		/*
		// create a color attachment texture
		unsigned int textureColorbuffer;
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		*/


        glGenFramebuffers(1, &fbo_);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

        GLenum attachment_type;
        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture_id_);

        if (RenderedTextureUsedType::COLOR_RGB_ONLY == used_type_)
        {
            attachment_type = GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0,
                GL_RGB, // 使用GL_RGB，表明指定texture中的颜色分量为3个
                texture_width, texture_height, 0,
                GL_RGB,//指定纹理像素格式为RGB
                GL_UNSIGNED_BYTE, // 指明每个颜色分量使用一个字节
                NULL);
        }
        else if (RenderedTextureUsedType::COLOR_RGBA_ONLY == used_type_)
        {
            attachment_type = GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0,
                GL_RGBA, // 使用GL_RGB，表明指定texture中的颜色分量为4个
                texture_width, texture_height, 0,
                GL_RGBA,  // 指定纹理像素格式为RGBA
                GL_UNSIGNED_BYTE,  // 指明每个颜色分量使用一个字节
                NULL);
        }
        else
        {
            if (RenderedTextureUsedType::DEPTH_ONLY == used_type_)
                attachment_type = GL_DEPTH_COMPONENT;
            else if (RenderedTextureUsedType::STENCIL_ONLY == used_type_)
                attachment_type = GL_STENCIL_INDEX;
            else if (RenderedTextureUsedType::WITH_DEPTH_STENCIL == used_type_)
                attachment_type = GL_DEPTH24_STENCIL8;

            // 这时的纹理用作渲染模板值,OpenGL直到4.4才支持设置单独的模板值，所以到目前为止，深度和模板一起用
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,
				texture_width, texture_height, 0, 
				GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);


			/*
			// create a color attachment texture
			unsigned int textureColorbuffer;
			glGenTextures(1, &textureColorbuffer);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
			*/
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //      glBindTexture(GL_TEXTURE_2D, 0);

        // 把创建出来的texture id绑定到frame buffer上
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id_, 0);

        // 如果有使用深度|模板缓冲区的话
        if (RenderedTextureUsedType::DEPTH_ONLY == used_type_ ||
            RenderedTextureUsedType::STENCIL_ONLY == used_type_ ||
            RenderedTextureUsedType::WITH_DEPTH_STENCIL == used_type_)
        {

            glGenRenderbuffers(1, &rbo_);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, texture_width, texture_height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_); // 绑定到帧缓冲区对象

			/*
			// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
			// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			*/
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw Error(L"ERROR::FRAMEBUFFER:: Framebuffer is not complete!", __FILE__, __LINE__);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}