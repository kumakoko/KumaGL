// 全局的纹理管理类
#ifndef kgl_texture_manager_h__
#define kgl_texture_manager_h__

#include "kgl_texture.h"
#include "dp/kgl_singleton.h"

namespace kgl
{
    class TextureManager
    {
    public:
		/// <summary>
		/// Finalizes an instance of the <see cref="TextureManager"/> class.
		/// </summary>
		~TextureManager();
		/// <summary>
		/// Determines whether [is texture exist] [the specified name].
		/// </summary>
		/// <param name="name">The name.</param>
		/// <returns>bool.</returns>
		bool IsTextureExist(const std::string& name);
		/// <summary>
		/// Gets the texture.
		/// </summary>
		/// <param name="name">The name.</param>
		/// <returns>TextureSPtr.</returns>
		TextureSPtr GetTexture(const std::string& name);

		/// <summary>
		/// Creates the texture from file.
		/// </summary>
		/// <param name="name">The name.</param>
		/// <param name="type">The type.</param>
		/// <param name="params">The parameters.</param>
		/// <returns>TextureSPtr.</returns>
		TextureSPtr CreateTextureFromFile(const std::string& name, TextureType type, const TextureParams& params);

		/// <summary>
		/// Purges all.
		/// </summary>
		void PurgeAll();
    private:
		/// <summary>
		/// The texture_map_
		/// </summary>
		std::map<std::string, TextureSPtr> texture_map_;
    };

    typedef dp::Singleton<TextureManager> KTextureManager;
}
#endif // kgl_texture_manager_h__