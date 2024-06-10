#include "graphics_buffer_object.h"

namespace DigitalSculpt
{
    GraphicsBufferObject::GraphicsBufferObject()
    {
        vao = 0; // vertex array buffer
        vbo = 0; // vertex buffer
        ebo = 0; // vertex element buffer
    }

    GraphicsBufferObject::~GraphicsBufferObject()
    {
    }

    void GraphicsBufferObject::bindVAO()
    {
        glBindVertexArray(vao);
    }
}