#ifndef digital_sculpt_alpha_info_h__
#define digital_sculpt_alpha_info_h__

//#include "../klib/kgl_texture.h"
#include "../klib/kgl_lib_pch.h"
#include <string>

namespace DigitalSculpt
{
    struct AlphaInfo
    {
        std::string _name;
        kgl::TextureSPtr _texture;
        float _ratioX;
        float _ratioY;
        float _ratioMax;
        float _width;
        float _height;

        AlphaInfo();
        AlphaInfo(const std::string& name, kgl::TextureSPtr u8, float ratioX, float ratioY, float ratioMax, float w, float h);
    };
}
#endif // digital_sculpt_alpha_info_h__
