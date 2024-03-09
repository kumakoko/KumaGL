#ifndef digital_sculpt_shader_base_h__
#define digital_sculpt_shader_base_h__


#include "misc/get_options_url.h" //import getOptionsURL from "misc/getOptionsURL";
#include "misc/utils.h"
#include "render/attribute.h"
//
//import Attribute from "render/Attribute";
//import colorSpaceGLSL from "render/shaders/glsl/colorSpace.glsl";
//import curvatureGLSL from "render/shaders/glsl/curvature.glsl";

namespace DigitalSculpt
{
    class ShaderBase
    {
    public:
        virtual ~ShaderBase();
    };
}

#endif // digital_sculpt_shader_base_h__