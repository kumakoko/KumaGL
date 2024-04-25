#include "misc/utils.h"
#include "render/buffer.h"

namespace DigitalSculpt
{
    Buffer::Buffer(GLenum type, GLenum hint) : _type(type), _hint(hint), _size(0)
    {
        glGenBuffers(1, &_buffer); // Generate a buffer
    }

    Buffer::~Buffer()
    {
        release(); // Ensure we clean up the buffer
    }

    void Buffer::bind()
    {
        glBindBuffer(_type, _buffer); // Bind the buffer for use
    }

    void Buffer::release()
    {
        if (_buffer != 0)
        {
            glDeleteBuffers(1, &_buffer); // Delete the buffer
            _buffer = 0;
            _size = 0;
        }
    }

    void Buffer::update(const void* data, size_t size)
    {
        bind(); // Ensure the buffer is bound

        if (size > _size) {
            // If the new data is larger than the current buffer size, reallocate with glBufferData
            glBufferData(_type, size, data, _hint);
            _size = size;
        }
        else {
            // Otherwise, update the existing buffer content
            glBufferSubData(_type, 0, size, data);
        }
    }
}