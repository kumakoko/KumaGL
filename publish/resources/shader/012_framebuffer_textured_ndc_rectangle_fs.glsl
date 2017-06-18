// 绘制一个不带光照的，只使用了一层纹理的基于NDC坐标的矩形
// 的fragment shader

#version 330 core
in vec2 fragment_tex_coord;

out vec4 color;

uniform sampler2D source_texture_1;

void main()
{             
    color = texture(source_texture_1, fragment_tex_coord);
}