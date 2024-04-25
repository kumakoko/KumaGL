#include "scene.h"
#include "drawables/back_ground.h"

namespace DigitalSculpt
{
    Background::Background(Scene* main)
    {
        this->_main = main;

        this->_vertexBuffer = new Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW); // vertices buffer
        this->_texCoordBuffer = new Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW); // tex coord buffer
        this->_fill = true; // if the canvas should be fille by the background

        this->_monoTex = 0;
        this->_texture = 0; // texture background
        this->_texWidth = 1;
        this->_texHeight = 1;

        this->_type = 0; // 0: fixed grey, 1 env spec, 2 env ambient
        this->_blur = 0.0;

        this->init();
    }

    void Background::init()
    {
        Float32Array data{ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
        _texCoordBuffer->update(data, 8);
        this->_monoTex = this->createOnePixelTexture(50, 50, 50, 255);
        //document.getElementById('backgroundopen').addEventListener('change', this->loadBackground.bind(this), false);
    }

    /* ÔÝÊ±×¢½â
    loadBackground(event) {
        if (event.target.files.length == = 0)
            return;

        var file = event.target.files[0];
        if (!file.type.match('image.*'))
            return;

        var self = this;
        var reader = new FileReader();
        reader.onload = function(evt) {
            var bg = new Image();
            bg.src = evt.target.result;

            bg.onload = function() {

                var canvas = self._main.getCanvas();
                self.loadBackgroundTexture(bg);
                self.onResize(canvas.width, canvas.height);
                self._main.render();
            };
        };

        document.getElementById('backgroundopen').value = '';
        reader.readAsDataURL(file);
    }
    */

    void Background::release()
    {
        this->deleteTexture();
        this->getVertexBuffer()->release();
        this->getTexCoordBuffer()->release();
    }

    void Background::onResize(float width, float height)
    {
        float ratio = (width / height) / (this->_texWidth / this->_texHeight);
        float comp = this->_fill || this->_type != 0 ? 1.0f / ratio : ratio;
        float x = comp < 1.0f ? 1.0f : 1.0f / ratio;
        float y = comp < 1.0f ? ratio : 1.0f;
        Float32Array box{ -x, -y, x, -y, -x, y, x, y };
        this->_vertexBuffer->update(box, 6);
    }

    GLuint Background::createOnePixelTexture(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
    {
        GLuint tex = 0;
        std::uint8_t px[4] = { r,g,b,a };
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, px);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, tex);
        return tex;
    }

    void Background::loadBackgroundTexture(const kgl::ImageFileReader& tex)
    {
        this->deleteTexture();
        this->_texWidth = tex.ImageWidth();//tex.width;
        this->_texHeight = tex.ImageHeight();
        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, this->_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _texWidth, _texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.GetImageData());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Background::deleteTexture()
    {
        if (this->_texture != 0)
        {
            this->_texWidth = this->_texHeight = 1;
            glDeleteTextures(1, &_texture);
            this->_texture = 0;
        }
    }

    void Background::render()
    {
        ShaderLibrary& lib = ShaderLibrary::GetInstance();
        lib[Enums::Shader::BACKGROUND]->getOrCreate()->draw(this);
    }
}
