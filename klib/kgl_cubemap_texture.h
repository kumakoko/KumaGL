// 纹理类的基类
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