// 纹理类的基类
/*!
 * \file kgl_cubemap_texture.h
 * \date 2017/12/31 13:35
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

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
#ifndef kgl_cubemap_texture_h__
#define kgl_cubemap_texture_h__

#include "kgl_texture.h"

namespace kgl
{
    class CubemapTexture : public Texture
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="CubemapTexture"/> class.
        /// </summary>
        /// <param name="positive_x_file">X方向上正方向贴图</param>
        /// <param name="negative_x_file">X方向上负方向贴图.</param>
        /// <param name="positive_y_file">Y方向上正方向贴图</param>
        /// <param name="negative_y_file">Y方向上负方向贴图</param>
        /// <param name="positive_z_file">Z方向上正方向贴图</param>
        /// <param name="negative_z_file">Z方向上负方向贴图</param>
        CubemapTexture(
            const std::string& positive_x_file,
            const std::string& negative_x_file,
            const std::string& positive_y_file,
            const std::string& negative_y_file,
            const std::string& positive_z_file,
            const std::string& negative_z_file);

        /// <summary>
        /// Finalizes an instance of the <see cref="CubemapTexture"/> class.
        /// </summary>
        ~CubemapTexture();

        /// <summary>
        /// Loads this instance.
        /// </summary>
        /// <returns>bool.</returns>
        bool Load();

        /// <summary>
        /// Binds the specified texture unit.
        /// </summary>
        /// <param name="TextureUnit">The texture unit.</param>
        void Bind(GLenum TextureUnit);

        /// <summary>
        /// Actives the bind.
        /// </summary>
        /// <param name="slot_index">激活并绑定本纹理到OpenGL管线</param>
        virtual void ActiveBind(GLuint slot_index) override;
    private:
        std::string file_name_[6];
        GLuint texture_id_;
    };

    typedef std::shared_ptr<CubemapTexture> CubemapTextureSPtr;
}

#endif // kgl_cubemap_texture_h__