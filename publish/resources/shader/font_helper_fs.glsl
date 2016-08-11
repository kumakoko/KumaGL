// 展示简单的显示纹理贴图的shader
#version 330 core

in vec2 fragment_tex_coord;
out vec4 color;

uniform sampler2D source_texture;

void main()
{
	color = texture(source_texture, fragment_tex_coord);
}