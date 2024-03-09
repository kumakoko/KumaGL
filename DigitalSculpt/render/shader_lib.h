#ifndef digital_sculpt_shader_lib_h__
#define digital_sculpt_shader_lib_h__

#include <string>
#include <map>
#include "misc/enums.h"
#include "render/shaders/shader_base.h"

namespace DigitalSculpt
{
    class ShaderLibrary
    {
    private:
        static std::map<Enums::Shader, ShaderBase*> _dictionary;
        static ShaderLibrary* _instance;
        ShaderLibrary();
    public:
        ShaderLibrary& GetInstance();
        ShaderBase* operator [] (Enums::Shader index);
    };

#define ShaderLib (ShaderLibrary::GetInstance())
}

#endif // digital_sculpt_shader_lib_h__
