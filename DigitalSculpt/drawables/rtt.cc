#include "scene.h"
#include "drawables/rtt.h"

namespace DigitalSculpt
{
    Buffer* Rtt::singletonBuffer = nullptr;

    Rtt::Rtt(Enums::Shader shaderType, GLuint depth/* = gl.createRenderbuffer()*/, bool halfFloat)
    {

        /*this->_texture = */glGenTextures(1, &_texture);//gl.createTexture();
        this->_depth = depth;
        /*this->_framebuffer = */glCreateFramebuffers(1, &_framebuffer);//gl.createFramebuffer();

        this->_shaderType = shaderType;
        _invSize.resize(2, 0.0f);// this->_invSize = new Float32Array(2);
        this->_vertexBuffer = nullptr;

        if (halfFloat && WebGLCaps.hasRTTHalfFloat())
            this->_type = GL_HALF_FLOAT;//WebGLCaps.HALF_FLOAT_OES;
        else if (halfFloat && WebGLCaps.hasRTTFloat())
            this->_type = GL_FLOAT;//gl.FLOAT;
        else
            this->_type = GL_UNSIGNED_BYTE;//gl.UNSIGNED_BYTE;

        this->setWrapRepeat(false);
        this->setFilterNearest(false);
        this->init();
    }

    void Rtt::init()
    {
        if (singletonBuffer == nullptr)
        {
            Float32Array data{ -1.0f, -1.0f, 4.0f, -1.0f, -1.0f, 4.0f };
            singletonBuffer = new Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
            singletonBuffer->update(data, data.size());
        }

        this->_vertexBuffer = singletonBuffer;
    }

    void Rtt::onResize(float width, float height)
    {
        this->_invSize[0] = 1.0f / width;
        this->_invSize[1] = 1.0f / height;

        glBindTexture(GL_TEXTURE_2D, this->_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, this->_type, nullptr);

        auto filter = this->_filterNearest ? GL_NEAREST : GL_LINEAR;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);

        auto wrap = this->_wrapRepeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

        if (this->_depth != 0)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, this->_depth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, this->_framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_texture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->_depth);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Rtt::release()
    {
        if (this->_texture != 0)
        {
            glDeleteTextures(1, &_texture);
        }

        this->getVertexBuffer()->release();
    }

    void Rtt::render(Scene* main)
    {
        auto shaderLib = ShaderLibrary::GetInstance();
        shaderLib[this->_shaderType]->getOrCreate()->draw(this, main);
    }
}
