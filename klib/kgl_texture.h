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
/*!
* \file kgl_texture.h
* \date 2017/12/31 13:59
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: 纹理类的基类
*
* \note
*/

#ifndef kgl_texture_h__
#define kgl_texture_h__

namespace kgl
{
    /// <summary>
    /// 纹理的类型
    /// </summary>
    enum TextureType
    {
        NONE,

		/** * @brief 2D纹理 */
        SOURCE_2D_TEXTURE,

		/** * @brief 立方体纹理 */
        CUBEMAP_TEXTURE,

		/** * @brief 可作为渲染目标的纹理*/
        RENDERED_TEXTURE
    };

    struct TextureParams
    {
    public:
		/** * @brief  wrap s mode */
        GLint wrap_s_mode;

		/** * @brief wrap t mode */
        GLint wrap_t_mode;

		/** * @brief mag filter mode */
        GLint mag_filter_mode;

		/** * @brief min filter mode */
        GLint min_filter_mode;

		/** * @brief 纹理在内存（系统或者显卡中）中的存储模式 */
        GLint internal_format;

		/** * @brief 原始图片中的颜色数据的格式 */
        GLint src_img_format;

		/** * @brief 原始图片中颜色每一个分量在内存中占用的字节类型 */
        GLint src_img_px_component_type;

		/** * @brief 是否使用mipmap */
        bool  used_mipmap;
    };

    /// <summary>
    /// 纹理类的基类
    /// </summary>
    class Texture
    {
    public:
        /// <summary>
        /// <see cref="Texture"/>类的构造函数
        /// </summary>
        Texture();

        /// <summary>
        /// Finalizes an instance of the <see cref="Texture"/>类的析构函数
        /// </summary>
        virtual ~Texture();

        /// <summary>
        /// Creates from file.
        /// </summary>
        /// <param name="file_name">The file_name.</param>
        /// <param name="texture_params">The texture_params.</param>
        virtual void CreateFromFile(const std::string& file_name, const TextureParams& texture_params);

        /// <summary>
        /// Creates the specified width.
        /// </summary>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        /// <param name="texture_params">The texture_params.</param>
        virtual void Create(int32_t width, int32_t height, const TextureParams& texture_params);

        /// <summary>
        /// Gets the width.
        /// </summary>
        /// <returns>int.</returns>
        virtual int GetWidth() const;

        /// <summary>
        /// Gets the height.
        /// </summary>
        /// <returns>int.</returns>
        virtual int GetHeight() const;

        /// <summary>
        /// Determines whether this instance is mipmap.
        /// </summary>
        /// <returns>bool.</returns>
        virtual bool IsMipmap() const;

        /// <summary>
        ///  激活并绑定本纹理到OpenGL管线
        /// </summary>
        /// <param name="slot_index">The slot_index.</param>
        virtual void ActiveBind(GLuint slot_index);

        /// <summary>
        /// 获取到纹理的类型
        /// </summary>
        /// <returns>返回纹理的类型值</returns>
        inline TextureType GetType() const
        {
            return type_;
        }

        /// <summary>
        /// 纹理是否被正确地创建出来并且是可用的
        /// </summary>
        /// <returns>有效返回true，否则返回false</returns>
        inline bool IsValid() const
        {
            return valid_;
        }

    protected:
        /// <summary>
        /// 纹理的类型
        /// </summary>
        TextureType type_ = TextureType::NONE;

        /// <summary>
        /// 是否创建成功
        /// </summary>
        bool valid_ = false;
    };

    typedef std::shared_ptr<Texture> TextureSPtr;
}

#endif // kgl_texture_h__