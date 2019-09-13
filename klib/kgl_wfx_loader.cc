/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#include "kgl_lib_pch.h"
#include "kgl_stb_image.h"
#include "kgl_wfx_loader.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"

namespace kgl
{
    namespace water_effect
    {
        RawModel Loader::loadToVao(const std::vector<GLfloat>& positions,
            const std::vector<GLfloat>& textureCoords,
            const std::vector<GLfloat>& normals,
            const std::vector<GLuint>& indices)
        {
            GLuint vaoId = createVao();
            bindIndexBuffer(indices);
            storeDataInAttributeList(0, 3, positions);
            storeDataInAttributeList(1, 2, textureCoords);
            storeDataInAttributeList(2, 3, normals);
            unbindVao();
            return RawModel(vaoId, indices.size());
        }

        RawModel Loader::loadToVao(const std::vector<GLfloat>& positions, int dimensions)
        {
            GLuint vaoId = createVao();
            storeDataInAttributeList(0, dimensions, positions);
            unbindVao();
            return RawModel(vaoId, positions.size() / dimensions);
        }

        GLuint Loader::loadTexture(const std::string& textureFile)
        {
            kgl::stb::Image image(textureFile);

            if (!image)
            {
                std::wstring msg(L"无法打开图片文件：\n");
                msg.append(kgl::StringConvertor::UTF8toUTF16LE(textureFile.c_str()));
                throw kgl::Error(msg, __FILE__, __LINE__);
            }

            TextureHandle texture;
            glGenTextures(1, &texture);
            GLuint texture_id = texture.get();
            //textures.push_back(std::move(texture));

            // 根据所读取的图片的像素直接位数的不同，设置不同的读取标志
            GLenum mode = (image.num_components() == 4 ? GL_RGBA : GL_RGB);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexImage2D(GL_TEXTURE_2D, 0, mode, image.width(), image.height(), 0, mode,GL_UNSIGNED_BYTE, image.data());
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            textures_.push_back(std::move(texture));
            return texture_id;
        }

        GLuint Loader::loadCubeMap(const std::vector<std::string>& textureFiles) 
        {
            TextureHandle texture;
            glGenTextures(1, &texture);
            GLuint textureId = texture.get();
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

            for (GLuint i = 0; i < textureFiles.size(); i++) 
            {
                kgl::stb::Image img(textureFiles[i]);

                if (!img)
                {
                    std::wstring msg(L"无法打开图片文件：\n");
                    msg.append(kgl::StringConvertor::UTF8toUTF16LE(textureFiles[i].c_str()));
                    throw kgl::Error(msg, __FILE__, __LINE__);
                }

                GLenum mode = (img.num_components() == 4 ? GL_RGBA : GL_RGB);

                // 因为着色器里面用到的纹理是4通道的，所以无论源图片是4通道还是3通道，
                // glTexImage2D函数的的第三个参数为GL_RGBA
                glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img.width(),
                    img.height(), 0, mode, GL_UNSIGNED_BYTE, img.data());
            }

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            textures_.push_back(std::move(texture));
            return textureId;
        }

        GLuint Loader::createVao()
        {
            VertexArrayHandle vao;
            glGenVertexArrays(1, &vao);
            GLuint vaoId = vao.get();
            vaos_.push_back(std::move(vao));
            glBindVertexArray(vaoId);
            return vaoId;
        }

        void Loader::bindIndexBuffer(std::vector<GLuint> const& indices)
        {
            BufferHandle vbo;
            glGenBuffers(1, &vbo);
            GLuint vboId = vbo.get();
            vbos_.push_back(std::move(vbo));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        }

        void Loader::storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<GLfloat> const& data) 
        {
            BufferHandle vbo;
            glGenBuffers(1, &vbo);
            GLuint vboId = vbo.get();
            vbos_.push_back(std::move(vbo));
            glBindBuffer(GL_ARRAY_BUFFER, vboId);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void Loader::unbindVao()
        {
            glBindVertexArray(0);
        }
    }
}