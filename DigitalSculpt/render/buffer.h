#ifndef digital_sculpt_buffer_h__
#define digital_sculpt_buffer_h__

#include <memory>
#include <cstdint>
#include <vector>

#include "boost/intrusive_ptr.hpp"
#include "reference_counter.h"
#include "../klib/kgl_lib_pch.h"
#include "misc/utils.h"

namespace DigitalSculpt
{
    class Buffer : public ReferenceCounter
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
        virtual ~Buffer();

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

        /******************************************************************************************************************
         * Desc:
         * Method:    update
         * Returns:   void
         * Parameter: const std::vector<T> & data
         * Parameter: size_t size
         ****************************************************************************************************************/
        template<typename T>
        void update(const std::vector<T>& data, size_t size)
        {
            bind();
            const std::uint8_t* u8ptr = reinterpret_cast<const std::uint8_t*>(data.data());

            if (size > _size)
            {

                glBufferData(_type, size, u8ptr, _hint);
                _size = size;
            }
            else
            {
                glBufferSubData(_type, 0, size, u8ptr);
            }
        }
    private:
        GLuint _buffer = 0; // Buffer ID
        GLenum _type; // Buffer type (GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, etc.)
        GLenum _hint; // Hint for buffer usage (GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc.)
        size_t _size; // Size of the buffer in bytes
    };

    typedef boost::intrusive_ptr<Buffer> BufferSPtr;
}

#endif // digital_sculpt_buffer_h__