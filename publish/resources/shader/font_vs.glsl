// 展示简单的显示纹理贴图的shader
#version 330 core
layout (location = 0) in vec2 input_position;
layout (location = 1) in vec4 input_color;
layout (location = 2) in vec2 input_tex_coord;

out vec2 fragment_tex_coord;
out vec4 fragment_color;

void main()
{
	gl_Position = vec4(input_position, 0.0f,1.0f);
	fragment_color = input_color;
	fragment_tex_coord = vec2(input_tex_coord.x,input_tex_coord.y);
}