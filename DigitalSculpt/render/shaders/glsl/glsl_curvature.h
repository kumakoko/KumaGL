#ifndef digital_sculpt_glsl_curvature_h__
#define digital_sculpt_glsl_curvature_h__

constexpr const char* curvatureGLSL =
"// http://madebyevan.com/shaders/curvature/\n"
"#extension GL_OES_standard_derivatives : enable\n"
"vec3 computeCurvature(const in vec3 vertex, const in vec3 normal, const in vec3 color, const in float str, const in float fov) {\n"
"    if (str < 1e-3)\n"
"        return color;\n"
"#ifndef GL_OES_standard_derivatives\n"
"    return color * pow(length(normal), str * 100.0);\n"
"#else\n"
"    vec3 n = normalize(normal);\n"
"    // Compute curvature\n"
"    vec3 dx = dFdx(n);\n"
"    vec3 dy = dFdy(n);\n"
"    vec3 xneg = n - dx;\n"
"    vec3 xpos = n + dx;\n"
"    vec3 yneg = n - dy;\n"
"    vec3 ypos = n + dy;\n"
"    // fov < 0.0 means ortho\n"
"    float depth = fov > 0.0 ? length(vertex) * fov : -fov;\n"
"    float cur = (cross(xneg, xpos).y - cross(yneg, ypos).x) * str * 80.0 / depth;\n"
"    return mix(mix(color, color * 0.3, clamp(-cur * 15.0, 0.0, 1.0)), color * 2.0, clamp(cur * 25.0, 0.0, 1.0));\n"
"#endif\n"
"}\n";

#endif // digital_sculpt_glsl_curvature_h__
