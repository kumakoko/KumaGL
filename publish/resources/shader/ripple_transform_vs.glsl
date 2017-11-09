// 用在水波效果上的顶点着色器

#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_tex_coord_1;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 world_matrix;

out vec4 fragment_color;
out vec2 fragment_tex_coord;

void main()
{
	gl_Position = projection_matrix * view_matrix * world_matrix *  vec4(vertex_position, 1.0f);
    fragment_tex_coord = vec2(input_tex_coord.x, 1.0 - vertex_tex_coord_1.y);
	
	/* 使用内建变量来变换的代码
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
	*/
}