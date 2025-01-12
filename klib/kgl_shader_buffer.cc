#include "kgl_lib_pch.h"
#include "kgl_defines.h"
#include "kgl_shader_buffer.h"

namespace kgl
{
    ShaderBuffer::ShaderBuffer(GLenum buffer_type) :buffer_id_(0), buffer_type_(buffer_type)
    {
        GL_CHECK_SIMPLE(glGenBuffers(1, &buffer_id_));
    }

    ShaderBuffer::~ShaderBuffer()
    {
        Destroy();
    }

    void ShaderBuffer::BindBufferAndUploadData(std::size_t data_size, const void* data_ptr, GLenum usage)
    {
        GL_CHECK_SIMPLE(glBindBuffer(buffer_type_, buffer_id_));

        /*
        glBufferData是OpenGL中用于为缓冲区对象分配内存并初始化数据的函数。它是创建和管理缓冲区对象数据的重要步骤。
        
        函数定义:
        void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
    
        参数说明:
            target:
                指定缓冲区的目标类型（即绑定的缓冲区对象）。常用值：
                    GL_ARRAY_BUFFER：顶点缓冲区对象(VBO)。
                    GL_ELEMENT_ARRAY_BUFFER：索引缓冲区对象(EBO)。
                    GL_UNIFORM_BUFFER：统一缓冲区对象(UBO)。
                    GL_SHADER_STORAGE_BUFFER：着色器存储缓冲区对象(SSBO)。
            size:
                指定数据存储所需的大小，以字节为单位。
            data :
                指向要复制到缓冲区的数据的指针。如果传递 NULL，表示只分配内存而不初始化数据。
            usage:
                提示数据的使用模式，帮助 OpenGL 优化缓冲区。
            常用值:
                GL_STATIC_DRAW：数据不会频繁更新，主要用于绘制。
                GL_DYNAMIC_DRAW：数据会频繁更新，主要用于绘制。
                GL_STREAM_DRAW：数据仅使用一次，之后会被更新。
        */
        if (data_size > 0 && nullptr != data_ptr)
        {
            GL_CHECK_SIMPLE(glBufferData(buffer_type_, data_size, data_ptr, usage));
        }
    }

    void ShaderBuffer::BindBufferBase(GLuint slot_index)
    {
        /*glBindBufferBase 是 OpenGL 中用于绑定缓冲区对象到特定绑定点的函数，
        主要用于统一缓冲区 (Uniform Buffer Objects, UBOs) 或着色器存储缓冲区
        (Shader Storage Buffer Objects, SSBOs)。

        函数定义:
        void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);

        参数说明:
        target: 指定缓冲区对象的目标类型。常用值如下：
                GL_UNIFORM_BUFFER: 表示绑定统一缓冲区。
                GL_SHADER_STORAGE_BUFFER: 表示绑定着色器存储缓冲区。
        index:  指定绑定点的索引。这是绑定点号，着色器中的布局声明将引用该索引，例如 layout(std140, binding = 0)。
        buffer: 指定要绑定的缓冲区对象的名称（ID）。如果传递 0，表示解绑该绑定点上的缓冲区。*/
        GL_CHECK_SIMPLE(glBindBufferBase(buffer_type_, slot_index, buffer_id_));
    }

    void ShaderBuffer::RebindBufferBase(GLenum new_buffer_type,GLuint slot_index)
    {
        /*glBindBufferBase 是 OpenGL 中用于绑定缓冲区对象到特定绑定点的函数，
        主要用于统一缓冲区 (Uniform Buffer Objects, UBOs) 或着色器存储缓冲区
        (Shader Storage Buffer Objects, SSBOs)。

        函数定义:
        void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);

        参数说明:
        target: 指定缓冲区对象的目标类型。常用值如下：
                GL_UNIFORM_BUFFER: 表示绑定统一缓冲区。
                GL_SHADER_STORAGE_BUFFER: 表示绑定着色器存储缓冲区。
        index:  指定绑定点的索引。这是绑定点号，着色器中的布局声明将引用该索引，例如 layout(std140, binding = 0)。
        buffer: 指定要绑定的缓冲区对象的名称（ID）。如果传递 0，表示解绑该绑定点上的缓冲区。*/
        buffer_type_ = new_buffer_type;
        GL_CHECK_SIMPLE(glBindBufferBase(buffer_type_, slot_index, buffer_id_));
    }

    void ShaderBuffer::Destroy()
    {
        if (0 != buffer_id_)
        {
            GL_CHECK_SIMPLE(glDeleteBuffers(1, &buffer_id_));
            buffer_id_ = 0;
        }
    }

    void ShaderBuffer::Rebinding(GLenum new_bind_type)
    {
        GL_CHECK_SIMPLE(glBindBuffer(new_bind_type, buffer_id_));
        buffer_type_ = new_bind_type;
    }
}