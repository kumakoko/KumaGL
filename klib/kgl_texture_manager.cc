// 全局的纹理管理类
#include "kgl_lib_pch.h"
#include "kgl_texture_manager.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_source_texture.h"

#if defined(__APPLE__) && defined(__MACH__)
#include "SOIL.h"
#endif

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
                p->CreateFromFileObsolete(name, params);
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

    TextureParams TextureManager::MakeTextureParamsRGB(GLint wrap_mode, GLint filter_mode, GLint src_img_px_component_type, bool use_mipmap)
    {
        TextureParams texture_param;
        texture_param.wrap_s_mode = wrap_mode;
        texture_param.wrap_t_mode = wrap_mode;
        texture_param.mag_filter_mode = filter_mode;
        texture_param.min_filter_mode = filter_mode;
        texture_param.internal_format = GL_RGB;
        texture_param.src_img_px_component_type = src_img_px_component_type;
        texture_param.src_img_format = GL_RGB;
        texture_param.used_mipmap = use_mipmap;
        return texture_param;
    }

    TextureParams TextureManager::MakeTextureParamsRGBA(GLint wrap_mode, GLint filter_mode, GLint src_img_px_component_type, bool use_mipmap)
    {
        TextureParams texture_param;
        texture_param.wrap_s_mode = wrap_mode;
        texture_param.wrap_t_mode = wrap_mode;
        texture_param.mag_filter_mode = filter_mode;
        texture_param.min_filter_mode = filter_mode;
        texture_param.internal_format = GL_RGBA;
        texture_param.src_img_px_component_type = src_img_px_component_type;
        texture_param.src_img_format = GL_RGBA;
        texture_param.used_mipmap = use_mipmap;
        return texture_param;
    }

    TextureParams TextureManager::MakeTextureParams(GLint wrap_s_mode, GLint wrap_t_mode, GLint mag_filter_mode, GLint min_filter_mode, GLint internal_format, GLint src_img_px_component_type, GLint src_img_format, GLint load_chanel, bool use_mipmap)
    {
        TextureParams texture_param;
        texture_param.wrap_s_mode = wrap_s_mode;
        texture_param.wrap_t_mode = wrap_t_mode;
        texture_param.mag_filter_mode = mag_filter_mode;
        texture_param.min_filter_mode = min_filter_mode;
        texture_param.internal_format = internal_format;
        texture_param.src_img_px_component_type = src_img_px_component_type;
        texture_param.src_img_format = src_img_format;
        texture_param.used_mipmap = use_mipmap;
        return texture_param;
    }
}
