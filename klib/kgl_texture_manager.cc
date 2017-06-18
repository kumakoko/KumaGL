// 全局的纹理管理类
#include "kgl_lib_pch.h"
#include "kgl_texture_manager.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_source_texture.h"

namespace kgl
{
    TextureManager::~TextureManager()
    {
        this->PurgeAll();
    }

    bool TextureManager::IsTextureExist(const std::string& name)
    {
        std::string lower_name(name);
        boost::to_lower(lower_name);
        if (texture_map_.find(lower_name) == texture_map_.end())
            return false;

        return true;
    }

    TextureSPtr TextureManager::GetTexture(const std::string& name)
    {
        std::string lower_name(name);
        boost::to_lower(lower_name);
        std::map<std::string, TextureSPtr>::iterator it = texture_map_.find(lower_name);

        if ( it == texture_map_.end())
            return std::make_shared<Texture>();

        return it->second;
    }

    TextureSPtr TextureManager::CreateTextureFromFile(const std::string& name, TextureType type, const TextureParams& params)
    {
        if (this->IsTextureExist(name))
        {
            std::wstringstream wss;
            wss << L"The texture named " << StringConvertor::ANSItoUTF16LE(name.c_str())<<L" has been created";
            throw Error(wss.str());
        }

        switch (type)
        {
        case SOURCE_2D_TEXTURE:
            {
                SourceTextureSPtr p = std::make_shared<SourceTexture>();
                p->CreateFromFile(name, params);
                std::string lower_name(name);
                boost::to_lower(lower_name);
                texture_map_[lower_name] = p;
                return p;
            }
            break;
        default:
            {
                throw Error(L"The texture type is not supported now.");
            }
            break;
        }
    }

    void TextureManager::PurgeAll()
    {
        std::map<std::string, TextureSPtr>::iterator it = texture_map_.begin();

        while (it != texture_map_.end())
        {
            it->second.reset();
            ++it;
        }

        texture_map_.clear();
    }
}