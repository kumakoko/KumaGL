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
        ~TextureManager();
        bool IsTextureExist(const std::string& name);
        TextureSPtr GetTexture(const std::string& name);
        TextureSPtr CreateTextureFromFile(const std::string& name, TextureType type, const TextureParams& params);
        void PurgeAll();
    private:
        std::map<std::string, TextureSPtr> texture_map_;
    };

    typedef dp::Singleton<TextureManager> KTextureManager;
}
#endif // kgl_texture_manager_h__