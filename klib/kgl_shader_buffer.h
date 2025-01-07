#ifndef kgl_shader_buffer_h__
#define kgl_shader_buffer_h__

namespace kgl
{
    class ShaderBuffer
    {
    public:
        ShaderBuffer(GLenum buffer_type);
        ~ShaderBuffer();

        /*********************************************************
        当data_size为0 ，或者data_ptr为nullptr时，仅执行buffer绑定
        并不upload函数
        @param  std::size_t data_size
        @param  const void * data_ptr
        @param  GLenum usage
        @return void    
        *********************************************************/
        void BindBufferAndUploadData(std::size_t data_size, const void* data_ptr, GLenum usage = GL_STATIC_DRAW);

        void BindBufferBase(GLuint slot_index);

        void Destroy();

        inline GLuint BufferID() const
        {
            return buffer_id_;
        }

        inline GLenum BufferType() const
        {
            return buffer_type_;
        }

        void Rebinding(GLenum new_bind_type);

    private:
        GLuint buffer_id_;
        GLenum buffer_type_;
    };


}

#endif // kgl_shader_buffer_h__
