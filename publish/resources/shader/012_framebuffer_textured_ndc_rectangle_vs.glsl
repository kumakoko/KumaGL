// 绘制一个不带光照的，只使用了一层纹理的基于NDC坐标的矩形
// 的vertex shader
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 input_tex_coords;

out vec2 fragment_tex_coord;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f); 
    fragment_tex_coord = input_tex_coords;
}  