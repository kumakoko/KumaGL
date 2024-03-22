#include "render/attribute.h"

namespace DigitalSculpt
{
    Attribute::Attribute(GLuint program, const char* name, GLint size, GLenum type)
    {
        _location = glGetAttribLocation(program, name); // the location
        _size = size; // numbe of components
        _type = type; // type of the components
    }

    void Attribute::bindToBuffer(Buffer* buffer) 
    {
        buffer->bind();
        glEnableVertexAttribArray(_location);
        glVertexAttribPointer(_location, _size, _type, false, 0, 0);
    }
}