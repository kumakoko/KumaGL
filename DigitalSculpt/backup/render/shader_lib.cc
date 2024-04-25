#include <string>
#include <map>
#include "misc/enums.h"
#include "render/shader_lib.h"
#include "render/shaders/shader_pbr.h"
#include "render/shaders/shader_matcap.h"
#include "render/shaders/shader_normal.h"
#include "render/shaders/shader_uv.h"
#include "render/shaders/shader_wireframe.h"
#include "render/shaders/shader_flat.h"
#include "render/shaders/shader_selection.h"
#include "render/shaders/shader_background.h"
#include "render/shaders/shader_merge.h"
#include "render/shaders/shader_fxaa.h"
#include "render/shaders/shader_contour.h"
#include "render/shaders/shader_paint_uv.h"
#include "render/shaders/shader_blur.h"

namespace DigitalSculpt
{
    std::map<Enums::Shader, ShaderBase*> ShaderLibrary::_dictionary;
    ShaderLibrary* ShaderLibrary::_instance = nullptr;

    ShaderBase* ShaderLibrary::operator [] (Enums::Shader index)
    {
        auto iter = _dictionary.find(index);

        if (_dictionary.end() == iter)
            return nullptr;

        return iter->second;
    }

    ShaderLibrary::ShaderLibrary()
    {
        _dictionary.insert(Enums::Shader::PBR, new ShaderPBR);
        _dictionary.insert(Enums::Shader::MATCAP, new ShaderMatcap);
        _dictionary.insert(Enums::Shader::NORMAL, new ShaderNormal);
        _dictionary.insert(Enums::Shader::UV, new ShaderUV);
        _dictionary.insert(Enums::Shader::WIREFRAME, new ShaderWireframe);
        _dictionary.insert(Enums::Shader::FLAT, new ShaderFlat);
        _dictionary.insert(Enums::Shader::SELECTION, new ShaderSelection);

        // 2D screen shaders
        _dictionary.insert(Enums::Shader::BACKGROUND, new ShaderBackground);
        _dictionary.insert(Enums::Shader::MERGE, new ShaderMerge);
        _dictionary.insert(Enums::Shader::FXAA, new ShaderFxaa);
        _dictionary.insert(Enums::Shader::CONTOUR, new ShaderContour);

        // misc
        _dictionary.insert(Enums::Shader::PAINTUV, new ShaderPaintUV);
        _dictionary.insert(Enums::Shader::BLUR, new ShaderBlur);
    }

    ShaderLibrary& ShaderLibrary::GetInstance()
    {
        if (nullptr == _instance)
            _instance = new ShaderLibrary();

        return *_instance;
    }
}