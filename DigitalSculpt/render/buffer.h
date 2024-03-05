#include "../klib/kgl_lib_pch.h"
#include <memory>

namespace DigitalSculpt
{
    class Buffer
    {
    public:
        /******************************************************************************************************************
         * Desc: 
         * Method:    Buffer
         * Returns:   
         * Parameter: GLenum type
         * Parameter: GLenum hint
         ****************************************************************************************************************/
         Buffer(GLenum type, GLenum hint);

        /******************************************************************************************************************
         * Desc: 
         * Method:    ~Buffer
         * Returns:   
         ****************************************************************************************************************/
         ~Buffer();

        /******************************************************************************************************************
         * Desc: 
         * Method:    bind
         * Returns:   void
         ****************************************************************************************************************/
         void bind();

        /******************************************************************************************************************
         * Desc: 
         * Method:    release
         * Returns:   void
         ****************************************************************************************************************/
         void release();

        /******************************************************************************************************************
         * Desc: 
         * Method:    update
         * Returns:   void
         * Parameter: const void * data
         * Parameter: size_t size
         ****************************************************************************************************************/
         void update(const void* data, size_t size);
    private:
        GLuint _buffer = 0; // Buffer ID
        GLenum _type; // Buffer type (GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, etc.)
        GLenum _hint; // Hint for buffer usage (GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc.)
        size_t _size; // Size of the buffer in bytes
    };

    typedef std::shared_ptr<Buffer> BufferSPtr;
}