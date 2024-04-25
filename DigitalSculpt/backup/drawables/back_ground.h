#ifndef digital_sculpt_back_ground_h__
#define digital_sculpt_back_ground_h__

#include <cstdint>

#include "../klib/kgl_image_file_reader.h"
#include "render/buffer.h"
#include "render/shader_lib.h"
#include "misc/enums.h"

namespace DigitalSculpt
{
    class Scene;

    class Background
    {
    private:
        Scene* _main;
        Buffer* _vertexBuffer;
        Buffer* _texCoordBuffer;
        bool _fill;
        float _blur;
        std::uint32_t _texWidth;
        std::uint32_t _texHeight;
        std::uint32_t _type;
        GLuint _monoTex;
        GLuint _texture;
    public:

        Background(Scene* main);

        void init();

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


        inline float getBlur() const
        {
            return this->_blur;
        }

        inline Buffer* getVertexBuffer()
        {
            return this->_vertexBuffer;
        }

        inline const Buffer* getVertexBuffer() const
        {
            return this->_vertexBuffer;
        }

        inline Buffer* getTexCoordBuffer()
        {
            return this->_texCoordBuffer;
        }

        inline const Buffer* getTexCoordBuffer() const
        {
            return this->_texCoordBuffer;
        }

        void release();

        inline std::uint32_t getType() const
        {
            return this->_type;
        }

        inline void setType(std::uint32_t val)
        {
            this->_type = val;
        }

        void onResize(float width, float height);

        inline GLuint getTexture() const
        {
            return this->_texture != 0 ? this->_texture : this->_monoTex;
        }

        GLuint createOnePixelTexture(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

        void loadBackgroundTexture(const kgl::ImageFileReader& tex);

        void deleteTexture();

        void render();
    };
}

#endif // digital_sculpt_back_ground_h__
