#ifndef open_gl_wrapper_h__
#define open_gl_wrapper_h__

#include "GL/glew.h"

namespace DigitalSculpt
{
    class GL
    {
    public:
        static const GLuint UnbindAllVAO; // Value to signify we are unbinding all VAOS.
        static const GLsizei GenerateOneBuffer;
        static void unbindActiveVAO(); // unbinds current VAO binding to GPU.
        static void drawFilled();
        static void drawLined();
        static void clearDepthBuffer();
        static void set3DRenderingParameters();
        static void printOpenGLEnviromentInfo();
        static void bindVertexArray(GLuint id);
        static void bindArrayBuffer(GLuint id);
        static void bindElementArrayBuffer(GLuint id);
    };
}

#endif // open_gl_wrapper_h__
