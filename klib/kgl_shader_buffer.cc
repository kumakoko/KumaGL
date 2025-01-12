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
        glBufferData��OpenGL������Ϊ��������������ڴ沢��ʼ�����ݵĺ��������Ǵ����͹��������������ݵ���Ҫ���衣
        
        ��������:
        void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
    
        ����˵��:
            target:
                ָ����������Ŀ�����ͣ����󶨵Ļ��������󣩡�����ֵ��
                    GL_ARRAY_BUFFER�����㻺��������(VBO)��
                    GL_ELEMENT_ARRAY_BUFFER����������������(EBO)��
                    GL_UNIFORM_BUFFER��ͳһ����������(UBO)��
                    GL_SHADER_STORAGE_BUFFER����ɫ���洢����������(SSBO)��
            size:
                ָ�����ݴ洢����Ĵ�С�����ֽ�Ϊ��λ��
            data :
                ָ��Ҫ���Ƶ������������ݵ�ָ�롣������� NULL����ʾֻ�����ڴ������ʼ�����ݡ�
            usage:
                ��ʾ���ݵ�ʹ��ģʽ������ OpenGL �Ż���������
            ����ֵ:
                GL_STATIC_DRAW�����ݲ���Ƶ�����£���Ҫ���ڻ��ơ�
                GL_DYNAMIC_DRAW�����ݻ�Ƶ�����£���Ҫ���ڻ��ơ�
                GL_STREAM_DRAW�����ݽ�ʹ��һ�Σ�֮��ᱻ���¡�
        */
        if (data_size > 0 && nullptr != data_ptr)
        {
            GL_CHECK_SIMPLE(glBufferData(buffer_type_, data_size, data_ptr, usage));
        }
    }

    void ShaderBuffer::BindBufferBase(GLuint slot_index)
    {
        /*glBindBufferBase �� OpenGL �����ڰ󶨻����������ض��󶨵�ĺ�����
        ��Ҫ����ͳһ������ (Uniform Buffer Objects, UBOs) ����ɫ���洢������
        (Shader Storage Buffer Objects, SSBOs)��

        ��������:
        void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);

        ����˵��:
        target: ָ�������������Ŀ�����͡�����ֵ���£�
                GL_UNIFORM_BUFFER: ��ʾ��ͳһ��������
                GL_SHADER_STORAGE_BUFFER: ��ʾ����ɫ���洢��������
        index:  ָ���󶨵�����������ǰ󶨵�ţ���ɫ���еĲ������������ø����������� layout(std140, binding = 0)��
        buffer: ָ��Ҫ�󶨵Ļ�������������ƣ�ID����������� 0����ʾ���ð󶨵��ϵĻ�������*/
        GL_CHECK_SIMPLE(glBindBufferBase(buffer_type_, slot_index, buffer_id_));
    }

    void ShaderBuffer::RebindBufferBase(GLenum new_buffer_type,GLuint slot_index)
    {
        /*glBindBufferBase �� OpenGL �����ڰ󶨻����������ض��󶨵�ĺ�����
        ��Ҫ����ͳһ������ (Uniform Buffer Objects, UBOs) ����ɫ���洢������
        (Shader Storage Buffer Objects, SSBOs)��

        ��������:
        void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);

        ����˵��:
        target: ָ�������������Ŀ�����͡�����ֵ���£�
                GL_UNIFORM_BUFFER: ��ʾ��ͳһ��������
                GL_SHADER_STORAGE_BUFFER: ��ʾ����ɫ���洢��������
        index:  ָ���󶨵�����������ǰ󶨵�ţ���ɫ���еĲ������������ø����������� layout(std140, binding = 0)��
        buffer: ָ��Ҫ�󶨵Ļ�������������ƣ�ID����������� 0����ʾ���ð󶨵��ϵĻ�������*/
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