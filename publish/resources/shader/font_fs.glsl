// 展示简单的显示纹理贴图的shader
#version 330 core

in vec2 fragment_tex_coord;
in vec4 fragment_color;
out vec4 color;

uniform sampler2D font_texture;

void main()
{
	color = texture(font_texture, fragment_tex_coord) * fragment_color;
}