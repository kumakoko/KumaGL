/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
// 阴影着色顶点shader
#version 330 core
layout(location = 0) in vec3 vsin_position;
layout(location = 1) in vec3 vsin_normal;
layout(location = 2) in vec2 vsin_texture_coords;

// vertex shader to fragment shader
out V2F
{
	vec3 fragment_position;
	vec3 fragment_normal;
	vec2 fragment_texture_coords;
	vec4 fragment_pos_in_light_space; // 片元变换到光源空间中的位置
} vs_out;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_world_matrix;
uniform mat4 u_light_space_matrix; // 把顶点变换到“经过投影变换的光源空间”后的变换用矩阵

void main()
{
	vs_out.fragment_position = vec3(u_world_matrix * vec4(vsin_position, 1.0));
	vs_out.fragment_normal = transpose(inverse(mat3(u_world_matrix))) * vsin_normal;
	vs_out.fragment_texture_coords = vsin_texture_coords;
	vs_out.fragment_pos_in_light_space = u_light_space_matrix * vec4(vs_out.fragment_position, 1.0);
	gl_Position = u_projection_matrix * u_view_matrix * u_world_matrix * vec4(vsin_position, 1.0);
}