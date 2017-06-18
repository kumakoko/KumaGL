// 演示数字图像处理效果的vertex shader
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 input_tex_coord_1;

out vec2 frag_tex_coord_1;

void main()
{
	gl_Position = vec4(position,0.0,1.0);
	frag_tex_coord_1 = vec2(input_tex_coord_1.x, 1.0 - input_tex_coord_1.y);
}