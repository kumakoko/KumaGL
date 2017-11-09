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
        /// Makes the texture parameters RGB.
        /// </summary>
        /// <param name="wrap_mode">The wrap_mode.</param>
        /// <param name="filter_mode">The filter_mode.</param>
        /// <param name="src_img_px_component_type">The src_img_px_component_type.</param>
        /// <param name="use_mipmap">The use_mipmap.</param>
        /// <returns>kgl.TextureParams.</returns>
        static TextureParams MakeTextureParamsRGB(GLint wrap_mode, GLint filter_mode, GLint src_img_px_component_type = GL_UNSIGNED_BYTE, bool use_mipmap = false);
        
        /// <summary>
        /// Makes the texture parameters rgba.
        /// </summary>
        /// <param name="wrap_mode">The wrap_mode.</param>
        /// <param name="filter_mode">The filter_mode.</param>
        /// <param name="src_img_px_component_type">The src_img_px_component_type.</param>
        /// <param name="use_mipmap">The use_mipmap.</param>
        /// <returns>kgl.TextureParams.</returns>
        static TextureParams MakeTextureParamsRGBA(GLint wrap_mode, GLint filter_mode, GLint src_img_px_component_type = GL_UNSIGNED_BYTE, bool use_mipmap = false);

        /// <summary>
        /// Makes the texture parameters.
        /// </summary>
        /// <param name="wrap_s_mode">The wrap_s_mode.</param>
        /// <param name="wrap_t_mode">The wrap_t_mode.</param>
        /// <param name="mag_filter_mode">The mag_filter_mode.</param>
        /// <param name="min_filter_mode">The min_filter_mode.</param>
        /// <param name="internal_format">The internal_format.</param>
        /// <param name="src_img_px_component_type">The src_img_px_component_type.</param>
        /// <param name="src_img_format">The src_img_format.</param>
        /// <param name="load_chanel">The load_chanel.</param>
        /// <param name="use_mipmap">The use_mipmap.</param>
        /// <returns>kgl.TextureParams.</returns>
        static TextureParams MakeTextureParams(GLint wrap_s_mode,GLint wrap_t_mode,GLint mag_filter_mode,GLint min_filter_mode,GLint internal_format,GLint src_img_px_component_type,GLint src_img_format,GLint load_chanel,bool use_mipmap);

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