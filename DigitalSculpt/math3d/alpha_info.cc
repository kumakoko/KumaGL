#include "math3d/alpha_info.h"

namespace DigitalSculpt
{
    AlphaInfo::AlphaInfo(const std::string& name, kgl::TextureSPtr u8, float ratioX, float ratioY, float ratioMax, float w, float h) :
        _name(name), _texture(u8), _ratioX(ratioX), _ratioY(ratioY), _ratioMax(ratioMax), _width(w), _height(h)
    {

    }

    AlphaInfo::AlphaInfo()
    {
    }
}