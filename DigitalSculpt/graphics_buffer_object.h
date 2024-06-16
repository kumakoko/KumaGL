#pragma once
// 封装了OpenGL的vertex array object，vertex buffer object ，vertex element buffer 的类
//#ifndef graphics_buffer_object_h__
//#define graphics_buffer_object_h__

#include "GL/glew.h"

namespace DigitalSculpt
{
    class GraphicsBufferObject
    {
    public:
         GraphicsBufferObject();
         ~GraphicsBufferObject();

        void bindVAO(); // bind the VAO to the GPU to call it'd ID and begin rendering it.

        GLuint vao; // vertex array buffer
        GLuint vbo; // vertex buffer
        GLuint ebo; // vertex element buffer
    };
}

//#endif // graphics_buffer_object_h__

