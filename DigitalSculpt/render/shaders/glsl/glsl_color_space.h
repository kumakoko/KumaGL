#ifndef digital_sculpt_glsl_color_space_h__
#define digital_sculpt_glsl_color_space_h__

namespace DigitalSculpt
{
    constexpr const char* colorSpaceGLSL =
        "// reference\n"
        "// https://www.khronos.org/registry/gles/extensions/EXT/EXT_sRGB.txt\n"
        "\n"
        "#define LIN_SRGB(x) x < 0.0031308 ? x * 12.92 : 1.055 * pow(x, 1.0/2.4) - 0.055\n"
        "\n"
        "float linearTosRGB(const in float c) {\n"
        "   return LIN_SRGB(c);\n"
        "}\n"
        "vec3 linearTosRGB(const in vec3 c) {\n"
        "   return vec3(LIN_SRGB(c.r), LIN_SRGB(c.g), LIN_SRGB(c.b));\n"
        "}\n"
        "vec4 linearTosRGB(const in vec4 c) {\n"
        "   return vec4(LIN_SRGB(c.r), LIN_SRGB(c.g), LIN_SRGB(c.b), c.a);\n"
        "}\n"
        "\n"
        "#define SRGB_LIN(x) x < 0.04045 ? x * (1.0 / 12.92) : pow((x + 0.055) * (1.0 / 1.055), 2.4)\n"
        "\n"
        "float sRGBToLinear(const in float c) {\n"
        "   return SRGB_LIN(c);\n"
        "}\n"
        "vec3 sRGBToLinear(const in vec3 c) {\n"
        "   return vec3(SRGB_LIN(c.r), SRGB_LIN(c.g), SRGB_LIN(c.b));\n"
        "}\n"
        "vec4 sRGBToLinear(const in vec4 c) {\n"
        "   return vec4(SRGB_LIN(c.r), SRGB_LIN(c.g), SRGB_LIN(c.b), c.a);\n"
        "}\n"
        "\n"
        "#define RANGE 5.0\n"
        "// http://graphicrants.blogspot.fr/2009/04/rgbm-color-encoding.html\n"
        "vec4 encodeRGBM(const in vec3 col) {\n"
        "   vec4 rgbm;\n"
        "   vec3 color = col / RANGE;\n"
        "   rgbm.a = clamp(max(max(color.r, color.g), max(color.b, 1e-6)), 0.0, 1.0);\n"
        "   rgbm.a = ceil(rgbm.a * 255.0) / 255.0;\n"
        "   rgbm.rgb = color / rgbm.a;\n"
        "   return rgbm;\n"
        "}\n"
        "\n"
        "vec3 decodeRGBM(const in vec4 col) {\n"
        "   return RANGE * col.rgb * col.a;\n"
        "}\n";
}
#endif // digital_sculpt_glsl_color_space_h__
