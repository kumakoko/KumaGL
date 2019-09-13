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
/*!
 * \file kgl_loader.h
 * \date 2019/01/26 20:49
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef kgl_loader_h__
#define kgl_loader_h__

#include "kgl_wfx_raw_model.h"
#include "kgl_wfx_resource_handle.h"

namespace kgl
{
    namespace water_effect
    {
        class Loader
        {
        public:
            /// <summary>
            /// Loads to vao.
            /// </summary>
            /// <param name="positions">The positions.</param>
            /// <param name="textureCoords">The texture coords.</param>
            /// <param name="normals">The normals.</param>
            /// <param name="indices">The indices.</param>
            /// <returns>kgl.water_effect.RawModel.</returns>
            RawModel loadToVao(const std::vector<GLfloat>& positions, const std::vector<GLfloat>& textureCoords, const std::vector<GLfloat>& normals, const std::vector<GLuint>& indices);

            /// <summary>
            /// Loads to vao.
            /// </summary>
            /// <param name="positions">The positions.</param>
            /// <param name="dimensions">The dimensions.</param>
            /// <returns>kgl.water_effect.RawModel.</returns>
            RawModel loadToVao(const std::vector<GLfloat>& positions, int dimensions);

            /// <summary>
            /// Loads the texture.
            /// </summary>
            /// <param name="textureFile">The texture file.</param>
            /// <returns>GLuint.</returns>
            GLuint loadTexture(const std::string& textureFile);

            /// <summary>
            /// Loads the cube map.
            /// </summary>
            /// <param name="textureFiles">The texture files.</param>
            /// <returns>GLuint.</returns>
            GLuint loadCubeMap(const std::vector<std::string>& textureFiles);

        private:
            /// <summary>
            /// Creates the vao.
            /// </summary>
            /// <returns>GLuint.</returns>
            GLuint createVao();

            /// <summary>
            /// Binds the index buffer.
            /// </summary>
            /// <param name="indices">The indices.</param>
            void bindIndexBuffer(std::vector<GLuint> const& indices);

            /// <summary>
            /// Stores the data in attribute list.
            /// </summary>
            /// <param name="attributeNumber">The attribute number.</param>
            /// <param name="coordinateSize">Size of the coordinate.</param>
            /// <param name="data">The data.</param>
            void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<GLfloat> const& data);

            /// <summary>
            /// Unbinds the vao.
            /// </summary>
            void unbindVao();

            /// <summary>
            /// The vaos_
            /// </summary>
            std::vector<VertexArrayHandle> vaos_;

            /// <summary>
            /// The vbos_
            /// </summary>
            std::vector<BufferHandle>      vbos_;

            /// <summary>
            /// The textures_
            /// </summary>
            std::vector<TextureHandle>     textures_;
        };

        typedef std::shared_ptr<Loader> LoaderSPtr;
    }
}

#endif // kgl_loader_h__