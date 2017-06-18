// 可渲染的纹理
#ifndef kgl_rendered_texture_h__
#define kgl_rendered_texture_h__

#include "kgl_texture.h"

namespace kgl
{
    enum class RenderedTextureUsedType
    {
        COLOR_RGB_ONLY,
        COLOR_RGBA_ONLY,
        STENCIL_ONLY,
        DEPTH_ONLY,
        WITH_DEPTH_STENCIL,
    };

    class RenderedTexture : public Texture
    {
    public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RenderedTexture"/> class.
		/// </summary>
		/// <param name="used_type">The used_type.</param>
		RenderedTexture(RenderedTextureUsedType used_type);

		/// <summary>
		/// Finalizes an instance of the <see cref="RenderedTexture"/> class.
		/// </summary>
		virtual ~RenderedTexture();

		/// <summary>
		/// Creates from file.
		/// </summary>
		/// <param name="file_name">The file_name.</param>
		/// <param name="texture_params">The texture_params.</param>
		virtual void CreateFromFile(const std::string& file_name, const TextureParams& texture_params) override;

		/// <summary>
		/// Creates the specified width.
		/// </summary>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="texture_params">The texture_params.</param>
		virtual void Create(int width, int height, const TextureParams& texture_params) override;

		/// <summary>
		/// Creates the specified texture_width.
		/// </summary>
		/// <param name="texture_width">The texture_width.</param>
		/// <param name="texture_height">The texture_height.</param>
		void Create(int32_t texture_width, int32_t texture_height);

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

		void StartWriting()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
		}

		void EndWritting()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

    protected:
        RenderedTextureUsedType used_type_;
        GLuint                  fbo_;
        GLuint                  texture_id_;
        GLuint                  rbo_;
    };

	typedef std::shared_ptr<RenderedTexture> RenderedTextureSPtr;
}
#endif // kgl_rendered_texture_h__