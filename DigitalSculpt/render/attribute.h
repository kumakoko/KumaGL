#ifndef digital_sculpt_attribute_h__
#define digital_sculpt_attribute_h__

#include "../klib/kgl_lib_pch.h"
#include "render/buffer.h"

namespace DigitalSculpt
{
    class Attribute
    {
    private:
        GLint _location;
        GLint _size;
        GLenum _type;
    public:
        Attribute(GLuint program, const char* name, GLint size, GLenum type);

        inline void unbind()
        {
            glDisableVertexAttribArray(_location);
        }

        void bindToBuffer(Buffer* buffer);
    };
}

#endif // digital_sculpt_attribute_h__