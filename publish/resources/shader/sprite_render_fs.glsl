// 绘制精灵的片元着色器
#version 330 core

in vec2 output_tex_coords;
out vec4 color;

uniform sampler2D sprite_texture;
uniform vec4 sprite_color;

void main()
{
    color = sprite_color * texture(sprite_texture, output_tex_coords);
}