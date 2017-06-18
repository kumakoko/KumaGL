// 封装了帧缓冲区的类
#include "kgl_lib_pch.h"
#include "kgl_frame_buffer.h"

namespace kgl
{
    FrameBuffer::FrameBuffer() :fbo_(0)
    {
    }

    FrameBuffer::~FrameBuffer()
    {
        this->ReleaseBuffer();
    }

    void FrameBuffer::Create()
    {
        GLuint fbo_;
        glGenFramebuffers(1, &fbo_);
        
    }

    void FrameBuffer::ReleaseBuffer()
    {
        if (fbo_ != 0)
        {
            glDeleteFramebuffers(1, &fbo_);
        }
    }
}