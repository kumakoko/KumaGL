// 可渲染的纹理
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
        RGBA16F
    };

    class RenderedTexture : public Texture
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="RenderedTexture"/> class.
        /// </summary>
        /// <param name="used_type">The used_type.</param>
        RenderedTexture(RenderedTexelType used_type);

        /// <summary>
        /// Finalizes an instance of the <see cref="RenderedTexture"/> class.
        /// </summary>
        virtual ~RenderedTexture();

       
        /// <summary>
        /// Creates the specified width.
        /// </summary>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        /// <param name="texture_params">The texture_params.</param>
        void Create(int32_t width, int32_t height, const TextureParams& texture_params) override;

        /// <summary>
        /// Creates the specified texture_width.
        /// </summary>
        /// <param name="texture_width">The texture_width.</param>
        /// <param name="texture_height">The texture_height.</param>
        //void Create(int32_t texture_width, int32_t texture_height);

        /// <summary>
        /// Gets the width.
        /// </summary>
        /// <returns>int.</returns>
        virtual int GetWidth() const override;

        /// <summary>
        /// Gets the height.
        /// </summary>
        /// <returns>int.</returns>
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

    protected:
        RenderedTexelType texel_type_;
        GLuint            texture_id_ = 0;
        int               width_ = 0;
        int               height_ = 0;
    };

    typedef std::shared_ptr<RenderedTexture> RenderedTextureSPtr;
}
#endif // kgl_rendered_texture_h__