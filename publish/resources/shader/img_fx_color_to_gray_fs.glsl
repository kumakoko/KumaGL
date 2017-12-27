//彩色图转灰度图的 fragment shader

#version 330 core
in vec2 frag_tex_coord_1;

out vec4 final_color;

uniform sampler2D main_texture;

// 彩色转灰度
vec3 ColorToGray(vec3 color_rgb)
{
    vec3 w = vec3(0.2125,0.7154,0.0721);
    float luminace = dot( color_rgb,w);
    return vec3(luminace,luminace,luminace);
}

void main()
{
    vec4 texture_color = texture(main_texture, frag_tex_coord_1);
    final_color = vec4(ColorToGray(texture_color.rgb),texture_color.a);
    //final_color = texture_color;
}