// 展示简单的显示纹理贴图的shader
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 input_tex_coord_1;
layout (location = 3) in vec2 input_tex_coord_2;
layout (location = 4) in vec2 input_tex_coord_3;
layout (location = 5) in vec2 input_tex_coord_4;

out vec3 vs_output_color;
out vec2 frag_tex_coord_1;
out vec2 frag_tex_coord_2;
out vec2 frag_tex_coord_3;
out vec2 frag_tex_coord_4;

void main()
{
	gl_Position = vec4(position, 1.0f);
	vs_output_color = color;
	frag_tex_coord_1 = vec2(input_tex_coord_1.x, 1.0 - input_tex_coord_1.y);
	frag_tex_coord_2 = vec2(input_tex_coord_2.x, 1.0 - input_tex_coord_2.y);
	frag_tex_coord_3 = vec2(input_tex_coord_3.x, 1.0 - input_tex_coord_3.y);
	frag_tex_coord_4 = vec2(input_tex_coord_4.x, 1.0 - input_tex_coord_4.y);
}