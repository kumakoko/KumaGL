// 模型装载

#version 330 core

in vec2 output_tex_coords;
in vec3 output_normal;
out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{    
    color = vec4(texture(texture_diffuse1, output_tex_coords));
}