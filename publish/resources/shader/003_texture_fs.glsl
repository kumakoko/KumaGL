// 展示简单的显示纹理贴图的shader
#version 330 core
in vec4 fragment_color;
in vec2 fragment_tex_coord;

out vec4 color;

uniform sampler2D source_texture_1;
uniform sampler2D source_texture_2;

void main()
{
    color = mix(texture(source_texture_1, fragment_tex_coord), texture(source_texture_2, fragment_tex_coord), 0.5) * fragment_color;
}