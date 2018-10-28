// 可渲染的纹理
/*!
 * \file kgl_rendered_texture.h
 * \date 2017/12/31 13:57
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
#ifndef kgl_rendered_texture_h__
#define kgl_rendered_texture_h__

#include "kgl_texture.h"

namespace kgl
{
    /// <summary>
    /// 可渲染的纹理的纹素格式
    /// </summary>
    enum class RenderedTexelType
    {
        RGB,
        RGBA,
        RGBA16F,
        DEPTH_COMPONENT
    };

    class RenderedTexture : public Texture
    {
    public:
        /// <summary>
        /// 初始化一个<see cref="RenderedTexture"/> 类实例对象.
        /// </summary>
        /// <param name="used_type">Rendered Texture的使用类型，也就是纹理格式类型</param>
        RenderedTexture(RenderedTexelType used_type);

        /// <summary>
        ///  <see cref="RenderedTexture"/>类的析构函数.
        /// </summary>
        virtual ~RenderedTexture();

        /// <summary>
        /// 根据指定的高宽创建纹理
        /// </summary>
        /// <param name="width">纹理的宽</param>
        /// <param name="height">纹理的高</param>
        /// <param name="texture_params">纹理创建参数</param>
        void Create(int32_t width, int32_t height, const TextureParams& texture_params) override;

        /// <summary>
        /// 获取纹理的宽
        /// </summary>
        /// <returns>纹理的宽</returns>
        virtual int GetWidth() const override;

        /// <summary>
        /// 获取纹理的高
        /// </summary>
        /// <returns>纹理的高</returns>
        virtual int GetHeight() const override;

        /// <summary>
        /// 返回本纹理是否启用mipmap
        /// </summary>
        /// <returns>启用返回true，否则返回false</returns>
        virtual bool IsMipmap() const override;

        /// <summary>
        ///  激活并绑定本纹理到OpenGL管线
        /// </summary>
        /// <param name="slot_index">本纹理所绑定到的纹理槽位</param>
        virtual void ActiveBind(GLuint slot_index) override;

        inline GLuint GetTextureID() const
        {
            return texture_id_;
        }

        inline RenderedTexelType GetTexelType() const
        {
            return texel_type_;
        }

        /// <summary>
        /// 根据使用类型，决定返回的用于frame buffer的attachment的类型
        /// </summary>
        /// <returns>返回的用于frame buffer的attachment的类型</returns>
        GLenum GetAttachmentForFrameBuffer();
        
    protected:
        RenderedTexelType texel_type_;
        GLuint            texture_id_ = 0;
        int               width_ = 0;
        int               height_ = 0;
    };

    typedef std::shared_ptr<RenderedTexture> RenderedTextureSPtr;
}
#endif // kgl_rendered_texture_h__