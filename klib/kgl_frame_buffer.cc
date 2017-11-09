// 封装了帧缓冲区的类
#include "kgl_lib_pch.h"
#include "kgl_frame_buffer.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"

// http://blog.csdn.net/dreamcs/article/details/7691690

namespace kgl
{
    FrameBuffer::FrameBuffer()
    {
    }

    FrameBuffer::~FrameBuffer()
    {
        this->ReleaseBuffer();
        rendered_texture_.reset();
    }

    void FrameBuffer::Create(int32_t frame_width, int32_t frame_height, RenderedTexelType t,bool use_depth)
    {
        glGenFramebuffers(1, &fbo_);
        RenderedTextureSPtr rendered_texture =std::make_shared<RenderedTexture>(t);
        rendered_texture->Create(frame_width, frame_height,TextureParams());
        this->AttachRenderedTexture(rendered_texture,use_depth);
    }

    void FrameBuffer::ReleaseBuffer()
    {
        if (0 != fbo_)
            glDeleteFramebuffers(1, &fbo_);

        if (0 != rbo_depth_)
            glDeleteRenderbuffers(1, &rbo_depth_);

        if (0 != rbo_stencil_)
            glDeleteRenderbuffers(1, &rbo_stencil_);
    }

    void FrameBuffer::StartWriting(const glm::vec4& clear_color, GLbitfield clear_mask)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClear(clear_mask);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void FrameBuffer::AttachRenderedTexture(RenderedTextureSPtr rendered_texture,bool use_depth)
    {
        rendered_texture_ = rendered_texture;
        const wchar_t* err_msg = nullptr;

        if (0 == fbo_)
        {
            err_msg = L"You should create frame buffer object before attaching rendered texture to it";
            throw Error(err_msg, __FILE__, __LINE__);
            return;
        }

        // 要绑定rendered texture到本frame buffer前，首先要... 
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        
        glBindTexture(GL_TEXTURE_2D, rendered_texture->GetTextureID());

        // 把创建出来的texture id绑定到frame buffer上
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               rendered_texture_->GetTextureID(),
                               0);

        glBindTexture(GL_TEXTURE_2D, 0);

        // 如果有使用深度|模板缓冲区的话
        if ( use_depth )
        {
            glGenRenderbuffers(1, &rbo_depth_);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_);
            glRenderbufferStorage(GL_RENDERBUFFER,
                GL_DEPTH_COMPONENT,
                rendered_texture_->GetWidth(), 
                rendered_texture_->GetHeight());
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            // 绑定到帧缓冲区对象
            glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                      GL_DEPTH_ATTACHMENT, 
                                      GL_RENDERBUFFER, 
                                      rbo_depth_);
        }

        // 这句代码必须放在检查frame buffer完整性之前，
        // 否则就会得到frame buffer不完整的信息
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        GLenum check_resut = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        
        if (GL_FRAMEBUFFER_COMPLETE != check_resut)
        {
            throw Error(
                kgl::StringConvertor::ANSItoUTF16LE(GetFrameBufferStatusInfo(check_resut)),
                __FILE__, __LINE__);
        }
    }

    const char* FrameBuffer::GetFrameBufferStatusInfo(GLenum check_result)
    {
        switch (check_result)
        {
        case GL_FRAMEBUFFER_COMPLETE:return "frame buffer和它的attachment完全符合渲染或者数据读取的需求";
        case GL_FRAMEBUFFER_UNDEFINED:return "GL_FRAMEBUFFER_UNDEFINED";
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
        case GL_FRAMEBUFFER_UNSUPPORTED:return "GL_FRAMEBUFFER_UNSUPPORTED";
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
        default:return "Unknown frame buffer check status";
        }
    }
}