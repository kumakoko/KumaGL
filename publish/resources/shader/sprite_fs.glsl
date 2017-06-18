// 用来绘制精灵的片元着色器

#version 330 core
in vec2 fragment_texture_coords;
out vec4 final_color;

uniform sampler2D sprite_texture;
uniform vec4 sprite_color;

void main()
{    
    final_color = sprite_color * texture(sprite_texture, fragment_texture_coords);
}  