#ifndef digital_sculpt_rtt_h__
#define digital_sculpt_rtt_h__

#include <string>
#include "../klib/kgl_lib_pch.h"
#include "render/buffer.h"
#include "render/shader_lib.h"
#include "misc/enums.h"
#include "misc/utils.h"

namespace DigitalSculpt
{
    //auto singletonBuffer;
    class Scene;

    class Rtt
    {
    public:
        static Buffer* singletonBuffer;
    protected:
        GLenum _type;
        GLuint _texture;
        GLuint _depth; // render buffer handle
        GLuint _framebuffer;
        Float32Array _invSize;
        Enums::Shader _shaderType;
        Buffer* _vertexBuffer;
        bool _wrapRepeat;
        bool _filterNearest;
    public:
        Rtt(Enums::Shader shaderType, GLuint depth/* = gl.createRenderbuffer()*/, bool halfFloat = false);

        inline Buffer* getVertexBuffer()
        {
            return this->_vertexBuffer;
        }

        inline const Buffer* getVertexBuffer() const
        {
            return this->_vertexBuffer;
        }

        inline GLuint getFramebuffer() const
        {
            return this->_framebuffer;
        }

        inline GLuint getTexture() const
        {
            return this->_texture;
        }

        inline GLuint getDepth() const
        {
            return this->_depth;
        }

        inline const Float32Array& getInverseSize() const
        {
            return this->_invSize;
        }

        inline Float32Array& getInverseSize()
        {
            return this->_invSize;
        }

        void init();

        inline void setWrapRepeat(bool w)
        {
            this->_wrapRepeat = w;
        }

        inline void setFilterNearest(bool f)
        {
            this->_filterNearest = f;
        }

        void onResize(float width, float height);

        void release();

        void render(Scene* main);
    };
}

#endif // digital_sculpt_rtt_h__